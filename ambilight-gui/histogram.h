#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QChartView>

// using namespace qtcharts
QT_CHARTS_USE_NAMESPACE

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/image.h>

class Histogram : public QChart {
public:
    Histogram();
    void update(const Magick::Image& data, int pixelCount);
private:
    QBarSet* mRedSet = new QBarSet("red");
    QBarSet* mGreenSet = new QBarSet("green");
    QBarSet* mBlueSet = new QBarSet("blue");
    QBarSeries mSeries;

    const int BIN_COUNT = 16;
};

#endif // HISTOGRAM_H
