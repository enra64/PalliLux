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

    // set up fps axes
    QValueAxis* xAxis = new QValueAxis();
    xAxis->setMax(mFpsPointCount);
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
    updateProgressbar(ProgressState::working, 0, 1);

    try {
        // try to connect
        mArduinoConnector->connect();
        // ui update
        updateStatus("");
        updateProgressbar(ProgressState::working, 1, 1);
    } catch(AmbiConnectorException e){
        // ui update
        updateStatus(string("catastrophic failure: ") + e.what());
        updateProgressbar(ProgressState::failure);
    }

    // start timing
    QTime startTime;
    startTime.start();

    // set progress to indefinite as we dont know how long data will be sent
    updateProgressbar(ProgressState::indefinite);

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
            updateStatus(string("catastrophic failure: ") + e.what());
            updateProgressbar(ProgressState::failure);
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

void ControlDialog::updateStatus(const string&msg)
{
    ui->stateState->setText(QString(msg.c_str()));
}

void ControlDialog::updateProgressbar(ProgressState state, int progress, int maximum)
{
    // handle progress display
    switch(state){
        case ProgressState::failure:
            ui->stateProgressBar->setDisabled(true);
            ui->stateProgressBar->setRange(0, 1);
            ui->stateProgressBar->setValue(0);
        break;
        case ProgressState::indefinite:
            ui->stateProgressBar->setDisabled(false);
            ui->stateProgressBar->setRange(0, 0);
        break;
        case ProgressState::working:
            ui->stateProgressBar->setDisabled(false);
            ui->stateProgressBar->setRange(0, maximum);
            ui->stateProgressBar->setValue(progress);
        break;
    }
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
