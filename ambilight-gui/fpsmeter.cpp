#include <QLabel>

#include "fpsmeter.h"
#include "ui_fpsmeter.h"


FpsMeter::FpsMeter(QWidget *parent) : QWidget(parent), ui(new Ui::FpsMeter) {
    // set up ui
    ui->setupUi(this);

    // set up chart
    setupChart();
}

FpsMeter::~FpsMeter() {
    delete mFpsChartView;
    delete ui;
}

#ifdef QT_CHARTS_FOUND
void FpsMeter::setupChart() {
    // set up axes
    QValueAxis* xAxis = new QValueAxis();
    xAxis->setMax(CONCURRENT_FPS_VALUES);
    xAxis->setVisible(false);
    QValueAxis* yAxis = new QValueAxis();
    yAxis->setMax(100);

    // set up chart
    mFpsChart->legend()->hide();
    mFpsChart->addSeries(mFpsLineSeries);
    mFpsChart->addSeries(mAverageFpsLineSeries);

    mFpsChart->setAxisX(xAxis, mFpsLineSeries);
    mFpsChart->setAxisY(yAxis, mFpsLineSeries);
    mFpsChart->setAxisX(xAxis, mAverageFpsLineSeries);
    mFpsChart->setAxisY(yAxis, mAverageFpsLineSeries);

    // set up chart view
    mFpsChartView = new QChartView(mFpsChart);
    mFpsChartView->setRenderHint(QPainter::Antialiasing);

    // zero out history and line series to create "valid" starting data
    for(int i = 0; i < CONCURRENT_FPS_VALUES; i++) {
        mFpsHistory[i] = 0;
        mFpsLineSeries->append(i, 0);
    }

    // create two points in the average fps line series to draw a straight line
    mAverageFpsLineSeries->append(0, 0);
    mAverageFpsLineSeries->append(CONCURRENT_FPS_VALUES, 0);
}

float FpsMeter::getHistoryAverage() {
    float sum = 0;
    for(int i = 0; i < CONCURRENT_FPS_VALUES; i++)
        sum += mFpsHistory[i];
    return sum / CONCURRENT_FPS_VALUES;
}

void FpsMeter::update(float fpsValue) {
    // avoid calculations if possible
    if(!mEnable)
        return;

    // wrap around at the end of the chart
    int currentIndex = mFpsTickCount++ % CONCURRENT_FPS_VALUES;

    // replace the oldest point with the new fps value
    mFpsLineSeries->replace(currentIndex, currentIndex, fpsValue);

    // update the history
    mFpsHistory[currentIndex] = fpsValue;

    // move the average line to the current average
    float fpsAvg = getHistoryAverage();
    mAverageFpsLineSeries->replace(0, 0, fpsAvg);
    mAverageFpsLineSeries->replace(1, CONCURRENT_FPS_VALUES, fpsAvg);

    // refresh chart
    mFpsChartView->repaint();
}

void FpsMeter::on_fpsMeterCheckbox_clicked(bool checked) {
    mEnable = checked;

    // en/disable display
    if(mEnable)
        ui->fpsMeterLayout->addWidget(mFpsChartView);
    else
        ui->fpsMeterLayout->removeWidget(mFpsChartView);
}
#else
void FpsMeter::setupChart() {
    ui->fpsMeterCheckbox->setEnabled(false);
    mFpsLabel = new QLabel(this);
    mFpsLabel->setText("-");
}

// if we do not have the QtCharts, we dont need to do anything here
void FpsMeter::update(float fpsValue) {
    mFpsLabel->setText(QString::number(fpsValue));
}

void FpsMeter::on_fpsMeterCheckbox_clicked(bool checked) {
    mEnable = checked;

    // en/disable display
    if(mEnable)
        ui->fpsMeterLayout->addWidget(mFpsLabel);
    else
        ui->fpsMeterLayout->removeWidget(mFpsLabel);
}
#endif //QT_CHARTS_FOUND
