#include "spectrometercontrolwidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>

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

    QComboBox* mappingSelector = new QComboBox();
    mappingSelector->addItem("Single Color Mapping");
    mappingSelector->addItem("Amplitude Mapping");
    mappingSelector->addItem("Rotating Single Color Mapping");

    connect(mappingSelector, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), this, &SpectrometerControlWidget::mappingSelected);

    addControlWidget(mappingSelector);

    mappingSelected("Single Color Mapping");
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
