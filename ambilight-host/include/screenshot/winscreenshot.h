#ifndef XLIBSCREENSHOT_H
#define XLIBSCREENSHOT_H

#include "screenshot.h"


/**
 * @brief An implementation of the Screenshot interface for windows
 */
class WinScreenshot : public Screenshot {
public:
    WinScreenshot();
    ~WinScreenshot();

    /**
     * @brief takeScreenshot Take a screenshot, converting it to a Magick++ image
     * @param result the resulting image
     * @param d the dimensions the image will be; these are the dimensions requested from X
     * @return the time in seconds required
     */
    float getScreenCrop(Magick::Image& result, const Magick::Geometry &d) override;

private:
};

#endif // XLIBSCREENSHOT_H
