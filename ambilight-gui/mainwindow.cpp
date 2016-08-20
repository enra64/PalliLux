#include "controldialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <memory>

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
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::refreshLedCount() {
    int ledCount = ui->xLedSpin->text().toInt() * 2 + ui->yLedSpin->text().toInt() * 2;
    ui->ledCountLabel->setText(QString::number(ledCount));
}

shared_ptr<BorderProvider> MainWindow::getSingleScreenBorderProvider(shared_ptr<Screenshot> screener) {
    int w = ui->resolutionWidthSpinbox->value();
    int h = ui->resolutionHeightSpinbox->value();
    int xOff = ui->xOffsetSpinbox->value();
    int yOff = ui->yOffsetSpinbox->value();
    int xLetterbox = ui->letterboxingWidthSpinBox->value();
    int yLetterbox = ui->letterboxingHeightSpinBox->value();

    return shared_ptr<BorderProvider>(new SingleScreenBorderProvider(w, h, screener, xOff, yOff, xLetterbox, yLetterbox));
}

std::unique_ptr<RgbLineProvider> MainWindow::createAmbilightRgbProvider() {
    // instantiate the desired screenshot class
    shared_ptr<Screenshot> screener = shared_ptr<Screenshot>(new XlibScreenshot());

    // instantiate the desired borderProvider with the screener. it will use the Screenshot instance
    // to get screenshots from the system
    shared_ptr<BorderProvider> borderProvider = getSingleScreenBorderProvider(screener);

    // instantiate and return an AmbiRgbLineProvider, the RGB data source. It will use the
    // BorderProvider to get images of the borders and convert them to RGB arrays
    return std::move(unique_ptr<RgbLineProvider>(new AmbiRgbLineProvider(borderProvider, 60, 18)));
}

QString MainWindow::getInfoText()
{
    QString w = QString::number(ui->resolutionWidthSpinbox->value());
    QString h = QString::number(ui->resolutionHeightSpinbox->value());
    QString xOff = QString::number(ui->xOffsetSpinbox->value());
    QString yOff = QString::number(ui->yOffsetSpinbox->value());
    QString xLetterbox = QString::number(ui->letterboxingWidthSpinBox->value());
    QString yLetterbox = QString::number(ui->letterboxingHeightSpinBox->value());

    return QString("Single screen, %1x%2+%3+%4, letterboxing %5x%6").arg(w, h, xOff, yOff, xLetterbox, yLetterbox);
}

void MainWindow::on_pushButton_clicked() {
    // ambilight rgb provider
    unique_ptr<RgbLineProvider> rgbProvider = createAmbilightRgbProvider();

    // supply our AmbiConnector with its chosen RgbLineProvider
    shared_ptr<ArduinoConnector> connector = shared_ptr<ArduinoConnector>(new ArduinoConnector(std::move(rgbProvider), "/dev/ttyUSB0"));

    // add low pass filter
    unique_ptr<DataFilter> lpFilter(new LowPassFilter(connector->getRequiredBufferLength(), .6f));
    connector->addFilter("lowpass", std::move(lpFilter));

    // add brightness filter
    unique_ptr<DataFilter> brFilter(new BrightnessFilter(1.f));
    connector->addFilter("brightness", std::move(brFilter));

    ControlDialog c(connector, this, getInfoText());
    c.exec();
}

void MainWindow::on_yLedSpin_valueChanged(int) {
    refreshLedCount();
}

void MainWindow::on_xLedSpin_valueChanged(int) {
    refreshLedCount();
}
