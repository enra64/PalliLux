#ifndef SERIALCONFIGDIALOG_H
#define SERIALCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class SerialConfigDialog;
}

class SerialConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialConfigDialog(QWidget *parent = 0);
    ~SerialConfigDialog();

    static QString getSerialDevice();
    static bool isSerialDeviceSet();
    static bool deviceExists(const QString &dev);

private slots:
    void onSerialDeviceTextChanged(const QString &);

private:
    Ui::SerialConfigDialog *ui;

    // QDialog interface
public slots:
    void accept() override;
};

#endif // SERIALCONFIGDIALOG_H
