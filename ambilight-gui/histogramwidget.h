#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QCheckBox>
#include <QLabel>
#include <QWidget>

#include <Magick++.h>

class HistogramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistogramWidget(QWidget *parent = 0);

    void update(Magick::Image *lineImg);

private slots:
    void toggled(bool checked);

private:
    QCheckBox* mEnableCheckBox;///< checkbox used to dis/enable widget display
    QString mHistogramLocation;///< the location of the temporary histogram file
    QPixmap mHistogram;///< a pixmap of our histogram
    QLabel* mHistogramView = nullptr;///< the label used to display our histogram

};

#endif // HISTOGRAMWIDGET_H
