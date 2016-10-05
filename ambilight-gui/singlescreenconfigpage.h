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

    // IScreenConfigPage interface
public:
    QString infoText() const override;
    QString pageLabel() const override;
    ControlWidget* getWidget(QWidget* parent, LedCount d) const override;
private slots:
    void on_letterboxAutoConfigButton_clicked();
};



#endif // SINGLESCREENCONFIGPAGE_H
