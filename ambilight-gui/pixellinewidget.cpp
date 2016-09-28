#include "pixellinewidget.h"
#include "ui_pixellinewidget.h"

#include <QDir>

using namespace Magick;
using namespace std;

PixelLineWidget::PixelLineWidget(QWidget *parent) : QWidget(parent) {
    // add a root layout for our widgets
    setLayout(new QVBoxLayout(this));

    // create & add checkbox for enabling the widget
    mEnableCheckBox = new QCheckBox("Show last pixel line", this);
    //layout()->addWidget(new QCheckBox(this));
    layout()->addWidget(mEnableCheckBox);

    connect(mEnableCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggled(bool)));

    // create label
    mPixelLine = new QLabel(this);

    // add pixel line label-image to layout
    layout()->addWidget(mPixelLine);

    // conform visibility to initial checkbox state
    mPixelLine->setVisible(mEnableCheckBox->isChecked());
}

void PixelLineWidget::update(Image *pixelLine) {
    // only calculate if necessary
    if(!mEnableCheckBox->isChecked())
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

void PixelLineWidget::toggled(bool checked)
{
    // en/disable display
    mPixelLine->setVisible(checked);
}
