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
    virtual QString infoText() const = 0;
public:
    virtual QString pageLabel() const = 0;
    virtual ControlWidget* getWidget(QWidget* parent, LedCount d) const = 0;
};

#endif // ICONFIGPAGE_H
