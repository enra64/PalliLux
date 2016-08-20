#ifndef CONTROLDIALOG_H
#define CONTROLDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

// using namespace qtcharts
QT_CHARTS_USE_NAMESPACE

#include <memory>

#include <arduinoconnector.h>
#include <singlescreenborderprovider.h>

namespace Ui {
class ControlDialog;
}

enum struct ProgressState {
    indefinite,
    failure,
    working
};

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

private:
    void setButtonState(bool currentlyRunning);
    void updateStatus(const std::__cxx11::string &msg, bool isFailure = false);
    void updateProgressbar(ProgressState state, int progress = 0, int maximum = 1);
    Ui::ControlDialog *ui;
    std::shared_ptr<ArduinoConnector> mArduinoConnector;

private:// fps chart stuff
    QLineSeries* mFpsLineSeries = new QLineSeries();
    QChart* mFpsChart = new QChart();
    QChartView* mFpsChartView;
    uint mFpsTickCount = 0;
    const int mFpsPointCount = 200;
    std::shared_ptr<SingleScreenBorderProvider> getBorderProvider();
};

#endif // CONTROLDIALOG_H
