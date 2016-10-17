#ifndef SPECTROMETERCONFIGPAGE_H
#define SPECTROMETERCONFIGPAGE_H

#include "iconfigpage.h"

#include <QWidget>

namespace Ui {
class SpectrometerConfigPage;
}

class SpectrometerConfigPage : public QWidget, public IScreenConfigPage
{
    Q_OBJECT

public:
    explicit SpectrometerConfigPage(QWidget *parent = 0);
    ~SpectrometerConfigPage();

private:
    Ui::SpectrometerConfigPage *ui;

    // IScreenConfigPage interface
protected:
    ControlWidget *getControlWidget(QWidget *);
};

#endif // SPECTROMETERCONFIGPAGE_H
