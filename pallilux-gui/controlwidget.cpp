#include "controlwidget.h"
#include "ui_controlwidget.h"

#include <QtConcurrent>
#include <QDoubleSpinBox>
#include <QFutureWatcher>
#include <QProgressDialog>
#include <QSettings>
#include <QTime>
#include <QFormLayout>

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
    onStatusUpdate(string("beginning connection attempt"), false);

    try {
        // try to connect, showing a busy dialog
        QProgressDialog dialog("connecting", "cancel", 0, 0, this);

        dialog.setCancelButton(nullptr);

        QFutureWatcher<void> watcher;
        connect(&watcher, SIGNAL(finished()), &dialog, SLOT(cancel()));

        QFuture<void> future = QtConcurrent::run(mArduinoConnector.get(), &ArduinoConnector::connect);
        watcher.setFuture(future);

        dialog.exec();

        // wait for finished to collect exception
        future.waitForFinished();

        // ui update
        onStatusUpdate(string("connection ok"), false);
    } catch(ArduinoConnectorException e) {
        // ui update
        onStatusUpdate(string("catastrophic failure: ") + e.what(), true);
        emit onStateChanged(false);
        return;
    } catch(SerialException e) {
        // ui update
        onStatusUpdate(string("serial failure: ") + e.what(), true);
        emit onStateChanged(false);
        return;
    } catch (QtConcurrent::UnhandledException&){
        onStatusUpdate(string("unhandled exception: check your connection"), true);
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
            onStatusUpdate("running for " + elapsed.toString("hh:mm:ss").toStdString(), false);

            // update custom widgets
            updateWidgets();

            // process ui events
            qApp->processEvents();
        } catch(ArduinoConnectorException e) {
            // ui update
            onStatusUpdate(string("catastrophic failure: ") + e.what(), true);
            emit onStateChanged(false);
            break;
        } catch (ScreenshotException e) {
            onStatusUpdate(string("catastrophic failure: ") + e.what(), true);
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
        onStatusUpdate(string("ambilight shut down"), false);
    } catch (ArduinoConnectorException e) {
        onStatusUpdate(string("disconnect failed: ") + e.what(), true);
    }
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

void ControlWidget::onStatusUpdate(const string &text, bool failure) {
    emit onStatusUpdate(QString::fromStdString(text), failure);
}
