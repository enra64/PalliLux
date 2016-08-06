#include "singlescreenborderprovider.h"

using namespace Magick;

SingleScreenBorderProvider::SingleScreenBorderProvider(size_t width, size_t height, std::shared_ptr<Screenshot> screener) : BorderProvider(screener) {
    mRightGeometry = Geometry(BORDER_WIDTH, height - 2 * BORDER_WIDTH, width - BORDER_WIDTH, BORDER_WIDTH);
    mTopGeometry = Geometry(width, BORDER_WIDTH, 0, BORDER_WIDTH);
    mLeftGeometry = Geometry(BORDER_WIDTH, height - 2 * BORDER_WIDTH, 0, BORDER_WIDTH);
    mBottomGeometry = Geometry(width, BORDER_WIDTH, 0, BORDER_WIDTH);
}

void SingleScreenBorderProvider::retrieveBorders(Magick::Image &right, Magick::Image &top, Magick::Image &left, Magick::Image &bottom)
{
    // call takeScreenshot in case the screenshot provider needs it
    mScreenshot->takeScreenshot();

    // capture each border
    mScreenshot->getScreenCrop(right, mRightGeometry);
    mScreenshot->getScreenCrop(top, mTopGeometry);
    mScreenshot->getScreenCrop(left, mLeftGeometry);
    mScreenshot->getScreenCrop(bottom, mBottomGeometry);
}
