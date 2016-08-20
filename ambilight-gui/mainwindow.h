#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

#include "arduinoconnector.h"

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
    void on_pushButton_clicked();

    void on_yLedSpin_valueChanged(int);

    void on_xLedSpin_valueChanged(int);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<ArduinoConnector> mAmbiConnector = std::unique_ptr<ArduinoConnector>(nullptr);
    void refreshLedCount();
    std::shared_ptr<BorderProvider> getSingleScreenBorderProvider(std::shared_ptr<Screenshot> screener);
    std::unique_ptr<RgbLineProvider> createAmbilightRgbProvider();
    QString getInfoText();
};

#endif // MAINWINDOW_H
