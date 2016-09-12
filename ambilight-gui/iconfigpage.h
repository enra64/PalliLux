#ifndef ICONFIGPAGE_H
#define ICONFIGPAGE_H

#include <QString>
#include <QDialog>
#include <memory>

#include <colordataprovider.h>
#include <screenshotprovider.h>
#include <ambiconnectorbuilder.h>


class IScreenConfigPage {
public:
    virtual QString pageLabel() const = 0;
    virtual QString infoText() const = 0;
    virtual void parametriseBuilder(AmbiConnectorBuilder& builder, int horizontalBorderLedCount, int verticalBorderLedCount) const = 0;
};

class ICustomDialogConfigPage : public IScreenConfigPage {
public:
    void parametriseBuilder(AmbiConnectorBuilder&, int, int) const {
        throw new std::logic_error("ArduinoConnectorConfigPage instances may not use an AmbiConnectorBuilder, use getConnector() instead");
    }

    virtual QDialog* getDialog(QWidget *parent, int horizontalBorderLedCount, int verticalBorderLedCount, std::string port) const = 0;
};

#endif // ICONFIGPAGE_H
