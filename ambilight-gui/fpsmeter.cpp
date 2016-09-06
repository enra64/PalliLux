#include "fpsmeter.h"
#include "ui_fpsmeter.h"

#include <QLabel>

FpsMeter::FpsMeter(QWidget *parent) : QWidget(parent), ui(new Ui::FpsMeter){
    // set up ui
    ui->setupUi(this);

    // set up chart
    setupChart();
}

FpsMeter::~FpsMeter(){
    delete ui;
}

#ifdef QT_CHARTS_FOUND
void FpsMeter::setupChart()
{
    // set up axes
    QValueAxis* xAxis = new QValueAxis();
    xAxis->setMax(mFpsPointCount);
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

}

void FpsMeter::update(float fpsValue){
    // avoid calculations if possible
    if(!mEnable)
        return;

    // add new data point to series
    mFpsLineSeries->append(mFpsTickCount++, fpsValue);

    todo:maybe_we_could_just_set_the_oldest_value_instead_of_scrolling;
    // remove data points until we reach our limit
    while(mFpsLineSeries->count() > CONCURRENT_FPS_VALUES) {
        // calculate how much we will need to scroll to still show all points
        qreal xDelta = mFpsChart->mapToPosition(
                           mFpsLineSeries->at(1)).rx() - mFpsChart->mapToPosition(mFpsLineSeries->at(0)).rx();

        // remove oldest point
        mFpsLineSeries->removePoints(0, 1);

        // scroll to show all points
        mFpsChart->scroll(xDelta, 0);
    }

    // refresh chart
    mFpsChartView->repaint();
}

void FpsMeter::on_fpsMeterCheckbox_clicked(bool checked){
    mEnable = checked;

    // en/disable display
    if(mEnable)
        ui->fpsMeterLayout->addWidget(mFpsChartView);
    else
        ui->fpsMeterLayout->removeWidget(mFpsChartView);
}
#else
void FpsMeter::setupChart(){
    ui->fpsMeterCheckbox->setEnabled(false);
    mFpsLabel = new QLabel(this);
    mFpsLabel->setText("-");
}

// if we do not have the QtCharts, we dont need to do anything here
void FpsMeter::update(float fpsValue){
    mFpsLabel->setText(QString::number(fpsValue));
}

void FpsMeter::on_fpsMeterCheckbox_clicked(bool checked){
    mEnable = checked;

    // en/disable display
    if(mEnable)
        ui->fpsMeterLayout->addWidget(mFpsLabel);
    else
        ui->fpsMeterLayout->removeWidget(mFpsLabel);
}
#endif //QT_CHARTS_FOUND
