#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QLabel>
#include <QWidget>

#include <Magick++.h>

namespace Ui {
class HistogramWidget;
}

class HistogramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistogramWidget(QWidget *parent = 0);
    ~HistogramWidget();

    void update(Magick::Image *lineImg);

private slots:
    void on_fpsMeterCheckbox_clicked(bool checked);

private:
    Ui::HistogramWidget *ui;

private:
    bool mEnable;///< true if the histogram should be shown
    QString mHistogramLocation;///< the location of the temporary histogram file
    QPixmap mHistogram;///< a pixmap of our histogram
    QLabel* mHistogramView;///< the label used to display our histogram

};

#endif // HISTOGRAMWIDGET_H
