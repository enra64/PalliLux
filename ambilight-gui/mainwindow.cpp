#include "ui_mainwindow.h"
#include "mainwindow.h"

#include <QMessageBox>

#include <memory>

#include "iconfigpage.h"
#include "ledconfigdialog.h"
#include "serialconfigdialog.h"

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

    // disable stop button
    setRunState(false);

    // get user to set led and serial config
    checkConfiguration();

    // connect kill buttons to the stop function
    connect(ui->stopControlDialogButton, &QPushButton::clicked, this, &MainWindow::stop);
}

MainWindow::~MainWindow() {
    delete ui;
}

IScreenConfigPage* MainWindow::getCurrentTab() {
    return dynamic_cast<IScreenConfigPage*>(ui->configTabHost->currentWidget());
}

void MainWindow::checkConfiguration() {
    // the program cannot function if any of these conditions is not met
    bool problem =
        !LedConfigDialog::isLedCountSet() ||
        !SerialConfigDialog::isSerialDeviceSet() ||
        !SerialConfigDialog::deviceExists(SerialConfigDialog::getSerialDevice());
    ui->startControlDialogButton->setEnabled(!problem);


    if(!LedConfigDialog::isLedCountSet())
        addWarningButton("Please enter your led configuration", mLedButton, SLOT(on_actionLED_Configuration_triggered()));
    else
        removeWarningButton(mLedButton, SLOT(on_actionLED_Configuration_triggered()));

    if(!SerialConfigDialog::isSerialDeviceSet())
        addWarningButton("Please enter your arduino serial connection", mSerialButton, SLOT(on_actionSerial_Configuration_triggered()));
    else if(!SerialConfigDialog::deviceExists(SerialConfigDialog::getSerialDevice()))
        addWarningButton("The entered serial device no longer exists", mSerialButton, SLOT(on_actionSerial_Configuration_triggered()));
    else
        removeWarningButton(mSerialButton, SLOT(on_actionSerial_Configuration_triggered()));
}

void MainWindow::addWarningButton(const QString& text, QPushButton*& button, const char* slot) {
    // abort if the button already exists
    if(button != nullptr)
        return;

    button = new QPushButton(text, this);
    connect(button, SIGNAL(clicked(bool)), this, slot);
    ui->buttonLayout->addWidget(button);
}

void MainWindow::removeWarningButton(QPushButton *&button, const char *slot) {
    disconnect(button, SIGNAL(clicked(bool)), this, slot);
    ui->buttonLayout->removeWidget(button);
    delete button;

    // reset variable
    button = nullptr;
}

void MainWindow::setRunState(bool running) {
    // disable switching to another tab
    for(int tab = 0; tab < ui->configTabHost->count(); tab++)
        if(tab != ui->configTabHost->currentIndex())
            ui->configTabHost->setTabEnabled(tab, !running);

    // start/stop buttons
    ui->startControlDialogButton->setEnabled(!running);
    ui->stopControlDialogButton->setEnabled(running);
}

void MainWindow::on_startControlDialogButton_clicked() {
    // disable controls which must not be accessed at ledruntime
    setRunState(true);

    // start widget
    getCurrentTab()->getControlWidget().start(SerialConfigDialog::getSerialDevice());

    // re-enable controls now that the connector went offline
    setRunState(false);
}

void MainWindow::on_actionLED_Configuration_triggered() {
    LedConfigDialog d(this);
    d.exec();
    checkConfiguration();
}

void MainWindow::on_actionSerial_Configuration_triggered() {
    SerialConfigDialog d(this);
    d.exec();
    checkConfiguration();
}

void MainWindow::stop() {
    getCurrentTab()->getControlWidget().stop();
}


void MainWindow::closeEvent(QCloseEvent *) {
    stop();
}
