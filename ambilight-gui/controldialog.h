#ifndef CONTROLDIALOG_H
#define CONTROLDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
QT_CHARTS_USE_NAMESPACE // using namespace qtcharts

#include <memory>

#include <QLabel>
#include <arduinoconnector.h>
#include <singlescreenborderprovider.h>

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

    void on_fpsCheckbox_clicked(bool checked);

    void on_histogramCheckbox_clicked(bool checked);

    void on_lineCheckbox_clicked(bool checked);

private:
    void setButtonState(bool currentlyRunning);
    void updateStatus(const std::__cxx11::string &msg, bool isFailure = false);
    Ui::ControlDialog *ui;
    std::shared_ptr<ArduinoConnector> mArduinoConnector;

private:
    bool mEnableFpsChart = false;
    bool mEnableHistogram = false;
    bool mEnableLastLineView = false;

private:// last line view
    QLabel* mLastLineView;

private:// histogram
    QLabel* mHistogramView;

private:// fps chart stuff
    QLineSeries* mFpsLineSeries = new QLineSeries();
    QChart* mFpsChart = new QChart();
    QChartView* mFpsChartView;
    uint mFpsTickCount = 0;
    const int mFpsPointCount = 200;
    std::shared_ptr<BorderProvider> getBorderProvider();
    std::shared_ptr<AmbiRgbLineProvider> getRgbLineProvider();
    bool mStopClicked = false;
};

#endif // CONTROLDIALOG_H
