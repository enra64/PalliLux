#ifndef BACKLIGHTCONFIGPAGE_H
#define BACKLIGHTCONFIGPAGE_H

#include "iconfigpage.h"

namespace Ui {
class BacklightConfigPage;
}

class BacklightConfigPage : public QWidget, public IScreenConfigPage
{
    Q_OBJECT

public:
    explicit BacklightConfigPage(QWidget *parent = 0);
    ~BacklightConfigPage();

private:
    Ui::BacklightConfigPage *ui;

    // IScreenConfigPage interface
    QString infoText() const override;
public:
    QString pageLabel() const override;
    ControlWidget* getWidget(QWidget *parent, LedCount d) const override;
};

#endif // BACKLIGHTCONFIGPAGE_H
