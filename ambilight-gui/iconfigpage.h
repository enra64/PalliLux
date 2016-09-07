#ifndef ICONFIGPAGE_H
#define ICONFIGPAGE_H

#include <QString>
#include <memory>

#include <rgblineprovider.h>
#include <screenshot.h>
#include <ambiconnectorbuilder.h>

#ifdef __linux__
    #include <xlibscreenshot.h>
#elif _WIN32_WINNT
    #include <winscreenshot.h>
#else
    #error Platform not recognized
#endif

class IScreenConfigPage {
public:
    virtual QString pageLabel() const = 0;
    virtual QString infoText() const = 0;
    virtual void parametriseBuilder(AmbiConnectorBuilder& builder, int horizontalBorderLedCount, int verticalBorderLedCount) const = 0;

    // super ugly function to get win/xlibscreenshot
    static std::shared_ptr<Screenshot> getPlatformAppropriateScreenshot(){
        #ifdef __linux__
            return std::shared_ptr<Screenshot>(new XlibScreenshot());
        #elif _WIN32_WINNT
            return std::shared_ptr<Screenshot>(new WinScreenshot());
        #else
            #error Platform not recognized
        #endif
    }
};

#endif // ICONFIGPAGE_H
