#include "ui_mainwindow.h"
#include "mainwindow.h"

#include <QMessageBox>

#include <memory>

#include "iconfigpage.h"

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

void MainWindow::loadControlWidget()
{
    // get the currently displayed configuration page to retrieve a rgbProvider
    const IScreenConfigPage* currentPage = getCurrentPage();

    QGridLayout* widgetContainer = static_cast<QGridLayout*>(ui->controlWidgetContainer);

    // kill old widget
    if(mCurrentControlWidget){
        // remove from layout
        widgetContainer->removeWidget(mCurrentControlWidget);

        // disconnect signal
        disconnect(this, &MainWindow::destroyed, mCurrentControlWidget, &ControlWidget::stop);

        // delete object
        delete mCurrentControlWidget;
    }

    // get new ControlWidget
    mCurrentControlWidget = currentPage->getWidget(parentWidget(), LedCount(ui->xLedSpin->value(), ui->yLedSpin->value()));

    // window destroyed -> stop connection
    connect(this, &MainWindow::destroyed, mCurrentControlWidget, &ControlWidget::stop);

    //display widget
    widgetContainer->addWidget(mCurrentControlWidget);
}

void MainWindow::on_startControlDialogButton_clicked() {
    // if the serial device does not exist, the user should not be able to start the control dialog
    if(!enteredSerialOk()){
        showNoSerialWarning();
        return;
    }

    mCurrentControlWidget->start(ui->ttyState->text());
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

    // update currently shown control widget
    loadControlWidget();
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

    // update currently shown control widget
    loadControlWidget();
}

void MainWindow::on_ttyState_textChanged(const QString &) {
    if(enteredSerialOk())
        ui->ttyState->setStyleSheet("color: black;");
    else
        ui->ttyState->setStyleSheet("color: red;");
}
