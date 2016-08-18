#ifndef CONTROLDIALOG_H
#define CONTROLDIALOG_H

#include <QDialog>

#include <memory>

#include "arduinoconnector.h"

namespace Ui {
class ControlDialog;
}

enum struct ProgressState {
    indefinite,
    failure,
    working
};

class ControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ControlDialog(std::shared_ptr<ArduinoConnector> connector, QWidget *parent = 0, const QString &infoString = "");
    ~ControlDialog();

private slots:
    void on_runButton_clicked();

    void on_stopButton_clicked();

private:
    void setButtonState(bool currentlyRunning);
    void updateStatus(const std::__cxx11::string &msg);
    void updateProgressbar(ProgressState state, int progress = 0, int maximum = 1);
    Ui::ControlDialog *ui;
    std::shared_ptr<ArduinoConnector> mArduinoConnector;
};

#endif // CONTROLDIALOG_H
