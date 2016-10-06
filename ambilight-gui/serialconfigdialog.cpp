#include "serialconfigdialog.h"
#include "ui_serialconfigdialog.h"

#include <QSettings>

#ifdef __linux__
#include <xlibscreenshotprovider.h>
#include <linuxserial.h>
#elif _WIN32_WINNT
#include <winscreenshotprovider.h>
#include <windowsserial.h>
#else
#error Platform not recognized
#endif

SerialConfigDialog::SerialConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialConfigDialog) {
    ui->setupUi(this);

    // connect to the tty edit change signal
    connect(ui->ttyNameEdit, SIGNAL(textChanged(QString)), this, SLOT(onSerialDeviceTextChanged(QString)));

    ui->ttyNameEdit->setText(getSerialDevice());
}

SerialConfigDialog::~SerialConfigDialog() {
    delete ui;
}

QString SerialConfigDialog::getSerialDevice() {
    QSettings s;
    return s.value("serial/device").toString();
}

bool SerialConfigDialog::isSerialDeviceSet() {
    return getSerialDevice() != "";
}

void SerialConfigDialog::accept() {
    QSettings s;
    s.setValue("serial/device", ui->ttyNameEdit->text());
    QDialog::accept();
}

void SerialConfigDialog::onSerialDeviceTextChanged(const QString &) {
    if(deviceExists(ui->ttyNameEdit->text()))
        ui->ttyNameEdit->setStyleSheet("color: black;");
    else
        ui->ttyNameEdit->setStyleSheet("color: red;");
}

bool SerialConfigDialog::deviceExists(const QString& dev) {
    // check whether the tty device exists
#ifdef __linux__
    LinuxSerial serial;
#elif _WIN32_WINNT
    WindowsSerial serial;
#else
#error Platform not recognized
#endif

    return serial.deviceExists(dev.toStdString());
}
