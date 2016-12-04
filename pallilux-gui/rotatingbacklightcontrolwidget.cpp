#include "rotatingbacklightcontrolwidget.h"

#include <QLabel>
#include <QColorDialog>

#include <pulsingfilter.h>

using namespace std;

RotatingBacklightControlWidget::RotatingBacklightControlWidget(LedConfig d, QWidget *parent) :
    ControlWidget(parent) {
    const double initialRotationSpeed = 1;
    // init & set ColorDataProvider
    mRotatingColorProvider = shared_ptr<RotatingColorDataProvider>(
                new RotatingColorDataProvider(d, initialRotationSpeed));
    mArduinoConnector->setColorDataProvider(mRotatingColorProvider);


    /*
     *
     *
     * rotating rgb color data provider config
     *
     *
     */
    mRotationFrequencySlider = new QSlider();
    mRotationFrequencySlider->setRange(0, 10);
    mRotationFrequencySlider->setValue(initialRotationSpeed);
    mRotationFrequencySlider->setOrientation(Qt::Horizontal);
    connect(mRotationFrequencySlider, SIGNAL(valueChanged(int)), this, SLOT(onConfigChanged()));
    addControlWidget(new QLabel("Rotation Speed"), mRotationFrequencySlider);

    mMaximumSlider = new QSlider();
    mMaximumSlider->setRange(0, 360);
    mMaximumSlider->setValue(360);
    mMaximumSlider->setOrientation(Qt::Horizontal);
    connect(mMaximumSlider, SIGNAL(valueChanged(int)), this, SLOT(onConfigChanged()));
    addControlWidget(new QLabel("maximum hue"), mMaximumSlider);

    mMinimumSlider = new QSlider();
    mMinimumSlider->setRange(0, 360);
    mMinimumSlider->setValue(0);
    mMinimumSlider->setOrientation(Qt::Horizontal);
    connect(mMinimumSlider, SIGNAL(valueChanged(int)), this, SLOT(onConfigChanged()));
    addControlWidget(new QLabel("minimum hue"), mMinimumSlider);

    mSaturationSlider = new QSlider();
    mSaturationSlider->setRange(0, 100);
    mSaturationSlider->setValue(100);
    mSaturationSlider->setOrientation(Qt::Horizontal);
    connect(mSaturationSlider, SIGNAL(valueChanged(int)), this, SLOT(onConfigChanged()));
    addControlWidget(new QLabel("saturation"), mSaturationSlider);

    mValueSlider = new QSlider();
    mValueSlider->setRange(0, 100);
    mValueSlider->setValue(100);
    mValueSlider->setOrientation(Qt::Horizontal);
    connect(mValueSlider, SIGNAL(valueChanged(int)), this, SLOT(onConfigChanged()));
    addControlWidget(new QLabel("value"), mValueSlider);

    /*
     *
     * pulse filter config sliders
     *
     *
     */
    mPulseFrequencySlider = new QSlider(parentWidget());
    mPulseFrequencySlider->setRange(1, 200);
    mPulseFrequencySlider->setValue(1);
    mPulseFrequencySlider->setOrientation(Qt::Horizontal);
    addControlWidget(new QLabel("Pulse speed"), mPulseFrequencySlider);
    connect(mPulseFrequencySlider, SIGNAL(valueChanged(int)), this, SLOT(onPulseConfigChanged()));

    // new amplitude qspinbox
    mPulseAmplitudeSlider = new QSlider(parentWidget());
    mPulseAmplitudeSlider->setRange(0, 200);
    mPulseAmplitudeSlider->setValue(0);
    mPulseAmplitudeSlider->setOrientation(Qt::Horizontal);
    addControlWidget(new QLabel("Pulse Amplitude"), mPulseAmplitudeSlider);
    connect(mPulseAmplitudeSlider, SIGNAL(valueChanged(int)), this, SLOT(onPulseConfigChanged()));

    // add pulsing filter
    unique_ptr<DataFilter> pulsingFilter(new PulsingFilter(100, 0));
    mArduinoConnector->addFilter("pulse", std::move(pulsingFilter));

    // show 'none' in misc group box
    addMiscWidget(new QLabel("none"));
}

void RotatingBacklightControlWidget::onPulseConfigChanged(){
    PulsingFilter* filter = dynamic_cast<PulsingFilter*>(mArduinoConnector->getFilter("pulse").get());
    filter->setCycleLength(mPulseFrequencySlider->value());
    filter->setMaximumAmplitude(double(mPulseAmplitudeSlider->value()) / 100.0);
}

void RotatingBacklightControlWidget::onConfigChanged(){
    mRotatingColorProvider->setRange(mMinimumSlider->value(), mMaximumSlider->value());
    mRotatingColorProvider->setSV(double(mSaturationSlider->value()) / 100.0, double(mValueSlider->value()) / 100.0);
    mRotatingColorProvider->setSpeed(mRotationFrequencySlider->value());
}

void RotatingBacklightControlWidget::updateWidgets(){
}
