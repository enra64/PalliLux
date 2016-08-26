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
    void on_yLedSpin_valueChanged(int);
    void on_xLedSpin_valueChanged(int);

    void on_configStackPrevButton_clicked();
    void on_configStackNextButton_clicked();

    void on_ttyState_textChanged(const QString &arg1);

    void on_startControlDialogButton_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<ArduinoConnector> mAmbiConnector = std::unique_ptr<ArduinoConnector>(nullptr);
    void refreshLedCount();
    std::shared_ptr<BorderProvider> getSingleScreenBorderProvider(std::shared_ptr<Screenshot> screener);
    std::shared_ptr<RgbLineProvider> createAmbilightRgbProvider();
    const IScreenConfigPage* getCurrentPage();
    QString getInfoText();
};

#endif // MAINWINDOW_H
