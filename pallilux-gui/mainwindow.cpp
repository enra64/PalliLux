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
#include <ddapiscreenshotprovider.h>
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

    // set window icon
    this->setWindowIcon(QIcon(":images/icon.png"));

    // disable stop button
    setRunState(false);

    // get user to set led and serial config
    checkConfiguration();

    // add a qlabel to the menubar for displaying status
    mStatusLabel = new QLabel("not connected", this);
    ui->menuBar->setCornerWidget(mStatusLabel);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setRunState(bool running) {
    // disable switching to another tab
    for(int tab = 0; tab < ui->configTabHost->count(); tab++)
        if(tab != ui->configTabHost->currentIndex())
            ui->configTabHost->setTabEnabled(tab, !running);

    // start/stop buttons
    ui->startStopButton->setText(running ? mStopText : mStartText);

    if(running) {
        // connect start/stop button to the stop function
        connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::stop);
        // disconnect from the start function
        disconnect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::start);
    } else {
        // connect start/stop button to the stop function
        connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::start);
        // disconnect from the start function
        disconnect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::stop);
    }

    // if pallilux stopped working, check if the connection possibly went bad
    if(!running)
        checkConfiguration();
}

void MainWindow::start() {
    ControlWidget& w = getCurrentTab()->getControlWidget();

    // connect the widgets start/stop signal
    connect(&w, &ControlWidget::onStateChanged, this, &MainWindow::setRunState);

    // connect to the widgets status update
    connect(&w, SIGNAL(onStatusUpdate(QString,bool)), this, SLOT(updateStatus(QString,bool)));

    // start widget
    getCurrentTab()->getControlWidget().start(SerialConfigDialog::getSerialDevice());
}

void MainWindow::stop() {
    // if pallilux is not running, calling stop() blocks indefinitely
    if(ui->startStopButton->text() == mStopText)
        getCurrentTab()->getControlWidget().stop();
}

void MainWindow::updateStatus(const QString &text, bool failure) {
    mStatusLabel->setText(text);
    mStatusLabel->setStyleSheet(failure ? "color: red;" : "color: black;");
    ui->menuBar->adjustSize();
    if(failure)
        checkConfiguration();
}

IScreenConfigPage *MainWindow::getCurrentTab() {
    return dynamic_cast<IScreenConfigPage*>(ui->configTabHost->currentWidget());
}

void MainWindow::closeEvent(QCloseEvent *) {
    stop();
}

void MainWindow::checkConfiguration() {
    // the program cannot function if any of these conditions is not met
    bool problem =
        !LedConfigDialog::isLedCountSet() ||
        !SerialConfigDialog::isSerialDeviceSet() ||
        !SerialConfigDialog::deviceExists(SerialConfigDialog::getSerialDevice());
    ui->startStopButton->setEnabled(!problem);


    if(!LedConfigDialog::isLedCountSet())
        addWarningButton("Please enter your led configuration", mLedButton, SLOT(on_actionLED_Configuration_triggered()));
    else
        removeWarningButton(mLedButton, SLOT(on_actionLED_Configuration_triggered()));

    if(!SerialConfigDialog::isSerialDeviceSet())
        addWarningButton("Please enter your arduino serial connection", mSerialButton, SLOT(on_actionSerial_Configuration_triggered()));
    else if(!SerialConfigDialog::deviceExists(SerialConfigDialog::getSerialDevice()))
        addWarningButton(SerialConfigDialog::getSerialDevice() + " no longer exists", mSerialButton, SLOT(on_actionSerial_Configuration_triggered()));
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
