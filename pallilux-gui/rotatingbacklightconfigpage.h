#ifndef ROTATINGBACKLIGHTCONFIGPAGE_H
#define ROTATINGBACKLIGHTCONFIGPAGE_H

#include "iconfigpage.h"

#include <QWidget>

namespace Ui {
class RotatingBacklightConfigPage;
}

class RotatingBacklightConfigPage : public QWidget, public IScreenConfigPage
{
    Q_OBJECT

public:
    explicit RotatingBacklightConfigPage(QWidget *parent = 0);
    ~RotatingBacklightConfigPage();

private:
    Ui::RotatingBacklightConfigPage *ui;

    // IScreenConfigPage interface
protected:
    ControlWidget *getControlWidget(QWidget *parent);
};

#endif // ROTATINGBACKLIGHTCONFIGPAGE_H
