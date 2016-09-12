#include "pixellinewidget.h"
#include "ui_pixellinewidget.h"

#include <QDir>

using namespace Magick;
using namespace std;

PixelLineWidget::PixelLineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PixelLineWidget) {
    ui->setupUi(this);

    // create label
    mPixelLine = new QLabel(this);
}

PixelLineWidget::~PixelLineWidget() {
    delete ui;
}

void PixelLineWidget::update(Image *pixelLine) {
    // only calculate if necessary
    if(!mEnable)
        return;

    // save the image into a blob
    pixelLine->write(&mBlob, "PNG");

    // load blob data into QPixmap
    mLinePixmap.loadFromData(
        static_cast<const uint8_t*>(mBlob.data()),
        static_cast<uint>(mBlob.length()));

    // load QPixmap into the QLabel used to display the image
    mPixelLine->setPixmap(mLinePixmap.scaled(mPixelLine->width(), mPixelLine->height()));
}

void PixelLineWidget::on_fpsMeterCheckbox_clicked(bool checked) {
    mEnable = checked;

    // en/disable display
    if(mEnable)
        ui->pixelLineWidgetLayout->addWidget(mPixelLine);
    else
        ui->pixelLineWidgetLayout->removeWidget(mPixelLine);
}
