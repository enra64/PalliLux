#ifndef PIXELLINEWIDGET_H
#define PIXELLINEWIDGET_H

#include <QCheckBox>
#include <QLabel>
#include <QWidget>

#include "include_img_lib.h"

class PixelLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PixelLineWidget(QWidget *parent = 0);

public:// interaction methods
    void update(Image *pixelLine);

private slots:
    void toggled(bool checked);

private:
    QLabel* mPixelLine = nullptr;///< label used to display line of pixels
    QCheckBox* mEnableCheckBox;///< checkbox used to dis/enable widget display
    QPixmap mLinePixmap;///< pixmap used to load the data into a Qt format
    QString mTempLocation;
};

#endif // PIXELLINEWIDGET_H
