#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

#include "arduinoconnector.h"
#include "iconfigpage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ttyState_textChanged(const QString &);

    void on_startControlDialogButton_clicked();

    void updateTabLedCount();
    void refreshLedCount();

    void on_actionLED_Configuration_triggered();

private:
    Ui::MainWindow *ui;
    IScreenConfigPage* getCurrentTab();
    void showNoSerialWarning();
    bool enteredSerialOk();
    void loadControlWidget();
    void setRunState(bool running);
};

#endif // MAINWINDOW_H
