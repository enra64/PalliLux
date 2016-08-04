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

    // get the root window
    mRootWindow = RootWindow(mDisplay, DefaultScreen(mDisplay));
}

Screenshot::~Screenshot() {
    XCloseDisplay(mDisplay);
}

float Screenshot::takeScreenshot(Magick::Image &result, const Dimensions &d) const {
    // benchmarking start
    clock_t start = clock();

    // get windows
    XImage* xImage = XGetImage(mDisplay, mRootWindow, d.x, d.y, d.w, d.h, AllPlanes, ZPixmap);

    // check output
    if(!xImage)
        throw "Could not create XImage";

    // create a magick++ image from the screenshot
    result.read(xImage->width, xImage->height, "BGRA", Magick::CharPixel, xImage->data);

    // free memory
    XDestroyImage(xImage);

    // return benchmarking value
    return float(clock() - start) / CLOCKS_PER_SEC;
}
