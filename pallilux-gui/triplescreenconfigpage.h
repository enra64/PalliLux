#ifndef TRIPLESCREENCONFIGPAGE_H
#define TRIPLESCREENCONFIGPAGE_H

#include <QWidget>

#include "iconfigpage.h"

namespace Ui {
class TripleScreenConfigPage;
}

class TripleScreenConfigPage : public QWidget, public IScreenConfigPage
{
    Q_OBJECT

public:
    explicit TripleScreenConfigPage(QWidget *parent = 0);
    ~TripleScreenConfigPage();

private:
    Ui::TripleScreenConfigPage *ui;

    // IScreenConfigPage interface
    ControlWidget* getControlWidget(QWidget* parent) override;
};



#endif // TRIPLESCREENCONFIGPAGE_H
