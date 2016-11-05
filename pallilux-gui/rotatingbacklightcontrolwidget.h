#ifndef BACKLIGHTCONTROLWIDGET_H
#define BACKLIGHTCONTROLWIDGET_H

#include "controlwidget.h"

#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSlider>
#include <rotatingcolordataprovider.h>

class RotatingBacklightControlWidget : public ControlWidget
{
    Q_OBJECT
public:
    explicit RotatingBacklightControlWidget(
            LedConfig d,
            QWidget *parent = 0);

private:
    std::shared_ptr<RotatingColorDataProvider> mRotatingColorProvider;///< color provider used for the backlight
    QSlider* mRotationFrequencySlider; ///< slider for the rotation speed
    QSlider* mMaximumSlider; ///< slider for maximum hue
    QSlider* mMinimumSlider; ///< slider for minimum hue
    QSlider* mSaturationSlider; ///< slider for saturation
    QSlider* mValueSlider; ///< slider for value

    QSlider* mPulseFrequencySlider; ///< slider for the pulse frequency
    QSlider* mPulseAmplitudeSlider;///< slider for the pulse amplitude

private slots:
    void onPulseConfigChanged();
    void onConfigChanged();

    // ControlWidget interface
protected:
    void updateWidgets() override;
};

#endif // BACKLIGHTCONTROLWIDGET_H
