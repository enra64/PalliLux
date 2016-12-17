#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QLayout>

#include <memory>
#include <algorithm>

#include <arduinoconnector.h>
#include <borderprovider.h>
#include <customexceptions.h>

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
    virtual void start(const QString &port);
    void stop();

signals:
    /**
     * @brief Signal is emitted when there is a change of state
     * @param running true when the system is running
     */
    void onStateChanged(bool running);

    /**
     * @brief Refresh the state of this control widget
     * @param newText the text that should be displayed in a status bar of some kind
     * @param warning true if the text should appear in a warning color
     */
    void onStatusUpdate(const QString& newText, bool warning);

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
     * @brief Add widget spanning both columns
     */
    void addControlWidget(QWidget *row);

    /**
     * @brief Add widget to the miscanellous group box
     */
    void addMiscWidget(QWidget* widget);

    /// \brief overload for the signal with the same name to allow std::string parameter \overload
    void onStatusUpdate(const std::string& text, bool failure);

    std::shared_ptr<ArduinoConnector> mArduinoConnector;///< this connector instance is used to communicate with the arduino

    /// \brief remove all widgets from the control area
    void clearMiscWidgets();
private:
    Ui::ControlWidget *ui;
};

#endif // CONTROLWIDGET_H
