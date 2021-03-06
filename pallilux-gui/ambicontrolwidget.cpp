#include "ambicontrolwidget.h"

#include <QDoubleSpinBox>
#include <brightnessfilter.h>
#include <lowpassfilter.h>
#include <saturationfilter.h>

AmbiControlWidget::AmbiControlWidget(std::shared_ptr<ArduinoConnector> connector, QWidget *parent) :
    ControlWidget(parent) {

    // override the connector created by the ControlWidget constructor
    mArduinoConnector = connector;

    // set up last line widget
    mLastLineWidget = new PixelLineWidget(this);
    addMiscWidget(mLastLineWidget);

    // set up fps widget
    mFpsMeter = new FpsMeter(this);
    addMiscWidget(mFpsMeter);

    // set up histogram
    mHistogramWidget = new HistogramWidget(this);
    addMiscWidget(mHistogramWidget);

    // set up most control widgets
    setupControlBox();

    // add the filters we want
    mArduinoConnector->addFilter("lowpass", std::unique_ptr<DataFilter>(new LowPassFilter(getColorDataProvider()->getRequiredBufferLength(), .6f)));
    mArduinoConnector->addFilter("brightness", std::unique_ptr<DataFilter>(new BrightnessFilter(1)));
    mArduinoConnector->addFilter("saturation", std::unique_ptr<DataFilter>(new SaturationFilter(1)));

    // lock to 60 fps
    //mArduinoConnector->setTargetFps(60);
}

void AmbiControlWidget::onNewDataFactorChanged(double newValue) {
    // im sorry
    LowPassFilter* filter = dynamic_cast<LowPassFilter*>(mArduinoConnector->getFilter("lowpass").get());
    // check that cast & finding worked
    assert(filter);
    // apply change
    filter->setNewDataFactor((float) newValue);
}

void AmbiControlWidget::onBrightnessFactorChanged(double newValue) {
    // sorry for this, too
    BrightnessFilter* filter = dynamic_cast<BrightnessFilter*>(mArduinoConnector->getFilter("brightness").get());
    // check that cast & finding worked
    assert(filter);
    // apply change
    filter->setFactor((float) newValue);
}

void AmbiControlWidget::onSaturationFactorChanged(double newValue)
{
    // also, sorry for this
    SaturationFilter* filter = dynamic_cast<SaturationFilter*>(mArduinoConnector->getFilter("saturation").get());
    // check that cast & finding worked
    assert(filter);
    // apply change
    filter->setFactor((float) newValue);
}

void AmbiControlWidget::onBorderWidthChanged(int newValue) {
    getBorderProvider()->setBorderWidth(newValue);
}

void AmbiControlWidget::setupControlBox() {
    // set up new data factor widgets
    QLabel* newDataFactorLabel = new QLabel("New data factor", parentWidget());
    QDoubleSpinBox* newDataFactorSpinbox = new QDoubleSpinBox(parentWidget());

    // set range
    newDataFactorSpinbox->setRange(0, 1);
    newDataFactorSpinbox->setValue(.6);
    newDataFactorSpinbox->setSingleStep(.1);

    // connect to update signal
    connect(newDataFactorSpinbox, SIGNAL(valueChanged(double)), this, SLOT(onNewDataFactorChanged(double)));

    // add to control layout
    addControlWidget(newDataFactorLabel, newDataFactorSpinbox);



    // set up brightness factor widgets
    QLabel* brightnessFactorLabel = new QLabel("Brightness factor", parentWidget());
    QDoubleSpinBox* brightnessFactorSpinbox = new QDoubleSpinBox(parentWidget());

    // set range
    brightnessFactorSpinbox->setRange(0, 1);
    brightnessFactorSpinbox->setValue(1);
    brightnessFactorSpinbox->setSingleStep(.1);

    // connect to update signal
    connect(brightnessFactorSpinbox, SIGNAL(valueChanged(double)), this, SLOT(onBrightnessFactorChanged(double)));

    // add to control layout
    addControlWidget(brightnessFactorLabel, brightnessFactorSpinbox);



    // set up saturation factor widgets
    QDoubleSpinBox* saturationFactorSpinbox = new QDoubleSpinBox(parentWidget());

    // set range
    saturationFactorSpinbox->setRange(0, 3);
    saturationFactorSpinbox->setValue(1);
    saturationFactorSpinbox->setSingleStep(.1);

    // connect to update signal
    connect(saturationFactorSpinbox, SIGNAL(valueChanged(double)), this, SLOT(onSaturationFactorChanged(double)));

    // add to control layout
    addControlWidget(new QLabel("Saturation factor"), saturationFactorSpinbox);



    // set up border width widgets
    QLabel* borderWidthLabel = new QLabel("Border width", parentWidget());
    QSpinBox* borderWidthSpinbox = new QSpinBox(parentWidget());

    // set range
    borderWidthSpinbox->setRange(1, 1000);
    borderWidthSpinbox->setValue(100);

    // connect to update signal
    connect(borderWidthSpinbox, SIGNAL(valueChanged(int)), this, SLOT(onBorderWidthChanged(int)));

    // add to control layout
    addControlWidget(borderWidthLabel, borderWidthSpinbox);
}

void AmbiControlWidget::updateWidgets() {
    // get last border line
    std::unique_ptr<Image> lastLine = getColorDataProvider()->getLastLineImage();

    // update fps widget
    mFpsMeter->update(mArduinoConnector->getCurrentFps());

    // update lastLine widget
    mLastLineWidget->update(lastLine.get());

    // update histogram chart
    mHistogramWidget->update(lastLine.get());
}
