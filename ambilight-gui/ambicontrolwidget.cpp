#include "ambicontrolwidget.h"

#include <QDoubleSpinBox>
#include <brightnessfilter.h>
#include <lowpassfilter.h>

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

    // set up combobox for selecting the interpolation type
    setupInterpolationCombobox();

    // nudge the dialog to stay at the smallest possible size after adding all ui
    layout()->setSizeConstraint(QLayout::SetFixedSize);
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

void AmbiControlWidget::onBorderWidthChanged(int newValue) {
    getBorderProvider()->setBorderWidth(newValue);
}

void AmbiControlWidget::onInterpolationChange(int index) {
    getColorDataProvider()->setResizeInterpolationMode(static_cast<AmbiColorDataProvider::CImgInterpolationType>(index - 1));
}

void AmbiControlWidget::setupControlBox() {
    // set up new data factor widgets
    QLabel* newDataFactorLabel = new QLabel("New data factor", parentWidget());
    QDoubleSpinBox* newDataFactorSpinbox = new QDoubleSpinBox(parentWidget());

    // set range
    newDataFactorSpinbox->setRange(0, 1);

    // connect to update signal
    connect(newDataFactorSpinbox, SIGNAL(valueChanged(double)), this, SLOT(onNewDataFactorChanged(double)));

    // add to control layout
    addControlWidget(newDataFactorLabel, newDataFactorSpinbox);



    // set up new data factor widgets
    QLabel* brightnessFactorLabel = new QLabel("Brightness factor", parentWidget());
    QDoubleSpinBox* brightnessFactorSpinbox = new QDoubleSpinBox(parentWidget());

    // set range
    brightnessFactorSpinbox->setRange(0, 1);

    // connect to update signal
    connect(brightnessFactorSpinbox, SIGNAL(valueChanged(double)), this, SLOT(onBrightnessFactorChanged(double)));

    // add to control layout
    addControlWidget(brightnessFactorLabel, brightnessFactorSpinbox);



    // set up border width widgets
    QLabel* borderWidthLabel = new QLabel("Border width", parentWidget());
    QSpinBox* borderWidthSpinbox = new QSpinBox(parentWidget());

    // set range
    borderWidthSpinbox->setRange(1, 1000);

    // connect to update signal
    connect(borderWidthSpinbox, SIGNAL(valueChanged(int)), this, SLOT(onBorderWidthFactorChanged(int)));

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

void AmbiControlWidget::setupInterpolationCombobox() {
    // create combobox for interpolation types
    mInterpolationComboBox = new QComboBox();

    // add combobox to control layout
    addControlWidget(mInterpolationComboBox);

    // make a list of available interpolation types
    QStringList interpolationTypes;
    interpolationTypes <<
                       "Raw memory resizing" <<
                       "Fill-space-interpolation" <<
                       "Nearest Neighbor interpolation" <<
                       "Moving average interpolation" <<
                       "Linear interpolation" <<
                       "Grid interpolation" <<
                       "Cubic interpolation" <<
                       "Lanczos interpolation";

    // insert interpolation types into selection box
    mInterpolationComboBox->addItems(interpolationTypes);

    // get the currently set interpolation mode
    AmbiColorDataProvider::CImgInterpolationType i = getColorDataProvider()->getResizeInterpolationMode();

    // enum range is -1 to 6, but our index here starts at 0 -> +1
    mInterpolationComboBox->setCurrentIndex(
        // convert enum to integer, offset by the difference in starting indices
        static_cast<typename std::underlying_type<AmbiColorDataProvider::CImgInterpolationType>::type>(i) + 1);

    // connect to currentIndexChanged after having it triggered programmatically
    connect(mInterpolationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInterpolationChange(int)));
}
