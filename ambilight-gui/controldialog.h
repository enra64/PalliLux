#ifndef CONTROLDIALOG_H
#define CONTROLDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTemporaryFile>

#include <memory>

#include <arduinoconnector.h>
#include <singlescreenborderprovider.h>

class FpsMeter;
class HistogramWidget;
class PixelLineWidget;

namespace Ui {
class ControlDialog;
}

class ControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ControlDialog(std::shared_ptr<ArduinoConnector> connector, QWidget *parent = 0, const QString &infoString = "");
    ~ControlDialog();


private slots:
    void on_runButton_clicked();

    void on_stopButton_clicked();

    void on_newdataFactorSpinbox_valueChanged(double arg1);

    void on_brightnessFactorSpinbox_valueChanged(double arg1);

    void on_borderWidthSpinbox_valueChanged(int arg1);

    void on_interpolationTypeComboBox_currentIndexChanged(int index);

private:
    void setButtonState(bool currentlyRunning);
    void updateStatus(const std::string &msg, bool isFailure = false);
    Ui::ControlDialog *ui;
    std::shared_ptr<ArduinoConnector> mArduinoConnector;
    bool mStopClicked = false;
    void setupInterpolationCombobox();

private:// info widgets
    HistogramWidget* mHistogramWidget;
    PixelLineWidget* mLastLineWidget;


private:// fps chart stuff
    FpsMeter* mFpsMeter;
    std::shared_ptr<BorderProvider> getBorderProvider();
    std::shared_ptr<AmbiColorDataProvider> getColorDataProvider();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *) override;
};

#endif // CONTROLDIALOG_H
