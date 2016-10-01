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
#include <ambicolordataprovider.h>

#include "pixellinewidget.h"
#include "histogramwidget.h"
#include "fpsmeter.h"

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

    // set up last line widget
    mLastLineWidget = new PixelLineWidget(this);
    ui->controlDialogMiscLayout->addWidget(mLastLineWidget);

    // set up fps widget
    mFpsMeter = new FpsMeter(this);
    ui->controlDialogMiscLayout->addWidget(mFpsMeter);

    // set up histogram
    mHistogramWidget = new HistogramWidget(this);
    ui->controlDialogMiscLayout->addWidget(mHistogramWidget);

    // nudge the dialog to stay at the smallest possible size
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    // set up combobox for selecting the interpolation type
    setupInterpolationCombobox();

    // connect to the finished() signal to gracefully stop the event loop
    connect(this, SIGNAL(finished(int)), this, SLOT(on_stopButton_clicked()));
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
            std::unique_ptr<Image> lastLine = getColorDataProvider()->getLastLineImage();

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
        } catch (ScreenshotException e) {
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

void ControlDialog::onInterpolationTypeChanged(int index) {
    getColorDataProvider()->setResizeInterpolationMode(static_cast<AmbiColorDataProvider::CImgInterpolationType>(index - 1));
}

//getColorDataProvider()->setResizeInterpolationMode(static_cast<AmbiColorDataProvider::CImgInterpolationType>(index - 1));
void ControlDialog::setupInterpolationCombobox() {
    // make a list of available interpolation types
    QStringList interpolationTypes;
    interpolationTypes <<
                       "Raw memory resizing" <<
                       "Fill-space-interpolation" <<
                       "Nearest Neighbor interpolation" <<
                       "Moving average interpolation" <<
                       "Linear interpolation" <<
                       "Grid interpolation" <<
                       "Cubic interpolation" <<
                       "Lanczos interpolation";

    // insert interpolation types into selection box
    ui->interpolationTypeComboBox->addItems(interpolationTypes);

    // get the currently set interpolation mode
    AmbiColorDataProvider::CImgInterpolationType i = getColorDataProvider()->getResizeInterpolationMode();

    // enum range is -1 to 6, but our index here starts at 0 -> +1
    ui->interpolationTypeComboBox->setCurrentIndex(
                // convert enum to integer, offset by the difference in starting indices
                static_cast<typename std::underlying_type<AmbiColorDataProvider::CImgInterpolationType>::type>(i) + 1);

    // connect to currentIndexChanged after having it triggered programmatically
    connect(ui->interpolationTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInterpolationTypeChanged(int)));
}
