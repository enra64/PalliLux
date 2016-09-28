#ifndef PIXELLINEWIDGET_H
#define PIXELLINEWIDGET_H

#include <QCheckBox>
#include <QLabel>
#include <QWidget>


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
    Magick::Blob mBlob;///< data blob used to temporarily store the image data
    QPixmap mLinePixmap;///< pixmap used to load the data into a Qt format
};

#endif // PIXELLINEWIDGET_H
