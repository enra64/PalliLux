#ifndef SPECTROMETERCONTROLWIDGET_H
#define SPECTROMETERCONTROLWIDGET_H

#include "controlwidget.h"

#include <spectrometer/spectrometercolordataprovider.h>

class SpectrometerControlWidget : public ControlWidget
{
public:
    SpectrometerControlWidget(LedConfig ledConfig);

    // ControlWidget interface
protected:
    void updateWidgets();

private:
    std::shared_ptr<SpectrometerColorDataProvider> mSpectrometer;

    // ControlWidget interface
public slots:
    void setParameters(int ledOffset, int numberOfLedsPerStereoChannel, double gain){
        mSpectrometer->setParameters(ledOffset, numberOfLedsPerStereoChannel, gain);
    }

    void start(const QString &port);
};

#endif // SPECTROMETERCONTROLWIDGET_H
