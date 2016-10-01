#include "histogramwidget.h"
#include "ui_histogramwidget.h"

#include <QDir>

HistogramWidget::HistogramWidget(QWidget *parent) : QWidget(parent) {
    // add a root layout for our widgets
    setLayout(new QVBoxLayout(this));

    // create & add checkbox for enabling the widget
    mEnableCheckBox = new QCheckBox("Show histogram", this);
    layout()->addWidget(mEnableCheckBox);

    // remove free space around checkbox
    layout()->setContentsMargins(0,0,0,0);

    // react to checkbox toggles
    connect(mEnableCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggled(bool)));

    // get our temporary file location (the histogram will be stored here)
    mTempLocation = QDir::tempPath() + "/histogram.ppm";

    // init display label
    mHistogramView = new QLabel(this);

    // add label-image to layout
    layout()->addWidget(mHistogramView);

    // center histogram, as horizontal scaling looks ugly here
    mHistogramView->setAlignment(Qt::AlignHCenter);

    // conform visibility to initial checkbox state
    mHistogramView->setVisible(mEnableCheckBox->isChecked());
}

void HistogramWidget::update(Image *lineImg) {
    if(!mEnableCheckBox->isChecked()) return;

    // save a histogram to the temp location
    //lineImg->get_2D_histogram(NUMBER_OF_BINS).save(mTempLocation.toStdString().c_str());

    // read from temp save to QPixmap
    mHistogram = QPixmap(mTempLocation);

    // display the QPixmap
    mHistogramView->setPixmap(mHistogram);
}

void HistogramWidget::toggled(bool checked) {
    // empty display if display is disabled
    mHistogramView->setVisible(checked);
}
