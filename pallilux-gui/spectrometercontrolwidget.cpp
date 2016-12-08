#include "spectrometercontrolwidget.h"

using namespace std;

SpectrometerControlWidget::SpectrometerControlWidget(LedConfig ledConfig)
{
    mSpectrometer = shared_ptr<SpectrometerColorDataProvider>(new SpectrometerColorDataProvider(ledConfig));
    mArduinoConnector->setColorDataProvider(mSpectrometer);
}

void SpectrometerControlWidget::updateWidgets()
{

}

void SpectrometerControlWidget::start(const QString &port)
{
    mSpectrometer->start();
    ControlWidget::start(port);
}
