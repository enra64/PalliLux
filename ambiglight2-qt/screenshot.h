#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/xwindow.h>

#include "dimensions.h"

/**
 * @brief The Screenshot class provides access to screen capturing functionality, mostly by its screenCapture method.
 * It could be overridden or modified to use something else than the X display server.
 */
class Screenshot
{
public:
    Screenshot();
    ~Screenshot();

    /**
     * @brief takeScreenshot Take a screenshot, converting it to a Magick++ image
     * @param result the resulting image
     * @param d the dimensions the image will be; these are the dimensions requested from X
     * @return the time in seconds required
     */
    float takeScreenshot(Magick::Image& result, const Dimensions& d);

private:
    /**
     * @brief mDisplay x display retrieved in constructor
     */
    Display* mDisplay;

    /**
     * @brief mRootWindow i have no idea. needed for x screen capture.
     */
    Window mRootWindow;
};

#endif // SCREENSHOT_H
