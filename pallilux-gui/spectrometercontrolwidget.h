#ifndef SPECTROMETERCONTROLWIDGET_H
#define SPECTROMETERCONTROLWIDGET_H

#include "controlwidget.h"

#include <spectrometercolordataprovider.h>



class SpectrometerControlWidget : public ControlWidget
{
public:
    SpectrometerControlWidget(LedConfig ledConfig);

    // ControlWidget interface
protected:
    void updateWidgets();

private:
    std::shared_ptr<SpectrometerColorDataProvider> mSpectrometer;
};

#endif // SPECTROMETERCONTROLWIDGET_H
