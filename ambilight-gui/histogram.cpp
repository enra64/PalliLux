#include "histogram.h"

#include <QValueAxis>

Histogram::Histogram() {
    setTitle("current histogram");

    for(int i = 0; i < BIN_COUNT; i++){
        mRedSet->append(0);
        mGreenSet->append(0);
        mBlueSet->append(0);
    }

    QBarSeries* mSeries = new QBarSeries();
    mSeries->append(mRedSet);
    mSeries->append(mGreenSet);
    mSeries->append(mBlueSet);
    addSeries(mSeries);

    // set up axes
    QValueAxis* yAxis = new QValueAxis();
    yAxis->setMax(10000);
    yAxis->setMin(0);


    // set up fps chart
    legend()->hide();
    createDefaultAxes();
    setAxisY(yAxis, mSeries);
}

void Histogram::update(const Magick::Image &data, int pixelCount) {
    uint32_t histValues[3][BIN_COUNT];
    memset(histValues, 0, 3*BIN_COUNT);

    for(int i = 0; i < pixelCount; i++) {
        // retrieve rgb data from the line
        Magick::ColorRGB pixel = data.pixelColor(i, 0);

        histValues[0][BIN_COUNT * (int) pixel.red()]++;
        histValues[1][BIN_COUNT * (int) pixel.green()]++;
        histValues[2][BIN_COUNT * (int) pixel.blue()]++;
    }

    for(int i = 0; i < BIN_COUNT; i++){
        mRedSet->replace(i, histValues[0][i]);
        mGreenSet->replace(i, histValues[1][i]);
        mBlueSet->replace(i, histValues[2][i]);
    }
}

