#include "histogramwidget.h"
#include "ui_histogramwidget.h"

#include <QDir>

HistogramWidget::HistogramWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogramWidget) {
    ui->setupUi(this);

    // sync the enable state of checkbox and variable
    mEnable = ui->histogramCheckbox->isChecked();

    // get our temporary file location (the histogram will be stored here)
    mHistogramLocation = QDir::tempPath() + "/line.png";

    // init display label
    mHistogramView = new QLabel(this);
}

HistogramWidget::~HistogramWidget() {
    delete ui;
}

void HistogramWidget::update(Magick::Image *lineImg)
{
    // temporarily save our line picture
    lineImg->write("histogram:" + mHistogramLocation.toStdString());

    // read from temp save to QPixmap
    mHistogram = QPixmap(mHistogramLocation);

    // display the QPixmap
    mHistogramView->setMinimumSize(mHistogram.width(), mHistogram.height());
    mHistogramView->setPixmap(mHistogram);
}

void HistogramWidget::on_fpsMeterCheckbox_clicked(bool checked) {
    mEnable = checked;

    // en/disable display
    if(mEnable)
        ui->histogramLayout->addWidget(mHistogramView);
    else
        ui->histogramLayout->removeWidget(mHistogramView);
}
