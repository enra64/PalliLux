#ifndef BACKLIGHTDIALOG_H
#define BACKLIGHTDIALOG_H

#include <QDialog>

#include <arduinoconnector.h>
#include <constantcolordataprovider.h>
#include <memory>

namespace Ui {
class BacklightDialog;
}

class BacklightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BacklightDialog(int horizontalBorderLedCount, int verticalBorderLedCount, std::string port, QWidget *parent = 0);
    ~BacklightDialog();

private slots:
    void on_backlightControlRedSlider_sliderMoved(int position);

    void on_backlightControlGreenSlider_sliderMoved(int position);

    void on_backlightControlBlueSlider_sliderMoved(int position);

    void on_backlightControlFrequencySlider_sliderMoved(int position);

    void on_runButton_clicked();

    void on_stopButton_clicked();

    void on_backlightControlColorPickerButton_clicked();

    void on_backlightControlAmplitudeSpinbox_valueChanged(double arg1);

private:
    uint8_t mFrequency;

    std::shared_ptr<ConstantColorDataProvider> mColorProvider;
    std::shared_ptr<ArduinoConnector> mArduinoConnector;

    bool mStopClicked = false;

    Ui::BacklightDialog *ui;
    void setButtonState(bool currentlyRunning);
};

#endif // BACKLIGHTDIALOG_H
