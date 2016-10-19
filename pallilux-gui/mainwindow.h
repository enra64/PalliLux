#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
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
    void on_actionLED_Configuration_triggered();
    void on_actionSerial_Configuration_triggered();

    /// \brief find the current control widget, and start the pallilux
    void start();

    /// \brief find the current control widget, and stop the pallilux
    void stop();

    /// \brief update the tool bar status label
    void updateStatus(const QString& text, bool failure);

private:
    Ui::MainWindow *ui;

    /// \brief Get the currently displayed tab
    IScreenConfigPage* getCurrentTab();

    /// \brief dis/enable appropriate widgets
    void setRunState(bool running);

    /// \brief label used to display application state
    QLabel* mStatusLabel = nullptr;

    /// \brief text to be displayed in the start/stop button when pallilux is not running
    const QString mStartText = "Let there be light";

    /// \brief text to be displayed in the start/stop button when pallilux is running
    const QString mStopText = "Shut down";

    // configuration control stuff
private:
    /// \brief ensure that the current led and serial config is good
    void checkConfiguration();

    /// \brief add and connect a button to the bottom button bar
    void addWarningButton(const QString &text, QPushButton *&button, const char *slot);

    /// \brief remove, disable and delete a button currently display in the bottom bar
    void removeWarningButton(QPushButton *&button, const char* slot);

    /// \brief button to show that the led configuration is bad
    QPushButton* mLedButton = nullptr;

    /// \brief button to show that the serial config is bad
    QPushButton* mSerialButton = nullptr;

    /// \brief timer to periodically check for working configuration
    QTimer* mConfigurationTimer = nullptr;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *) override;
};

#endif // MAINWINDOW_H
