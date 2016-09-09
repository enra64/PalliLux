#include "xlibscreenshotprovider.h"

#include <cstdint>
#include <cstring>
#include <ctime>
#include <vector>
#include <iostream>

#include <Magick++.h>

using namespace Magick;

XlibScreenshotProvider::XlibScreenshotProvider() {
    mDisplay = XOpenDisplay(getenv("DISPLAY"));

    // confirm display ok
    if(!mDisplay) throw "Cannot connect to X server";

    // get the root window
    mRootWindow = RootWindow(mDisplay, DefaultScreen(mDisplay));
}

XlibScreenshotProvider::~XlibScreenshotProvider() {
    XCloseDisplay(mDisplay);
}

float XlibScreenshotProvider::getScreenCrop(Image &result, const Geometry& d) {
    // benchmarking start
    clock_t start = clock();

    // get windows
    XImage* xImage = XGetImage(mDisplay, mRootWindow, d.xOff(), d.yOff(), d.width(), d.height(), AllPlanes, ZPixmap);

    // check output
    if(!xImage)
        throw "Could not create XImage";

    // create a magick++ image from the screenshot
    result.read(xImage->width, xImage->height, "BGRA", Magick::CharPixel, xImage->data);

    // free memory
    XDestroyImage(xImage);

    // return benchmarking value
    return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}
