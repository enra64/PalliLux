#ifndef BACKLIGHTCONFIGPAGE_H
#define BACKLIGHTCONFIGPAGE_H

#include "iconfigpage.h"

namespace Ui {
class BacklightConfigPage;
}

class BacklightConfigPage : public QWidget, public ICustomDialogConfigPage
{
    Q_OBJECT

public:
    explicit BacklightConfigPage(QWidget *parent = 0);
    ~BacklightConfigPage();

private:
    Ui::BacklightConfigPage *ui;

    // ICustomDialogConfigPage interface
public:
    QString pageLabel() const override;
    QString infoText() const override;
    QDialog *getDialog(QWidget *parent, int horizontalBorderLedCount, int verticalBorderLedCount, std::string port) const override;
};

#endif // BACKLIGHTCONFIGPAGE_H
