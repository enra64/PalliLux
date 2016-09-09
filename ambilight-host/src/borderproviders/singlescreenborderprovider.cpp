#include "singlescreenborderprovider.h"

#include "assert.h"

using namespace Magick;

SingleScreenBorderProvider::SingleScreenBorderProvider(size_t width, size_t height,
    size_t xOffset, size_t yOffset,
    size_t letterboxOffsetX, size_t letterboxOffsetY) :
    BorderProvider(), mWidth(width), mHeight(height), mXOffset(xOffset), mYOffset(yOffset), mVerticalLetterboxBarWidth(letterboxOffsetX), mHorizontalLetterboxBarHeight(letterboxOffsetY)
{
	SingleScreenBorderProvider::updateGeometry();
}

void SingleScreenBorderProvider::updateGeometry()
{
	mLeftGeometry = Geometry(
		mBorderWidth, //width
        mHeight - 2 * mBorderWidth - (2 * mHorizontalLetterboxBarHeight), //height
        mVerticalLetterboxBarWidth + mXOffset + 0, //x offset
        mHorizontalLetterboxBarHeight + mYOffset + mBorderWidth);// y offset

	mRightGeometry = Geometry(
		mBorderWidth, //width
        mHeight - 2 * mBorderWidth - (2 * mHorizontalLetterboxBarHeight), //height
        (-mVerticalLetterboxBarWidth) + mXOffset + mWidth - mBorderWidth, //x offset
        mHorizontalLetterboxBarHeight + mYOffset + mBorderWidth);// y offset


	mTopGeometry = Geometry(
        mWidth - (2 * mVerticalLetterboxBarWidth), //width
		mBorderWidth, //height
        mVerticalLetterboxBarWidth + mXOffset + 0, //x offset
        mHorizontalLetterboxBarHeight + mYOffset + 0);// y offset

	mBottomGeometry = Geometry(
        mWidth - (2 * mVerticalLetterboxBarWidth), //width
		mBorderWidth, //height
        mVerticalLetterboxBarWidth + mXOffset + 0, //x offset
        (-mHorizontalLetterboxBarHeight) + mYOffset + mHeight - mBorderWidth);// y offset
}

void SingleScreenBorderProvider::retrieveBorders(Magick::Image& right, Magick::Image& top, Magick::Image& left, Magick::Image& bottom)
{
    //check whether we have a ScreenshotProvider
    assert(mScreenshotProvider);

	// call takeScreenshot in case the screenshot provider needs it
	mScreenshotProvider->takeScreenshot();

	// capture each border
	mScreenshotProvider->getScreenCrop(right, mRightGeometry);
	mScreenshotProvider->getScreenCrop(top, mTopGeometry);
	mScreenshotProvider->getScreenCrop(left, mLeftGeometry);
	mScreenshotProvider->getScreenCrop(bottom, mBottomGeometry);
}
