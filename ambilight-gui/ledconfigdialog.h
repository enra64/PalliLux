#ifndef LEDCONFIGDIALOG_H
#define LEDCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class LedConfigDialog;
}

class LedConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LedConfigDialog(QWidget *parent = 0);
    ~LedConfigDialog();

private:
    Ui::LedConfigDialog *ui;

    // QDialog interface
    void refreshLedCount();
public slots:
    void accept() override;
    void reject() override;
};

#endif // LEDCONFIGDIALOG_H
