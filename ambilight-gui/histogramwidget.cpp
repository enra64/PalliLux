#include "histogramwidget.h"
#include "ui_histogramwidget.h"

#include <QDir>

HistogramWidget::HistogramWidget(QWidget *parent) : QWidget(parent) {
    // add a root layout for our widgets
    setLayout(new QVBoxLayout(this));

    // create & add checkbox for enabling the widget
    mEnableCheckBox = new QCheckBox("Show histogram", this);
    layout()->addWidget(mEnableCheckBox);

    // react to checkbox toggles
    connect(mEnableCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggled(bool)));

    // get our temporary file location (the histogram will be stored here)
    mHistogramLocation = QDir::tempPath() + "/line.png";

    // init display label
    mHistogramView = new QLabel(this);

    // add label-image to layout
    layout()->addWidget(mHistogramView);

    // center histogram, as horizontal scaling looks ugly here
    mHistogramView->setAlignment(Qt::AlignHCenter);

    // conform visibility to initial checkbox state
    mHistogramView->setVisible(mEnableCheckBox->isChecked());
}

void HistogramWidget::update(Magick::Image *lineImg) {
    if(!mEnableCheckBox->isChecked()) return;

    // temporarily save our line picture
    lineImg->write("histogram:" + mHistogramLocation.toStdString());

    // read from temp save to QPixmap
    mHistogram = QPixmap(mHistogramLocation);

    // display the QPixmap
    mHistogramView->setPixmap(mHistogram);
}

void HistogramWidget::toggled(bool checked) {
    // empty display if display is disabled
    mHistogramView->setVisible(checked);
}
