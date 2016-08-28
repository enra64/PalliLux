#include "singlescreenborderprovider.h"

using namespace Magick;

SingleScreenBorderProvider::SingleScreenBorderProvider(
        size_t width, size_t height, 
        std::shared_ptr<Screenshot> screener, 
        size_t xOffset, size_t yOffset, 
        size_t blackbarOffsetX, size_t blackBarOffsetY) : 
            BorderProvider(screener), mWidth(width), mHeight(height), mXOffset(xOffset), mYOffset(yOffset), mLetterBoxX(blackbarOffsetX), mLetterBoxY(blackBarOffsetY) {
    SingleScreenBorderProvider::updateGeometry();
}

void SingleScreenBorderProvider::updateGeometry() {
    mLeftGeometry = Geometry(
        mBorderWidth, //width
        mHeight - 2 * mBorderWidth - (2 * mLetterBoxY), //height
        mLetterBoxX + mXOffset + 0, //x offset
        mLetterBoxY + mYOffset + mBorderWidth);// y offset
        
    mRightGeometry = Geometry(
        mBorderWidth, //width
        mHeight - 2 * mBorderWidth - (2 * mLetterBoxY), //height
        (-mLetterBoxX) + mXOffset + mWidth - mBorderWidth, //x offset
        mLetterBoxY + mYOffset + mBorderWidth);// y offset
    

    mTopGeometry = Geometry(
        mWidth  - (2 * mLetterBoxX), //width
        mBorderWidth, //height
        mLetterBoxX + mXOffset + 0, //x offset
        mLetterBoxY + mYOffset + 0);// y offset
    
    mBottomGeometry = Geometry(
        mWidth - (2 * mLetterBoxX), //width
        mBorderWidth, //height
        mLetterBoxX + mXOffset + 0, //x offset
        (-mLetterBoxY) + mYOffset + mHeight - mBorderWidth);// y offset
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
