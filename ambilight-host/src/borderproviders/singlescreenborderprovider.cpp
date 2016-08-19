#include "singlescreenborderprovider.h"

using namespace Magick;

SingleScreenBorderProvider::SingleScreenBorderProvider(size_t width, size_t height, std::shared_ptr<Screenshot> screener, size_t xOffset, size_t yOffset) : BorderProvider(screener) {
    mRightGeometry = Geometry(BORDER_WIDTH, height - 2 * BORDER_WIDTH, xOffset + width - BORDER_WIDTH, yOffset + BORDER_WIDTH);
    mLeftGeometry = Geometry(BORDER_WIDTH, height - 2 * BORDER_WIDTH, xOffset + 0, yOffset + BORDER_WIDTH);

    mTopGeometry = Geometry(width, BORDER_WIDTH, xOffset + 0, yOffset + 0);
    mBottomGeometry = Geometry(width, BORDER_WIDTH, xOffset + 0, yOffset + height - BORDER_WIDTH);
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
