#ifndef ICONFIGPAGE_H
#define ICONFIGPAGE_H

#include "controlwidget.h"

#include <QString>
#include <QDialog>
#include <memory>

#include <colordataprovider.h>
#include <screenshotprovider.h>
#include <ambiconnectorbuilder.h>


class IScreenConfigPage {
protected:
    virtual ControlWidget* getWidget(QWidget* parent, LedCount d) const = 0;
    ControlWidget* mCurrentControlWidget = nullptr;
public:
    virtual ControlWidget& getWidget() {
        return *mCurrentControlWidget;
    }

    virtual void updateLedCount(const LedCount& l) = 0;
};

#endif // ICONFIGPAGE_H
