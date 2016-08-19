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

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_2_valueChanged(int){
    refreshLedCount();
}

void MainWindow::on_spinBox_valueChanged(int){
    refreshLedCount();
}

void MainWindow::refreshLedCount()
{
    int ledCount = ui->xLedSpin->text().toInt() * 2 + ui->yLedSpin->text().toInt() * 2;
    ui->ledCountLabel->setText(QString::number(ledCount));
}

std::unique_ptr<RgbLineProvider> createAmbilightRgbProvider(){
    // instantiate the desired screenshot class
    shared_ptr<Screenshot> screener = shared_ptr<Screenshot>(new XlibScreenshot());

    // instantiate the desired borderProvider with the screener. it will use the Screenshot instance
    // to get screenshots from the system
    shared_ptr<BorderProvider> borderProvider = shared_ptr<BorderProvider>(
                new SingleScreenBorderProvider(1920, 1080, screener, 1024, 0));
    //shared_ptr<BorderProvider> borderProvider = shared_ptr<BorderProvider>(new TripleScreenBorderProvider(screener));

    // instantiate and return an AmbiRgbLineProvider, the RGB data source. It will use the
    // BorderProvider to get images of the borders and convert them to RGB arrays
    return move(unique_ptr<RgbLineProvider>(new AmbiRgbLineProvider(borderProvider, 60, 18)));
}

void MainWindow::on_pushButton_clicked()
{
    // ambilight rgb provider
    unique_ptr<RgbLineProvider> rgbProvider = createAmbilightRgbProvider();

    // supply our AmbiConnector with its chosen RgbLineProvider
    shared_ptr<ArduinoConnector> connector = shared_ptr<ArduinoConnector>(new ArduinoConnector(std::move(rgbProvider), "/dev/ttyUSB0"));
    ControlDialog c(connector, this, "center screen only");
    c.exec();
}
