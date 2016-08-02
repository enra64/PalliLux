#include "screenshot.h"

#include <cstdint>
#include <cstring>
#include <ctime>
#include <vector>
#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

using namespace Magick;

Screenshot::Screenshot() {
    mDisplay = XOpenDisplay(getenv("DISPLAY"));

    // confirm display ok
    if(!mDisplay) throw "Cannot connect to X server";

    // retrieve default screen... for testing, return value is unused
    DefaultScreen(mDisplay);

    // get the root window
    mRootWindow = RootWindow(mDisplay, DefaultScreen(mDisplay));

    // retrieve background
    mBackground = getRootPixmap(mDisplay, &mRootWindow);
}

Screenshot::~Screenshot() {
    XCloseDisplay(mDisplay);
    XFreePixmap(mDisplay, mBackground);
}

float Screenshot::takeScreenshot(Magick::Image &result, const Dimensions &d) {
    // benchmarking start
    clock_t start = clock();

    // get background
    XImage* xBackgroundImage = XGetImage(mDisplay, mBackground, d.x, d.y, d.w, d.h, AllPlanes, ZPixmap);
    // get windows
    XImage* xImage = XGetImage(mDisplay, mRootWindow, d.x, d.y, d.w, d.h, AllPlanes, ZPixmap);

    // check output
    if(!xImage)
        throw "Could not create XImage";

    // create a magick++ image from the screenshot
    result.read(xImage->width, xImage->height, "BGRA", Magick::CharPixel, xImage->data);

    Image bg;
    bg.read(xBackgroundImage->width, xBackgroundImage->height, "BGRA", Magick::CharPixel, xBackgroundImage->data);

    bg.composite(result, 0, 0);

    // free memory
    XDestroyImage(xImage);

    // return benchmarking value
    return float(clock() - start) / CLOCKS_PER_SEC;
}

Pixmap Screenshot::getRootPixmap(Display *display, Window *root) {
    Pixmap currentRootPixmap;
    Atom act_type;
    int act_format;
    unsigned long nitems, bytes_after;
    unsigned char *data = nullptr;
    Atom _XROOTPMAP_ID;

    _XROOTPMAP_ID = XInternAtom(display, "_XROOTPMAP_ID", False);

    if (XGetWindowProperty(display, *root, _XROOTPMAP_ID, 0, 1, False,
                           XA_PIXMAP, &act_type, &act_format, &nitems, &bytes_after,
                           &data) == Success) {

        if (data) {
            currentRootPixmap = *((Pixmap *) data);
            XFree(data);
        }
        else
            throw "background data is nullptr";
    }
    else
        throw "XGetWindowProperty for background failed";

    return currentRootPixmap;
}
