#ifndef BACKLIGHTCONTROLWIDGET_H
#define BACKLIGHTCONTROLWIDGET_H

#include "controlwidget.h"

#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSlider>
#include <constantcolordataprovider.h>

class BacklightControlWidget : public ControlWidget
{
    Q_OBJECT
public:
    explicit BacklightControlWidget(
            LedConfig d,
            QWidget *parent = 0);

private slots:
    /**
     * @brief Change red intensity
     */
    void onRedChanged(int val) {
        assert(val > 0 && val < 256);
        mConstantColorProvider->setRed(static_cast<uint8_t>(val));
    }

    void onGreenChanged(int val){
        assert(val > 0 && val < 256);
        mConstantColorProvider->setGreen(static_cast<uint8_t>(val));
    }

    void onBlueChanged(int val){
        assert(val > 0 && val < 256);
        mConstantColorProvider->setBlue(static_cast<uint8_t>(val));
    }

    void onColorPickerClicked();

    void onPeriodChanged(int period);
    void onAmplitudeChanged(double amplitude);

private:
    std::shared_ptr<ConstantColorDataProvider> mConstantColorProvider;///< color provider used for the backlight
    QSlider* mFrequencySlider; ///< slider for the pulse period
    QSlider* mRedSlider; ///< slider for red intensity
    QSlider* mGreenSlider; ///< slider for green intensity
    QSlider* mBlueSlider; ///< slider for blue intensity
    QPushButton* mColorPickerButton; ///< button for activating the color picker
    QDoubleSpinBox* mAmplitudeSpinbox; ///< spinbox for setting maximum amplitude
    uint8_t mFrequency; ///< pulse period

    // ControlWidget interface
protected:
    void updateWidgets() override;
};

#endif // BACKLIGHTCONTROLWIDGET_H
