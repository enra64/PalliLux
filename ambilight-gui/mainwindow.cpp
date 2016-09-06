#include "ui_mainwindow.h"
#include "mainwindow.h"

#include "controldialog.h"
#include "iconfigpage.h"

#include <memory>

#include <ambirgblineprovider.h>
#include <rgblineprovider.h>
#include <screenshot.h>

#include <singlescreenborderprovider.h>
#include <triplescreenborderprovider.h>

#ifdef __linux__
    #include <xlibscreenshot.h>
    #include <linuxserial.h>
#elif _WIN32_WINNT
    #include <winscreenshot.h>
    #include <windowsserial.h>
#else
    #error Platform not recognized
#endif

#include <lowpassfilter.h>
#include <brightnessfilter.h>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // check whether the default tty device exists
    on_ttyState_textChanged(ui->ttyState->text());

    // initialise our ambiconnector so we can later set the rgb provider
    mAmbiConnector = shared_ptr<ArduinoConnector>(new ArduinoConnector());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::refreshLedCount() {
    int ledCount = ui->xLedSpin->text().toInt() * 2 + ui->yLedSpin->text().toInt() * 2;
    ui->ledCountLabel->setText(QString::number(ledCount));
}

const IScreenConfigPage* MainWindow::getCurrentPage() {
    return dynamic_cast<IScreenConfigPage*>(ui->configStack->currentWidget());
}

void MainWindow::showNoSerialWarning()
{
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

void MainWindow::on_startControlDialogButton_clicked() {
    // if the serial device does not exist, the user should not be able to start the control dialog
    if(!enteredSerialOk()){
        showNoSerialWarning();
        return;
    }

    // get the currently displayed configuration page to retrieve a rgbProvider
    const IScreenConfigPage* currentPage = getCurrentPage();

    // ambilight rgb provider
    shared_ptr<RgbLineProvider> rgbProvider = currentPage->rgbProvider(ui->xLedSpin->value(), ui->yLedSpin->value());

    // parametrize the ArduinoConnector
    mAmbiConnector->setRgbLineProvider(rgbProvider);
    mAmbiConnector->setPort(ui->ttyState->text().toStdString());

    // add low pass filter
    unique_ptr<DataFilter> lpFilter(new LowPassFilter(mAmbiConnector->getRequiredBufferLength(), .6f));
    mAmbiConnector->addFilter("lowpass", std::move(lpFilter));

    // add brightness filter
    unique_ptr<DataFilter> brFilter(new BrightnessFilter(1.f));
    mAmbiConnector->addFilter("brightness", std::move(brFilter));

    ControlDialog c(mAmbiConnector, this, currentPage->infoText());
    c.exec();
}

void MainWindow::on_yLedSpin_valueChanged(int) {
    refreshLedCount();
}

void MainWindow::on_xLedSpin_valueChanged(int) {
    refreshLedCount();
}

void MainWindow::on_configStackPrevButton_clicked() {
    // get current index
    int index = ui->configStack->currentIndex() - 1;

    // clamp to positive indices
    if(index < 0) index += ui->configStack->count();

    // update page
    ui->configStack->setCurrentIndex(index);

    // update label
    ui->configStackLabel->setText(getCurrentPage()->pageLabel());
}

void MainWindow::on_configStackNextButton_clicked() {
    // get current index
    int nextIndex = ui->configStack->currentIndex() + 1;

    // clamp to valid values
    nextIndex %= ui->configStack->count();

    // update oage
    ui->configStack->setCurrentIndex(nextIndex);

    // update label
    ui->configStackLabel->setText(getCurrentPage()->pageLabel());
}

void MainWindow::on_ttyState_textChanged(const QString &) {
    if(enteredSerialOk())
        ui->ttyState->setStyleSheet("color: black;");
    else
        ui->ttyState->setStyleSheet("color: red;");
}
