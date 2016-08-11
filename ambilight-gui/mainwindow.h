#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../ambilight-host/arduinoconnector.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_spinBox_2_valueChanged(int);

    void on_spinBox_valueChanged(int);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<ArduinoConnector> mAmbiConnector = std::unique_ptr<ArduinoConnector>(nullptr);
    void refreshLedCount();
};

#endif // MAINWINDOW_H
