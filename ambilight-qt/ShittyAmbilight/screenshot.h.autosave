#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cstdint>
#include <cstring>
#include <ctime>
#include <vector>
#include <iostream>

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/xwindow.h>



struct ScreenShot {
    ScreenShot() {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);

        m_imageIsDestroyed = true;
    }

    clock_t takeScreenshot(Magick::Image* result, Dimensions d) {

        // free memory
        if(!m_imageIsDestroyed)
            XDestroyImage(img);

        // TODO: cut appropriate parts from the image, scale for led values

        // benchmarking start
        clock_t start = clock();

        // get screenshot  x, y, width, height,
        img = XGetImage(display, root, d.x, d.y, d.w, d.h, ~0L, ZPixmap);

        // save state
        m_imageIsDestroyed = false;

        // convert to magick image
        result->read(img->width, img->height, "BGRA", Magick::CharPixel, img->data);

        // benchmarking end
        return clock() - start;
    }

    ~ScreenShot() {
        // free memory if necessary
        if(!m_imageIsDestroyed)
            XDestroyImage(img);

        // close the display again
        XCloseDisplay(display);
    }

private:
    Display* display;
    Window root;
    XImage* img;
    bool m_imageIsDestroyed;
};

#endif // SCREENSHOT_H
