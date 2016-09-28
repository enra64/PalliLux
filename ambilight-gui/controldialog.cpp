#include "controldialog.h"
#include "ui_controldialog.h"

#include <QDateTime>
#include <QDir>

#include "assert.h"
#include <string>

#include <ctime>
#include <lowpassfilter.h>
#include <brightnessfilter.h>
#include <customexceptions.h>

#include "histogramwidget.h"
#include "pixellinewidget.h"
#include "fpsmeter.h"

#include <Magick++.h>

using namespace std;

ControlDialog::ControlDialog(shared_ptr<ArduinoConnector> connector, QWidget *parent, const QString& infoString) :
    QDialog(parent), ui(new Ui::ControlDialog), mArduinoConnector(connector) {
    //setup ui
    ui->setupUi(this);

    // disable the stop button (we havent even started yet)
    ui->stopButton->setEnabled(false);

    // set info string
    ui->infoState->setText(infoString);

    // update border width
    ui->borderWidthSpinbox->setValue(static_cast<int>(getBorderProvider()->getBorderWidth()));

    // set up fps widget
    mFpsMeter = new FpsMeter(this);
    ui->controlDialogMiscLayout->addWidget(mFpsMeter);

    // set up last line widget
    mLastLineWidget = new PixelLineWidget(this);
    ui->controlDialogMiscLayout->addWidget(mLastLineWidget);

    // set up histogram
    mHistogramWidget = new HistogramWidget(this);
    ui->controlDialogMiscLayout->addWidget(mHistogramWidget);
}

ControlDialog::~ControlDialog() {
    delete ui;
}

void ControlDialog::on_runButton_clicked() {
    // disable the stop button
    setButtonState(true);
    mStopClicked = false;

    // check for null
    assert(mArduinoConnector);

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
        setButtonState(false);
        return;
    } catch(SerialException e) {
        // ui update
        updateStatus(string("serial failure: ") + e.what(), true);
        setButtonState(false);
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

            // get last border line
            std::unique_ptr<Magick::Image> lastLine = getColorDataProvider()->getLastLineImage();

            // update fps widget
            mFpsMeter->update(mArduinoConnector->getCurrentFps());

            // update lastLine widget
            mLastLineWidget->update(lastLine.get());

            // update histogram chart
            mHistogramWidget->update(lastLine.get());

            // process ui events
            qApp->processEvents();
        } catch(ArduinoConnectorException e) {
            // ui update
            updateStatus(string("catastrophic failure: ") + e.what(), true);
            setButtonState(false);
            break;
        }
    }
}

void ControlDialog::on_stopButton_clicked() {
    // disable the run button
    setButtonState(false);
    mStopClicked = true;

    // stop the arduino lighting
    try {
        mArduinoConnector->disconnect(true);
        updateStatus("ambilight shut down");
    } catch (ArduinoConnectorException e) {
        updateStatus(string("disconnect failed: ") + e.what(), true);
    }
}

void ControlDialog::setButtonState(bool currentlyRunning) {
    ui->runButton->setEnabled(!currentlyRunning);
    ui->stopButton->setEnabled(currentlyRunning);
}

void ControlDialog::updateStatus(const string&msg, bool isFailure) {
    ui->stateState->setText(QString(msg.c_str()));
    if(isFailure)
        ui->stateState->setStyleSheet("QLabel { color : red; }");
    else
        ui->stateState->setStyleSheet("QLabel { color : black; }");
}

void ControlDialog::on_newdataFactorSpinbox_valueChanged(double arg1) {
    // im sorry
    LowPassFilter* filter = dynamic_cast<LowPassFilter*>(mArduinoConnector->getFilter("lowpass").get());
    // check that cast & finding worked
    assert(filter);
    // apply change
    filter->setNewDataFactor((float) arg1);
}

void ControlDialog::on_brightnessFactorSpinbox_valueChanged(double arg1) {
    // sorry for this, too
    BrightnessFilter* filter = dynamic_cast<BrightnessFilter*>(mArduinoConnector->getFilter("brightness").get());
    // check that cast & finding worked
    assert(filter);
    // apply change
    filter->setFactor((float) arg1);
}

shared_ptr<AmbiColorDataProvider> ControlDialog::getColorDataProvider() {
    return dynamic_pointer_cast<AmbiColorDataProvider>(mArduinoConnector->getColorDataProvider());
}

shared_ptr<BorderProvider> ControlDialog::getBorderProvider() {
    return dynamic_pointer_cast<BorderProvider>(getColorDataProvider()->getBorderProvider());
}

void ControlDialog::on_borderWidthSpinbox_valueChanged(int arg1) {
    getBorderProvider()->setBorderWidth(arg1);
}


void ControlDialog::closeEvent(QCloseEvent *) {
    on_stopButton_clicked();
}
