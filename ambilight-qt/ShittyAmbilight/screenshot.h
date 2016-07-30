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

    void operator() (Magick::Image& result) {

        // free memory
        if(!m_imageIsDestroyed)
            XDestroyImage(img);

        // benchmarking start
        clock_t start = clock();

        // get screenshot
        img = XGetImage(display, root, 0, 0, WIDTH, HEIGHT, AllPlanes, ZPixmap);

        // save state
        m_imageIsDestroyed = false;

        // convert to magick image
        result.read(img->width, img->height, "BGRA", Magick::CharPixel, img->data);

        // benchmarking end
        printf ("screenshot took (%f seconds).\n", ((float)(clock() - start))/CLOCKS_PER_SEC);
    }

    ~ScreenShot() {
        // free memory if necessary
        if(!m_imageIsDestroyed)
            XDestroyImage(img);

        // close the display again
        XCloseDisplay(display);
    }

private:
    const int WIDTH = 1024 + 1920 + 1080;
    const int HEIGHT = 1920;

    Display* display;
    Window root;

    XImage* img;

    bool m_imageIsDestroyed;
};

#endif // SCREENSHOT_H
