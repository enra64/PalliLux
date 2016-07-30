#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cstdint>
#include <cstring>
#include <vector>
#include <iostream>

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/xwindow.h>


struct ScreenShot
{
    ScreenShot(int x, int y, int width, int height):
        x(x),
        y(y),
        width(width),
        height(height)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);
        screen = DefaultScreen(display);

        depth = DefaultDepth(display, screen);

        m_imageIsDestroyed = true;
    }

    void operator() (Magick::Image& result)
    {
        // free memory
        if(!m_imageIsDestroyed)
            XDestroyImage(img);

        // get screenshot
        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);

        // save state
        m_imageIsDestroyed = false;
        std::cout << img->depth << std::endl;
        result.read(img->width, img->height, "BGRA", Magick::CharPixel, img->data);

        //result = *MagickExport::XImportImage(img, nullptr);
        //cvImg = Mat(height, width, CV_8UC4, img->data);
    }

    ~ScreenShot()
    {
        // free memory if necessary
        if(!m_imageIsDestroyed)
            XDestroyImage(img);

        // close the display again
        XCloseDisplay(display);
    }

private:
    Display* display;
    Window root;
    int x,y,width,height, depth, screen;
    XImage* img;

    bool m_imageIsDestroyed;
};

#endif // SCREENSHOT_H
