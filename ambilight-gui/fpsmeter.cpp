#include <QLabel>

#include "fpsmeter.h"

FpsMeter::FpsMeter(QWidget *parent) : QWidget(parent) {
    // add a root layout for our widgets
    setLayout(new QVBoxLayout(this));

    // create & add checkbox for enabling the widget
    mEnableCheckBox = new QCheckBox("Show FPS graph", this);
    layout()->addWidget(mEnableCheckBox);

    // remove free space around checkbox
    layout()->setContentsMargins(0,0,0,0);

    connect(mEnableCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggled(bool)));

    // set up chart
    setupChart();
}

FpsMeter::~FpsMeter() {
    delete mFpsChartView;
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

    // add the chart widget to the layout
    layout()->addWidget(mFpsChartView);

    // and finally, conform visibility to initial checkbox state
    mFpsChartView->setVisible(mEnableCheckBox->isChecked());
}

float FpsMeter::getHistoryAverage() {
    float sum = 0;
    for(int i = 0; i < CONCURRENT_FPS_VALUES; i++)
        sum += mFpsHistory[i];
    return sum / CONCURRENT_FPS_VALUES;
}

void FpsMeter::update(float fpsValue) {
    // avoid calculations if possible
    if(!mEnableCheckBox->isChecked())
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

    // set the title to the fps average
    mFpsChart->setTitle(QString("Current average: %1fps").arg((int) fpsAvg));

    // refresh chart
    mFpsChartView->repaint();
}

void FpsMeter::toggled(bool checked){
    // en/disable display
    mFpsChartView->setVisible(checked);
}
#else
void FpsMeter::setupChart() {
    mFpsLabel = new QLabel(this);
    mFpsLabel->setText("-");
}

// if we do not have the QtCharts, we dont need to do anything here
void FpsMeter::update(float fpsValue) {
    mFpsLabel->setText(QString::number(fpsValue));
}

void FpsMeter::on_fpsMeterCheckbox_clicked(bool checked) {
    mFpsLabel->setVisible(checked);
}
#endif //QT_CHARTS_FOUND
