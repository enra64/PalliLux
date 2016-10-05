#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QLayout>

#include <memory>
#include <algorithm>

#include <arduinoconnector.h>
#include <borderprovider.h>
#include <customexceptions.h>

using namespace std;

namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = 0);
    ~ControlWidget();

    std::shared_ptr<ArduinoConnector> getArduinoConnector(){
        return mArduinoConnector;
    }

public slots:
    void start(const QString &port);
    void stop();

signals:
    /**
     * @brief Signal is emitted when there is a change of state
     * @param running true when the system is running
     */
    void onStateChanged(bool running);

protected:
    std::shared_ptr<ColorDataProvider> getColorDataProvider(){
        return mArduinoConnector->getColorDataProvider();
    }

    /**
     * @brief Update custom widgets. This must be overridden.
     */
    virtual void updateWidgets() = 0;

    /**
     * @brief Set to true to stop the run loop
     */
    bool mStopClicked = false;

    /**
     * @brief Add widget to the status group box
     */
    void addStatusWidget(QWidget* widget);

    /**
     * @brief Add widgets to the control group box; if right is nullptr, left will span the whole row
     */
    void addControlWidget(QWidget* left, QWidget *right);

    /**
     * @brief Add widget to the miscanellous group box
     */
    void addMiscWidget(QWidget* widget);

    /**
     * @brief Show a new text in the state label
     * @param msg the message to display
     * @param isFailure if true, the text will be red
     */
    void updateStatus(const std::string &msg, bool isFailure = false);

    /**
     * @brief Add a widget spanning both columns to the control widget
     */
    void addControlWidget(QWidget *row);

    /**
     * @brief Retrieve the current led config from qsettings
     */
    LedCount getLedConfig();

    std::shared_ptr<ArduinoConnector> mArduinoConnector;///< this connector instance is used to communicate with the arduino
private:
    Ui::ControlWidget *ui;
};

#endif // CONTROLWIDGET_H
