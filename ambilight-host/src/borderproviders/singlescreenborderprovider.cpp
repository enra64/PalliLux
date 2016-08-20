#include "singlescreenborderprovider.h"

using namespace Magick;

SingleScreenBorderProvider::SingleScreenBorderProvider(
        size_t width, size_t height, 
        std::shared_ptr<Screenshot> screener, 
        size_t xOffset, size_t yOffset, 
        size_t blackbarOffsetX, size_t blackBarOffsetY) : BorderProvider(screener) {
    mLeftGeometry = Geometry(
        BORDER_WIDTH, //width
        height - 2 * BORDER_WIDTH - (2 * blackBarOffsetY), //height
        blackbarOffsetX + xOffset + 0, //x offset
        blackBarOffsetY + yOffset + BORDER_WIDTH);// y offset
        
    mRightGeometry = Geometry(
        BORDER_WIDTH, //width
        height - 2 * BORDER_WIDTH - (2 * blackBarOffsetY), //height
        (-blackbarOffsetX) + xOffset + width - BORDER_WIDTH, //x offset
        blackBarOffsetY + yOffset + BORDER_WIDTH);// y offset
    

    mTopGeometry = Geometry(
        width  - (2 * blackbarOffsetX), //width
        BORDER_WIDTH, //height
        blackbarOffsetX + xOffset + 0, //x offset
        blackBarOffsetY + yOffset + 0);// y offset
    
    mBottomGeometry = Geometry(
        width - (2 * blackbarOffsetX), //width
        BORDER_WIDTH, //height
        blackbarOffsetX + xOffset + 0, //x offset
        (-blackBarOffsetY) + yOffset + height - BORDER_WIDTH);// y offset
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
