#ifndef ICONFIGPAGE_H
#define ICONFIGPAGE_H

#include <QString>
#include <memory>
#include <rgblineprovider.h>

class IScreenConfigPage {
public:
    virtual QString pageLabel() const = 0;
    virtual QString infoText() const = 0;
    virtual std::shared_ptr<RgbLineProvider> rgbProvider(int horizontalBorderLedCount, int verticalBorderLedCount) const = 0;
};

#endif // ICONFIGPAGE_H
