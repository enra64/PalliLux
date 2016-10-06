#include "controlwidget.h"
#include "ui_controlwidget.h"

#include <QDoubleSpinBox>
#include <QSettings>
#include <QTime>

using namespace std;

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget) {
    // setup ui
    ui->setupUi(this);

    // not running yet
    emit onStateChanged(false);

    // create default arduino connector
    mArduinoConnector = shared_ptr<ArduinoConnector>(new ArduinoConnector());
}

ControlWidget::~ControlWidget() {
    delete ui;
}

void ControlWidget::start(const QString& port) {
    // disable the stop button
    emit onStateChanged(true);
    mStopClicked = false;

    // check for null
    assert(mArduinoConnector);

    // set port
    mArduinoConnector->setPort(port.toStdString());

    // ui update
    updateStatus("beginning connection attempt");

    try {
        // try to connect
        mArduinoConnector->connect();
        // ui update
        updateStatus("connection ok");
    } catch(ArduinoConnectorException e) {
        // ui update
        updateStatus(string("catastrophic failure: ") + e.what(), true);
        emit onStateChanged(false);
        return;
    } catch(SerialException e) {
        // ui update
        updateStatus(string("serial failure: ") + e.what(), true);
        emit onStateChanged(false);
        return;
    }

    // start timing
    QTime startTime;
    startTime.start();

    // begin sending data
    while(!mStopClicked) {
        try {
            // update leds
            mArduinoConnector->update();
            mArduinoConnector->draw();

            // update state label with runtime information
            QTime elapsed = QTime(0, 0).addMSecs(startTime.elapsed());
            updateStatus("running for " + elapsed.toString("hh:mm:ss").toStdString());

            // update custom widgets
            updateWidgets();

            // process ui events
            qApp->processEvents();
        } catch(ArduinoConnectorException e) {
            // ui update
            updateStatus(string("catastrophic failure: ") + e.what(), true);
            emit onStateChanged(false);
            break;
        } catch (ScreenshotException e) {
            updateStatus(string("catastrophic failure: ") + e.what(), true);
            emit onStateChanged(false);
            break;
        }
    }
}

void ControlWidget::stop() {
    // disable the run button
    emit onStateChanged(false);
    mStopClicked = true;

    // stop the arduino lighting
    try {
        mArduinoConnector->disconnect(true);
        updateStatus("ambilight shut down");
    } catch (ArduinoConnectorException e) {
        updateStatus(string("disconnect failed: ") + e.what(), true);
    }
}

void ControlWidget::addStatusWidget(QWidget *widget) {
    ui->stateLayout->addWidget(widget);
}

void ControlWidget::addControlWidget(QWidget* row) {
    addControlWidget(row, nullptr);
}

void ControlWidget::addControlWidget(QWidget *left, QWidget* right) {
    // initialise layout if none is there yet
    if(ui->controlGroupBox->layout() == nullptr)
        ui->controlGroupBox->setLayout(new QFormLayout());

    QFormLayout* controlBox = static_cast<QFormLayout*>(ui->controlGroupBox->layout());

    // add widgets
    if(right != nullptr)
        controlBox->addRow(left, right);
    else
        controlBox->addRow(left);
}

void ControlWidget::addMiscWidget(QWidget *widget) {
    // initialise layout if none is there yet
    if(ui->miscGroupBOx->layout() == nullptr)
        ui->miscGroupBOx->setLayout(new QVBoxLayout(parentWidget()));

    // add widget
    ui->miscGroupBOx->layout()->addWidget(widget);
}

void ControlWidget::updateStatus(const std::__cxx11::string &msg, bool isFailure) {
    ui->stateState->setText(QString(msg.c_str()));
    if(isFailure)
        ui->stateState->setStyleSheet("QLabel { color : red; }");
    else
        ui->stateState->setStyleSheet("QLabel { color : black; }");
}
