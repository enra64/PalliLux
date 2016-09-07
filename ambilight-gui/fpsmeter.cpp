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
    mFpsChart->setAxisX(xAxis, mFpsLineSeries);
    mFpsChart->setAxisY(yAxis, mFpsLineSeries);

    // set up chart view
    mFpsChartView = new QChartView(mFpsChart);
    mFpsChartView->setRenderHint(QPainter::Antialiasing);

    // fill line series with "valid" data that can be replaced during the first run
    for(int i = 0; i < CONCURRENT_FPS_VALUES; i++)
        mFpsLineSeries->append(i, 0);
}

void FpsMeter::update(float fpsValue) {
    // avoid calculations if possible
    if(!mEnable)
        return;

    // wrap around at the end of the chart
    int currentIndex = mFpsTickCount++ % CONCURRENT_FPS_VALUES;

    // replace the oldest point with the new fps value
    mFpsLineSeries->replace(currentIndex, currentIndex, fpsValue);

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
