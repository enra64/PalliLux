#include "backlightcontrolwidget.h"

#include <QLabel>
#include <QColorDialog>

#include <pulsingfilter.h>

using namespace std;

BacklightControlWidget::BacklightControlWidget(LedConfig d, QWidget *parent) :
    ControlWidget(parent) {

    // init & set ColorDataProvider
    mConstantColorProvider = shared_ptr<ConstantColorDataProvider>(new ConstantColorDataProvider(d));
    mArduinoConnector->setColorDataProvider(mConstantColorProvider);

    // color sliders
    mRedSlider = new QSlider(parentWidget());
    mRedSlider->setOrientation(Qt::Horizontal);
    addControlWidget(new QLabel("Red", parentWidget()), mRedSlider);
    connect(mRedSlider, SIGNAL(valueChanged(int)), this, SLOT(onRedChanged(int)));

    mGreenSlider = new QSlider(parentWidget());
    mGreenSlider->setOrientation(Qt::Horizontal);
    addControlWidget(new QLabel("Green", parentWidget()), mGreenSlider);
    connect(mGreenSlider, SIGNAL(valueChanged(int)), this, SLOT(onGreenChanged(int)));

    mBlueSlider = new QSlider(parentWidget());
    mBlueSlider->setOrientation(Qt::Horizontal);
    addControlWidget(new QLabel("Blue", parentWidget()), mBlueSlider);
    connect(mBlueSlider, SIGNAL(valueChanged(int)), this, SLOT(onBlueChanged(int)));

    // new frequency slider
    mFrequencySlider = new QSlider(parentWidget());
    mFrequencySlider->setOrientation(Qt::Horizontal);
    addControlWidget(new QLabel("Period"), mFrequencySlider);
    connect(mFrequencySlider, SIGNAL(valueChanged(int)), this, SLOT(onPeriodChanged(int)));

    // new amplitude qspinbox
    mAmplitudeSpinbox = new QDoubleSpinBox(parentWidget());
    addControlWidget(new QLabel("Amplitude"), mAmplitudeSpinbox);
    connect(mAmplitudeSpinbox, SIGNAL(valueChanged(double)), this, SLOT(onAmplitudeChanged(double)));

    mConstantColorProvider->setRgb(0, 0, 0);

    // add pulsing filter
    unique_ptr<DataFilter> pulsingFilter(new PulsingFilter(100, 0));
    mArduinoConnector->addFilter("pulse", std::move(pulsingFilter));

    // show 'none' in misc group box
    addMiscWidget(new QLabel("none"));
}



void BacklightControlWidget::onColorPickerClicked() {
    // recreate current color to set start value for color picker
    QColor pastColor(mRedSlider->value(),
                     mGreenSlider->value(),
                     mBlueSlider->value());

    // get color picker
    QColor result = QColorDialog::getColor(pastColor, this);

    // notify color provider of change
    mConstantColorProvider->setRgb(
        result.red(),
        result.green(),
        result.blue());

    // notify sliders
    mRedSlider->setValue(result.red());
    mGreenSlider->setValue(result.green());
    mBlueSlider->setValue(result.blue());
}

void BacklightControlWidget::onPeriodChanged(int period) {
    PulsingFilter* filter = dynamic_cast<PulsingFilter*>(mArduinoConnector->getFilter("pulse").get());
    filter->setCycleLength(period);
}

void BacklightControlWidget::onAmplitudeChanged(double amplitude) {
    PulsingFilter* filter = dynamic_cast<PulsingFilter*>(mArduinoConnector->getFilter("pulse").get());
    filter->setMaximumAmplitude(amplitude);
}


void BacklightControlWidget::updateWidgets(){
}
