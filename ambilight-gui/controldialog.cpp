#include "controldialog.h"
#include "ui_controldialog.h"

#include <QDateTime>

#include "assert.h"
#include <string>

#include <QValueAxis>
#include <ctime>
#include <lowpassfilter.h>
#include <brightnessfilter.h>

using namespace std;

ControlDialog::ControlDialog(shared_ptr<ArduinoConnector> connector, QWidget *parent, const QString& infoString) :
    QDialog(parent),
    ui(new Ui::ControlDialog),
    mArduinoConnector(connector)
{
    ui->setupUi(this);

    // disable the stop button
    ui->stopButton->setEnabled(false);

    // set info string
    ui->infoState->setText(infoString);

    // update border width
    ui->borderWidthSpinbox->setValue(getBorderProvider()->getBorderWidth());

    // set up fps axes
    QValueAxis* xAxis = new QValueAxis();
    xAxis->setMax(mFpsPointCount);
    xAxis->setVisible(false);
    QValueAxis* yAxis = new QValueAxis();
    yAxis->setMax(100);

    // set up fps chart
    mFpsChart->legend()->hide();
    mFpsChart->addSeries(mFpsLineSeries);
    mFpsChart->setAxisX(xAxis, mFpsLineSeries);
    mFpsChart->setAxisY(yAxis, mFpsLineSeries);
    mFpsChart->setTitle("fps");

    mFpsChartView = new QChartView(mFpsChart);
    mFpsChartView->setRenderHint(QPainter::Antialiasing);

    // add fps chart to window
    ui->stateLayout->layout()->addWidget(mFpsChartView);
}

ControlDialog::~ControlDialog()
{
    delete ui;
}

void ControlDialog::on_runButton_clicked()
{
    // disable the stop button
    setButtonState(true);

    // check for null
    assert(mArduinoConnector);

    // ui update
    updateStatus("beginning connection attempt");

    try {
        // try to connect
        mArduinoConnector->connect();
        // ui update
        updateStatus("");
    } catch(AmbiConnectorException e){
        // ui update
        updateStatus(string("catastrophic failure: ") + e.what(), true);
    }

    // start timing
    QTime startTime;
    startTime.start();

    // begin sending data
    while(1){
        try {
            // update leds
            mArduinoConnector->update();
            mArduinoConnector->draw();

            // update fps chart
            mFpsLineSeries->append(mFpsTickCount++, mArduinoConnector->getCurrentFps());
            while(mFpsLineSeries->count() > mFpsPointCount){
                qreal xDelta = mFpsChart->mapToPosition(mFpsLineSeries->at(1)).rx() - mFpsChart->mapToPosition(mFpsLineSeries->at(0)).rx();
                mFpsLineSeries->removePoints(0, 1);
                mFpsChart->scroll(xDelta, 0);
            }
            mFpsChartView->repaint();

            // update "running for" label
            int sec = startTime.elapsed() / 1000;
            QString s = QString::number(sec % 60);
            QString m = QString::number((sec / 60) % 60);
            QString h = QString::number(sec / 60 / 60);
            ui->runningforState->setText(h + ":" + m + ":" + s);

            // process ui events
            qApp->processEvents();
        } catch(AmbiConnectorException e){
            // ui update
            updateStatus(string("catastrophic failure: ") + e.what(), true);
            break;
        }
    }
}

void ControlDialog::on_stopButton_clicked()
{
    // disable the run button
    setButtonState(false);

    // stop the arduino lighting
    mArduinoConnector->disconnect(true);
}

void ControlDialog::setButtonState(bool currentlyRunning)
{
    ui->runButton->setEnabled(!currentlyRunning);
    ui->stopButton->setEnabled(currentlyRunning);
}

void ControlDialog::updateStatus(const string&msg, bool isFailure)
{
    ui->stateState->setText(QString(msg.c_str()));
    if(isFailure)
        ui->stateState->setStyleSheet("QLabel { color : red; }");
    else
        ui->stateState->setStyleSheet("QLabel { color : black; }");
}

void ControlDialog::on_newdataFactorSpinbox_valueChanged(double arg1)
{
    // im sorry
    LowPassFilter* filter = dynamic_cast<LowPassFilter*>(mArduinoConnector->getFilter("lowpass").get());
    // check that cast & finding worked
    assert(filter);
    // apply change
    filter->setNewDataFactor((float) arg1);
}

void ControlDialog::on_brightnessFactorSpinbox_valueChanged(double arg1)
{
    // sorry for this, too
    BrightnessFilter* filter = dynamic_cast<BrightnessFilter*>(mArduinoConnector->getFilter("brightness").get());
    // check that cast & finding worked
    assert(filter);
    // apply change
    filter->setFactor((float) arg1);
}

shared_ptr<SingleScreenBorderProvider> ControlDialog::getBorderProvider(){
    // maybe the interface is a bit shitty. just maybe.
    shared_ptr<AmbiRgbLineProvider> lineProvider = dynamic_pointer_cast<AmbiRgbLineProvider>(mArduinoConnector->getRgbLineProvider());
    return dynamic_pointer_cast<SingleScreenBorderProvider>(lineProvider->getBorderProvider());
}

void ControlDialog::on_borderWidthSpinbox_valueChanged(int arg1)
{
    getBorderProvider()->setBorderWidth(arg1);
}
