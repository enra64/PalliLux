#ifndef XLIBSCREENSHOT_H
#define XLIBSCREENSHOT_H

#include "screenshotprovider.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

/**
 * @brief An implementation of the Screenshot interface; uses xlib.
 */
class XlibScreenshotProvider : public ScreenshotProvider {
public:
    XlibScreenshotProvider();
    ~XlibScreenshotProvider();

    /**
     * @brief takeScreenshot Take a screenshot, converting it to a Magick++ image
     * @param result the resulting image
     * @param d the dimensions the image will be; these are the dimensions requested from X
     * @return the time in seconds required
     */
    float getScreenCrop(Image& result, const Geometry &d) override;

private:
    /**
     * @brief mDisplay x display retrieved in constructor
     */
    static Display* mDisplay;

    /**
     * @brief mRootWindow x window thingy
     */
    Window mRootWindow;

    /**
     * @brief mImage we need this image instance, because otherwise we cannot return a reference in getScreenCrop
     */
    Image mImage;
};

#endif // XLIBSCREENSHOT_H
