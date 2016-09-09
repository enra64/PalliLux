#include "backlightdialog.h"
#include "ui_backlightdialog.h"

#include <QColorDialog>
#include <QDateTime>
#include <constantcolordataprovider.h>
#include <customexceptions.h>
#include <pulsingfilter.h>

using namespace std;

BacklightDialog::BacklightDialog(int horizontalBorderLedCount, int verticalBorderLedCount, std::string port, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BacklightDialog) {
    ui->setupUi(this);

    // disable the stop button (we havent even started yet)
    ui->stopButton->setEnabled(false);

    // load slider values
    mFrequency = ui->backlightControlFrequencySlider->value();

    // create arduinoconnector
    mColorProvider = shared_ptr<ConstantColorDataProvider>(new ConstantColorDataProvider(horizontalBorderLedCount, verticalBorderLedCount));

    mColorProvider->setRgb(
        ui->backlightControlRedSlider->value(),
        ui->backlightControlGreenSlider->value(),
        ui->backlightControlBlueSlider->value());

    // create connector
    mArduinoConnector = shared_ptr<ArduinoConnector>(new ArduinoConnector(port));

    // set color provider
    mArduinoConnector->setColorDataProvider(mColorProvider);

    // add pulsing filter
    unique_ptr<DataFilter> pulsingFilter(new PulsingFilter(100, 0));
    mArduinoConnector->addFilter("pulse", std::move(pulsingFilter));
}

BacklightDialog::~BacklightDialog() {
    delete ui;
}

void BacklightDialog::on_backlightControlRedSlider_sliderMoved(int position) {
    mColorProvider->setRed(ui->backlightControlRedSlider->value());
}

void BacklightDialog::on_backlightControlGreenSlider_sliderMoved(int position) {
    mColorProvider->setGreen(position);
}

void BacklightDialog::on_backlightControlBlueSlider_sliderMoved(int position) {
    mColorProvider->setBlue(position);
}

void BacklightDialog::on_backlightControlFrequencySlider_sliderMoved(int position) {
    PulsingFilter* filter = dynamic_cast<PulsingFilter*>(mArduinoConnector->getFilter("pulse").get());
    filter->setCycleLength(position);
}

void BacklightDialog::on_runButton_clicked() {
    // disable the stop button
    setButtonState(true);
    mStopClicked = false;

    // ui update
    //updateStatus("beginning connection attempt");

    try {
        // try to connect
        mArduinoConnector->connect();
        // ui update
        //updateStatus("connection ok");
    } catch(ArduinoConnectorException e) {
        // ui update
        //updateStatus(string("catastrophic failure: ") + e.what(), true);
        setButtonState(false);
        return;
    } catch(SerialException e) {
        // ui update
        //updateStatus(string("serial failure: ") + e.what(), true);
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

            // process ui events
            qApp->processEvents();
        } catch(ArduinoConnectorException e) {
            // ui update
            //updateStatus(string("catastrophic failure: ") + e.what(), true);
            setButtonState(false);
            break;
        }
    }
}

void BacklightDialog::on_stopButton_clicked() {
    // disable the run button
    setButtonState(false);
    mStopClicked = true;

    // stop the arduino lighting
    try {
        mArduinoConnector->disconnect(true);
        //updateStatus("ambilight shut down");
    } catch (ArduinoConnectorException e) {
        //updateStatus(string("disconnect failed: ") + e.what(), true);
    }
}

void BacklightDialog::setButtonState(bool currentlyRunning) {
    ui->runButton->setEnabled(!currentlyRunning);
    ui->stopButton->setEnabled(currentlyRunning);
}

void BacklightDialog::on_backlightControlColorPickerButton_clicked() {
    // recreate current color to set start value for color picker
    QColor pastColor(ui->backlightControlRedSlider->value(),
                     ui->backlightControlGreenSlider->value(),
                     ui->backlightControlBlueSlider->value());

    // get color picker
    QColor result = QColorDialog::getColor(pastColor, this);

    // notify color provider of change
    mColorProvider->setRgb(
        result.red(),
        result.green(),
        result.blue());

    // notify sliders
    ui->backlightControlRedSlider->setValue(result.red());
    ui->backlightControlGreenSlider->setValue(result.green());
    ui->backlightControlBlueSlider->setValue(result.blue());
}

void BacklightDialog::on_backlightControlAmplitudeSpinbox_valueChanged(double arg1) {
    PulsingFilter* filter = dynamic_cast<PulsingFilter*>(mArduinoConnector->getFilter("pulse").get());
    filter->setMaximumAmplitude(arg1);
}
