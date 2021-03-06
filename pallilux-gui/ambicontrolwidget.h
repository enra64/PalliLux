#ifndef AMBICONTROLWIDGET_H
#define AMBICONTROLWIDGET_H

#include "controlwidget.h"
#include "fpsmeter.h"
#include "histogramwidget.h"
#include "pixellinewidget.h"

#include <QComboBox>

class AmbiControlWidget : public ControlWidget
{
    Q_OBJECT
public:
    explicit AmbiControlWidget(
            std::shared_ptr<ArduinoConnector> connector,
            QWidget *parent = 0);

private slots:
    void onNewDataFactorChanged(double newValue);
    void onBrightnessFactorChanged(double newValue);
    void onSaturationFactorChanged(double newValue);
    void onBorderWidthChanged(int newValue);


protected:// ControlWidget interface
    void updateWidgets() override;

private:// member functions
    /**
     * @brief Set up all widgets we want n the control box
     */
    void setupControlBox();

    std::shared_ptr<AmbiColorDataProvider> getColorDataProvider(){
        return std::dynamic_pointer_cast<AmbiColorDataProvider>(
                    mArduinoConnector->getColorDataProvider());
    }

    std::shared_ptr<BorderProvider> getBorderProvider(){
        return std::dynamic_pointer_cast<BorderProvider>(getColorDataProvider()->getBorderProvider());
    }

private:// info widgets
    HistogramWidget* mHistogramWidget;
    PixelLineWidget* mLastLineWidget;
    FpsMeter* mFpsMeter;
};

#endif // AMBICONTROLWIDGET_H
