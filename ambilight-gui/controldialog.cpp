#include "controldialog.h"
#include "ui_controldialog.h"

#include "assert.h"
#include <string>

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
    ui->infoData->setText(infoString);
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

    // set progress to indefinite as we dont know how long data will be sent
    updateProgressbar(ProgressState::indefinite);

    // begin sending data
    while(1){
        try {
            mArduinoConnector->update();
            mArduinoConnector->draw();
            qApp->processEvents();
        } catch(AmbiConnectorException e){
            // ui update
            updateStatus(string("catastrophic failure: ") + e.what());
            updateProgressbar(ProgressState::failure);
            ui->stateFps->setText("0");
            break;
        }
    }
}

void ControlDialog::on_stopButton_clicked()
{
    // disable the run button
    setButtonState(false);
}

void ControlDialog::setButtonState(bool currentlyRunning)
{
    ui->runButton->setEnabled(!currentlyRunning);
    ui->stopButton->setEnabled(currentlyRunning);
}

void ControlDialog::updateStatus(const string&msg)
{
    ui->stateData->setText(QString(msg.c_str()));
}

void ControlDialog::updateProgressbar(ProgressState state, int progress, int maximum)
{
    // not implemented here
    assert(state != ProgressState::working);

    // handle other cases
    switch(state){
    case ProgressState::failure:
        ui->stateProgressBar->setDisabled(true);
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
