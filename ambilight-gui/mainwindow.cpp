#include "ui_mainwindow.h"
#include "mainwindow.h"

#include <QMessageBox>

#include <memory>

#include "iconfigpage.h"
#include "ledconfigdialog.h"

#include <ambicolordataprovider.h>
#include <colordataprovider.h>
#include <screenshotprovider.h>

#include <singlescreenborderprovider.h>
#include <triplescreenborderprovider.h>


#ifdef __linux__
#include <xlibscreenshotprovider.h>
#include <linuxserial.h>
#elif _WIN32_WINNT
#include <winscreenshotprovider.h>
#include <windowsserial.h>
#else
#error Platform not recognized
#endif

#include <lowpassfilter.h>
#include <brightnessfilter.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // check whether the default tty device exists
    on_ttyState_textChanged(ui->ttyState->text());

    // connect to tabhost change signal
    connect(ui->configTabHost, &QTabWidget::currentChanged, this, &MainWindow::updateTabLedCount);

    // init first tab
    updateTabLedCount();

    // disable stop button
    setRunState(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::refreshLedCount() {
    const int ledCount =
        ui->bLedSpin->value() +
        ui->rLedSpin->value() +
        ui->tLedSpin->value() +
        ui->lLedSpin->value();
    ui->ledCountLabel->setText(QString::number(ledCount));

    updateTabLedCount();
}

IScreenConfigPage* MainWindow::getCurrentTab() {
    return dynamic_cast<IScreenConfigPage*>(ui->configTabHost->currentWidget());
}

void MainWindow::showNoSerialWarning() {
    QMessageBox::warning(this, "Serial device error", QString("The serial device %1 could not be found!").arg(ui->ttyState->text()), QMessageBox::Ok);
}

bool MainWindow::enteredSerialOk() {
    // check whether the tty device exists
#ifdef __linux__
    LinuxSerial serial;
#elif _WIN32_WINNT
    WindowsSerial serial;
#else
#error Platform not recognized
#endif

    return serial.deviceExists(ui->ttyState->text().toStdString());
}

void MainWindow::setRunState(bool running) {
    // control tab state
    for(int tab = 0; tab < ui->configTabHost->count(); tab++)
        if(tab != ui->configTabHost->currentIndex())
            ui->configTabHost->setTabEnabled(tab, !running);

    // start/stop buttons
    ui->startControlDialogButton->setEnabled(!running);
    ui->stopControlDialogButton->setEnabled(running);

    // tty device
    ui->ttyState->setEnabled(!running);

    // led count
    ui->bLedSpin->setEnabled(!running);
    ui->rLedSpin->setEnabled(!running);
    ui->tLedSpin->setEnabled(!running);
    ui->lLedSpin->setEnabled(!running);
}

void MainWindow::on_startControlDialogButton_clicked() {
    // if the serial device does not exist, the user should not be able to start the control dialog
    if(!enteredSerialOk()) {
        showNoSerialWarning();
        return;
    }

    // disable controls which must not be accessed at ledruntime
    setRunState(true);

    // connect stop button to this widget
    connect(ui->stopControlDialogButton, &QPushButton::clicked, &getCurrentTab()->getWidget(), &ControlWidget::stop);

    // start widget
    getCurrentTab()->getWidget().start(ui->ttyState->text());

    // re-enable controls now that the connector went offline
    setRunState(false);

    // disconnect the stop button after running
    disconnect(ui->stopControlDialogButton, &QPushButton::clicked, &getCurrentTab()->getWidget(), &ControlWidget::stop);
}

void MainWindow::updateTabLedCount() {
    getCurrentTab()->updateLedCount(LedCount(ui->bLedSpin->value(), ui->rLedSpin->value(), ui->tLedSpin->value(), ui->lLedSpin->value()));
}

void MainWindow::on_ttyState_textChanged(const QString &) {
    if(enteredSerialOk())
        ui->ttyState->setStyleSheet("color: black;");
    else
        ui->ttyState->setStyleSheet("color: red;");
}

void MainWindow::on_actionLED_Configuration_triggered() {
    LedConfigDialog d(this);
    d.exec();
}
