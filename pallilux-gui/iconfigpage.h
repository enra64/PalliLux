#ifndef ICONFIGPAGE_H
#define ICONFIGPAGE_H

#include "controlwidget.h"
#include "ledconfigdialog.h"

#include <QString>
#include <QDialog>
#include <QSettings>
#include <memory>

#include <colordataprovider.h>
#include <screenshotprovider.h>
#include <ambiconnectorbuilder.h>


class IScreenConfigPage {
protected:
    virtual ControlWidget* getControlWidget(QWidget* parent) = 0;

    IScreenConfigPage(){
        mLedConfiguration = LedConfigDialog::getLedConfig();

        // if the configuration is invalid, reset it to values that wont kill
        // signed -> unsigned conversion
        if(!mLedConfiguration.valid()){
            mLedConfiguration.bottom = 0;
            mLedConfiguration.right = 0;
            mLedConfiguration.top = 0;
            mLedConfiguration.left = 0;
        }
    }

    ControlWidget* mCurrentControlWidget = nullptr;
    LedConfig mLedConfiguration;
public:
    /**
     * @brief Get a reference to the control widget of this page
     */
    virtual ControlWidget& getControlWidget() {
        return *mCurrentControlWidget;
    }
};

#endif // ICONFIGPAGE_H
