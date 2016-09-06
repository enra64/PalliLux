#include "controldialog.h"
#include "ui_controldialog.h"

#include <QDateTime>

#include "assert.h"
#include <string>

#include <ctime>
#include <lowpassfilter.h>
#include <brightnessfilter.h>
#include <customexceptions.h>

#include <magick++.h>

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

    // set up last line view
    mLastLineView = new QLabel(this);

    // set up histogram
    mHistogramView = new QLabel(this);
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
    } catch(AmbiConnectorException e) {
        // ui update
        updateStatus(string("catastrophic failure: ") + e.what(), true);
        setButtonState(false);
        return;
    } catch(SerialException e){
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

            // update fps widget
            ui->fpsWidget->update(mArduinoConnector->getCurrentFps());

            // update state label with runtime information
            QTime elapsed = QTime(0, 0).addMSecs(startTime.elapsed());
            updateStatus("running for " + elapsed.toString("hh:mm:ss").toStdString());

            // get last border line
            std::unique_ptr<Magick::Image> lastLine = getRgbLineProvider()->getLastLineImage();

            // show last border line
            if(mEnableLastLineView) {
                Magick::Blob bob;
                lastLine->write(&bob, "PNG");
                QPixmap line;
                line.loadFromData((uchar*)(bob.data()), static_cast<uint>(bob.length()));
                mLastLineView->setPixmap(line.scaled(mLastLineView->width(), mLastLineView->height()));
            }

            // update histogram chart
            if(mEnableHistogram) {
                // temporarily save our line picture, must be crossplatformed
                //todo:crossplatformify;
                lastLine->write("histogram:/tmp/line.png");
                QPixmap histogram("/tmp/line.png");
                mHistogramView->setMinimumSize(histogram.width(), histogram.height());
                mHistogramView->setPixmap(histogram);
            }

            // process ui events
            qApp->processEvents();
        } catch(AmbiConnectorException e) {
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
    }
    catch (AmbiConnectorException e){
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

shared_ptr<AmbiRgbLineProvider> ControlDialog::getRgbLineProvider() {
    return dynamic_pointer_cast<AmbiRgbLineProvider>(mArduinoConnector->getRgbLineProvider());
}

shared_ptr<BorderProvider> ControlDialog::getBorderProvider() {
    return dynamic_pointer_cast<BorderProvider>(getRgbLineProvider()->getBorderProvider());
}

void ControlDialog::on_borderWidthSpinbox_valueChanged(int arg1) {
    getBorderProvider()->setBorderWidth(arg1);
}

void ControlDialog::on_histogramCheckbox_clicked(bool checked) {
    mEnableHistogram = checked;
    mHistogramView->setVisible(checked);
    if(checked){
        ui->histogramLayout->addWidget(mHistogramView);
    }
    else{
        ui->histogramLayout->removeWidget(mHistogramView);
    }
}

void ControlDialog::on_lineCheckbox_clicked(bool checked){
    mEnableLastLineView = checked;
    mLastLineView->setVisible(checked);
    if(checked){
        ui->lineLayout->addWidget(mLastLineView);
    }
    else{
        ui->lineLayout->removeWidget(mLastLineView);
    }
}
