#include "controldialog.h"
#include "iconfigpage.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <memory>
#include <unistd.h>

#include <ambirgblineprovider.h>
#include <rgblineprovider.h>
#include <screenshot.h>

#include <singlescreenborderprovider.h>
#include <triplescreenborderprovider.h>
#include <xlibscreenshot.h>

#include <lowpassfilter.h>
#include <brightnessfilter.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // check whether the default tty device exists
    on_ttyState_textChanged(ui->ttyState->text());
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

void MainWindow::on_startControlDialogButton_clicked() {
    const IScreenConfigPage* currentPage = getCurrentPage();

    // ambilight rgb provider
    shared_ptr<RgbLineProvider> rgbProvider = currentPage->rgbProvider(ui->xLedSpin->value(), ui->yLedSpin->value());

    // supply our AmbiConnector with its chosen RgbLineProvider
    shared_ptr<ArduinoConnector> connector = shared_ptr<ArduinoConnector>(new ArduinoConnector(rgbProvider, ui->ttyState->text().toStdString()));

    // add low pass filter
    unique_ptr<DataFilter> lpFilter(new LowPassFilter(connector->getRequiredBufferLength(), .6f));
    connector->addFilter("lowpass", std::move(lpFilter));

    // add brightness filter
    unique_ptr<DataFilter> brFilter(new BrightnessFilter(1.f));
    connector->addFilter("brightness", std::move(brFilter));

    ControlDialog c(connector, this, currentPage->infoText());
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

void MainWindow::on_ttyState_textChanged(const QString &ttyDevice) {
    // check whether the tty device exists
    if(access(ttyDevice.toStdString().c_str(), F_OK) < 0)
        ui->ttyState->setStyleSheet("color : red;");
    else
        ui->ttyState->setStyleSheet("color : black;");
}
