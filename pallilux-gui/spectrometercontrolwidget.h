#ifndef SPECTROMETERCONTROLWIDGET_H
#define SPECTROMETERCONTROLWIDGET_H

#include "controlwidget.h"

#include "spectrometer/spectrometercolordataprovider.h"
#include "amplitudemapper.h"
#include "singlecolormapper.h"
#include "singlerotatingcolormapper.h"

#include <QDoubleSpinBox>

class SpectrometerControlWidget : public ControlWidget
{
    Q_OBJECT
public:
    SpectrometerControlWidget(LedConfig ledConfig);

    double getGain() {
        return mSpectrometer->getGain();
    }

    // ControlWidget interface
protected:
    void updateWidgets();

private:
    std::shared_ptr<SpectrometerColorDataProvider> mSpectrometer;
    SingleColorMapper *mSingleColorMapper = nullptr;
    SingleRotatingColorMapper *mRotatingSingleColorMapper = nullptr;
    AmplitudeMapper* mAmplitudeMapper = nullptr;

    QDoubleSpinBox* mGainSpinBox = nullptr;

    void addLabel(const QString& text);
    void setupControlBox();

signals:
    void gainChanged(double newGain);

private slots:
    void onNewDataFactorChanged(double newValue);
    void onBrightnessFactorChanged(double newValue);
    void onAutomaticGainControlClicked(bool newValue);
    void mappingSelected(const QString& text);
    // ControlWidget interface
public slots:
    void setParameters(int ledOffset, int numberOfLedsPerStereoChannel, double gain){
        mSpectrometer->setParameters(ledOffset, numberOfLedsPerStereoChannel, gain);
    }

    void start(const QString &port);
};

#endif // SPECTROMETERCONTROLWIDGET_H
