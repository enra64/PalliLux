#include "pixellinewidget.h"
#include "ui_pixellinewidget.h"

#include <QDir>

using namespace std;

PixelLineWidget::PixelLineWidget(QWidget *parent) : QWidget(parent) {
    // add a root layout for our widgets
    setLayout(new QVBoxLayout(this));

    // create & add checkbox for enabling the widget
    mEnableCheckBox = new QCheckBox("Show last pixel line", this);

    layout()->addWidget(mEnableCheckBox);

    connect(mEnableCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggled(bool)));

    // create label
    mPixelLine = new QLabel(this);

    // add pixel line label-image to layout
    layout()->addWidget(mPixelLine);

    // conform visibility to initial checkbox state
    mPixelLine->setVisible(mEnableCheckBox->isChecked());

    // get our temporary file location (the histogram will be stored here)
    mTempLocation = QDir::tempPath() + "/line.ppm";
}

void PixelLineWidget::update(Image *pixelLine) {
    // only calculate if necessary
    if(!mEnableCheckBox->isChecked())
        return;

    // save the image to a file
    pixelLine->save(mTempLocation.toStdString().c_str());

    // load file into QPixmap
    mLinePixmap = QPixmap(mTempLocation);

    // load QPixmap into the QLabel used to display the image
    mPixelLine->setPixmap(mLinePixmap.scaled(mPixelLine->width(), mPixelLine->height()));
}

void PixelLineWidget::toggled(bool checked)
{
    // en/disable display
    mPixelLine->setVisible(checked);
}
