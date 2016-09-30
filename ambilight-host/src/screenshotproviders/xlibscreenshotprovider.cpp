#include "xlibscreenshotprovider.h"

#include <cstdint>
#include <cstring>
#include <ctime>
#include <vector>
#include <iostream>

#include "customexceptions.h"



XlibScreenshotProvider::XlibScreenshotProvider() {
    mDisplay = XOpenDisplay(getenv("DISPLAY"));

    // confirm display ok
    if(!mDisplay)
        throw ScreenshotException("Cannot connect to X server");

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
    // possibly use a plane_mask here? we dont need alpha, so 0xffffff00 would be appropriate i guess
    XImage* xImage = XGetImage(mDisplay, mRootWindow, d.xOffset, d.yOffset, d.width, d.height, AllPlanes, ZPixmap);

    // check output
    if(!xImage)
        throw ScreenshotException("XImage could not be created, perhaps invalid screen boundaries");

    // create an Image from the screenshot
    result.read(xImage->data, "BGRA", 4, xImage->width, xImage->height, 3, nullptr, 0);

    // free memory
    XDestroyImage(xImage);

    // return benchmarking value
    return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}
