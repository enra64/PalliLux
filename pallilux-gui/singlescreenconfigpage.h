#ifndef SINGLESCREENCONFIGPAGE_H
#define SINGLESCREENCONFIGPAGE_H


#include <QWidget>

#include <memory>

#include "iconfigpage.h"

#include <borderprovider.h>
#include <screenshotprovider.h>
#include <colordataprovider.h>

namespace Ui {
class SingleScreenConfigPage;
}

class SingleScreenConfigPage : public QWidget, public IScreenConfigPage {
    Q_OBJECT

public:
    explicit SingleScreenConfigPage(QWidget *parent = 0);
    ~SingleScreenConfigPage();

private:
    Ui::SingleScreenConfigPage *ui;
    std::shared_ptr<BorderProvider> getSingleScreenBorderProvider() const;

    // persist configuration
private:
    void loadConfigFromSettings();
    void saveConfigToSettings();

    // IScreenConfigPage interface
private:
    ControlWidget* getControlWidget(QWidget* parent) override;
private slots:
    void on_letterboxAutoConfigButton_clicked();
    void updateBorderProvider();
};



#endif // SINGLESCREENCONFIGPAGE_H
