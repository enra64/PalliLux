#ifndef ICONFIGPAGE_H
#define ICONFIGPAGE_H

#include <QString>
#include <QDialog>
#include <memory>

#include <colordataprovider.h>
#include <screenshotprovider.h>
#include <ambiconnectorbuilder.h>

#ifdef __linux__
    #include <xlibscreenshotprovider.h>
#elif _WIN32_WINNT
    #include <winscreenshotprovider.h>
#else
    #error Platform not recognized
#endif

class IScreenConfigPage {
public:
    virtual QString pageLabel() const = 0;
    virtual QString infoText() const = 0;
    virtual void parametriseBuilder(AmbiConnectorBuilder& builder, int horizontalBorderLedCount, int verticalBorderLedCount) const = 0;

    /**
     * @brief Super ugly function to get win/xlibscreenshotprovider in one line of code
     */
    static std::shared_ptr<ScreenshotProvider> getPlatformAppropriateScreenshotProvider(){
        #ifdef __linux__
            return std::shared_ptr<ScreenshotProvider>(new XlibScreenshotProvider());
        #elif _WIN32_WINNT
            return std::shared_ptr<Screenshot>(new WinScreenshotProvider());
        #else
            #error Platform not recognized
        #endif
    }
};

class ICustomDialogConfigPage : public IScreenConfigPage {
public:
    void parametriseBuilder(AmbiConnectorBuilder&, int, int) const {
        throw new std::logic_error("ArduinoConnectorConfigPage instances may not use an AmbiConnectorBuilder, use getConnector() instead");
    }

    virtual QDialog* getDialog(QWidget *parent, int horizontalBorderLedCount, int verticalBorderLedCount, std::string port) const = 0;
};

#endif // ICONFIGPAGE_H
