#ifndef LEDCONFIGDIALOG_H
#define LEDCONFIGDIALOG_H

#include <QDialog>
#include <colordataprovider.h>

namespace Ui {
class LedConfigDialog;
}

class LedConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LedConfigDialog(QWidget *parent = 0);
    ~LedConfigDialog();
    static LedConfig getLedConfig();
    static bool isLedCountSet();
private:
    Ui::LedConfigDialog *ui;

    // QDialog interface
    void refreshLedCount();
public slots:
    void accept() override;
};

#endif // LEDCONFIGDIALOG_H
