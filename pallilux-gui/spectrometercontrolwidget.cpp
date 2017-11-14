#include "spectrometercontrolwidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>

#include <brightnessfilter.h>
#include <lowpassfilter.h>

using namespace std;

/**
 * @brief Convert a string to an integer for use in the mapping selection switch.
 * From http://stackoverflow.com/a/16388610
 */
constexpr unsigned int str2int(const char* str, int h = 0){
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

SpectrometerControlWidget::SpectrometerControlWidget(LedConfig ledConfig)
{
    mSpectrometer = shared_ptr<SpectrometerColorDataProvider>(new SpectrometerColorDataProvider(ledConfig));
    mArduinoConnector->setColorDataProvider(mSpectrometer);

    // add the filters we want
    mArduinoConnector->addFilter("lowpass", std::unique_ptr<DataFilter>(new LowPassFilter(getColorDataProvider()->getRequiredBufferLength(), .6f)));
    mArduinoConnector->addFilter("brightness", std::unique_ptr<DataFilter>(new BrightnessFilter(1)));

    // set up control box (mapping selector, new data factor, brightness factor)
    setupControlBox();
}

void SpectrometerControlWidget::setupControlBox() {
    QComboBox* mappingSelector = new QComboBox();
    mappingSelector->addItem("Single Color Mapping");
    mappingSelector->addItem("Amplitude Mapping");
    mappingSelector->addItem("Rotating Single Color Mapping");

    // weave into the fabric of the ui
    connect(mappingSelector, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), this, &SpectrometerControlWidget::mappingSelected);
    addControlWidget("Amplitude-Color-Mapper", mappingSelector);

    // auto-select the single color mapping
    mappingSelected("Single Color Mapping");

    // set up new data factor widgets
    QDoubleSpinBox* newDataFactorSpinbox = new QDoubleSpinBox(parentWidget());

    // set range
    newDataFactorSpinbox->setRange(0, 1);
    newDataFactorSpinbox->setValue(1);
    newDataFactorSpinbox->setSingleStep(.1);

    // connect to update signal
    connect(newDataFactorSpinbox, SIGNAL(valueChanged(double)), this, SLOT(onNewDataFactorChanged(double)));

    // add to control layout
    addControlWidget("New data factor", newDataFactorSpinbox);


    // set up brightness factor widgets
    QDoubleSpinBox* brightnessFactorSpinbox = new QDoubleSpinBox(parentWidget());

    // set range
    brightnessFactorSpinbox->setRange(0, 1);
    brightnessFactorSpinbox->setValue(1);
    brightnessFactorSpinbox->setSingleStep(.1);

    // connect to update signal
    connect(brightnessFactorSpinbox, SIGNAL(valueChanged(double)), this, SLOT(onBrightnessFactorChanged(double)));

    // add to control layout
    addControlWidget("Brightness factor", brightnessFactorSpinbox);
}

void SpectrometerControlWidget::onNewDataFactorChanged(double newValue) {
    // im sorry
    LowPassFilter* filter = dynamic_cast<LowPassFilter*>(mArduinoConnector->getFilter("lowpass").get());
    // check that cast & finding worked
    assert(filter);
    // apply change
    filter->setNewDataFactor((float) newValue);
}

void SpectrometerControlWidget::onBrightnessFactorChanged(double newValue) {
    // sorry for this, too
    BrightnessFilter* filter = dynamic_cast<BrightnessFilter*>(mArduinoConnector->getFilter("brightness").get());
    // check that cast & finding worked
    assert(filter);
    // apply change
    filter->setFactor((float) newValue);
}

void SpectrometerControlWidget::updateWidgets()
{
}

void SpectrometerControlWidget::addLabel(const QString &text)
{
    QLabel* speedSpinLabel = new QLabel();
    speedSpinLabel->setText(text);
    addMiscWidget(speedSpinLabel);
}

void SpectrometerControlWidget::mappingSelected(const QString &text)
{
    // remove all control widgets so we can add other ones for the selected mapper
    clearMiscWidgets();

    switch(str2int(text.toStdString().c_str())){
        case str2int("Single Color Mapping"):{
            QDoubleSpinBox* colorSpin = new QDoubleSpinBox();
            colorSpin->setMaximum(360);
            connect(colorSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                [=](double hue){ mSingleColorMapper->setColor(hue); });

            addLabel("Hue");
            addMiscWidget(colorSpin);

            mSingleColorMapper = new SingleColorMapper(255, 0, 0);
            mSpectrometer->setColorMapper(mSingleColorMapper);
            break;
        }
        case str2int("Rotating Single Color Mapping"):{
            QDoubleSpinBox* speedSpin = new QDoubleSpinBox();
            double defaultSpeed = 100.0;
            speedSpin->setMinimum(0);
            speedSpin->setMaximum(1000);
            speedSpin->setValue(defaultSpeed);
            speedSpin->setSingleStep(0.001);

            connect(speedSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                [=](double speed){ mRotatingSingleColorMapper->setSpeed(speed); });


            mRotatingSingleColorMapper = new SingleRotatingColorMapper(defaultSpeed);
            mSpectrometer->setColorMapper(mRotatingSingleColorMapper);

            addLabel("Rotation Speed");

            addMiscWidget(speedSpin);
            break;
        }
        case str2int("Amplitude Mapping"):{
            QDoubleSpinBox* minHueSpin = new QDoubleSpinBox();
            minHueSpin->setMaximum(360);
            connect(minHueSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                [=](double min){ mAmplitudeMapper->setMinimumHue(min); });
            addLabel("Minimum Hue");
            addMiscWidget(minHueSpin);

            QDoubleSpinBox* maxHueSpin = new QDoubleSpinBox();
            maxHueSpin->setMaximum(360);
            connect(maxHueSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                [=](double max){ mAmplitudeMapper->setMaximumHue(max); });

            addLabel("Maximum Hue");
            addMiscWidget(maxHueSpin);

            mAmplitudeMapper = new AmplitudeMapper();
            mSpectrometer->setColorMapper(mAmplitudeMapper);
            break;
        }
    }
}

void SpectrometerControlWidget::start(const QString &port)
{
    mSpectrometer->start();
    ControlWidget::start(port);
}
