#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_pushButton_clicked()
{
    if(mAmbiConnector.get() != nullptr){

    }
    else{

    }
}
