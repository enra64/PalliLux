#include "screenshot.h"

#include <cstdint>
#include <cstring>
#include <ctime>
#include <vector>
#include <iostream>

Screenshot::Screenshot() {
    mDisplay = XOpenDisplay(nullptr);
    mRootWindow = DefaultRootWindow(mDisplay);
}

Screenshot::~Screenshot() {
    XCloseDisplay(mDisplay);
}

float Screenshot::takeScreenshot(Magick::Image &result, const Dimensions &d)
{
    // benchmarking start
    clock_t start = clock();

    // get screenshot  x, y, width, height,
    XImage* xImage = XGetImage(mDisplay, mRootWindow, d.x, d.y, d.w, d.h, ~0L, ZPixmap);

    // create a magick++ image from the screenshot
    result.read(xImage->width, xImage->height, "BGRA", Magick::CharPixel, xImage->data);

    // free memory
    XDestroyImage(xImage);

    // return benchmarking value
    return float(clock() - start) / CLOCKS_PER_SEC;
}
