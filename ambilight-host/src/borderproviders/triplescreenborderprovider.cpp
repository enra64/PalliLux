#include "triplescreenborderprovider.h"

#include <Magick++.h>

#include <vector>

#include "xlibscreenshot.h"

using namespace Magick;
using namespace std;

TripleScreenBorderProvider::TripleScreenBorderProvider(std::shared_ptr<Screenshot> screener, size_t w1, size_t h1, size_t w2, size_t h2, size_t w3, size_t h3) : BorderProvider(screener), LEFT_SCREEN_WIDTH(w1), LEFT_SCREEN_HEIGHT(h1), CENTER_SCREEN_WIDTH(w2), CENTER_SCREEN_HEIGHT(h2), RIGHT_SCREEN_WIDTH(w3), RIGHT_SCREEN_HEIGHT(h3)
{
	TripleScreenBorderProvider::updateGeometry();
}

void TripleScreenBorderProvider::retrieveBorders(Image& right, Image& top, Image& left, Image& bottom)
{
	// take the screenshot (if the screenshot class overrides it)
	mScreenshot->takeScreenshot();

	// take shots of all but the bottom border
	mScreenshot->getScreenCrop(left, mLeftBorderDimensions);
	mScreenshot->getScreenCrop(top, mTopBorderDimensions);
	mScreenshot->getScreenCrop(right, mRightBorderDimensions);

	// the bottom bar is located over several positions, so a different approach is required:
	// we take single shots of each screen bottom, and then append those.

	// list of images
	vector<Image> bottomBorderVector = vector<Image>(3);

	mScreenshot->getScreenCrop(bottomBorderVector[0], mBottomLeftBorderDimensions);
	mScreenshot->getScreenCrop(bottomBorderVector[1], mBottomCenterBorderDimensions);
	mScreenshot->getScreenCrop(bottomBorderVector[2], mBottomRightBorderDimensions);

	appendImages(&bottom, bottomBorderVector.begin(), bottomBorderVector.end());
}

void TripleScreenBorderProvider::updateGeometry()
{
	LEFT_X_POSITION = 0;
	LEFT_Y_POSITION = 0;

	CENTER_X_POSITION = LEFT_SCREEN_WIDTH;
	CENTER_Y_POSITION = 0;

	RIGHT_X_POSITION = LEFT_SCREEN_WIDTH + CENTER_SCREEN_WIDTH;
	RIGHT_Y_POSITION = 0;

	mTopBorderDimensions = Geometry(ALL_WIDTH, mBorderWidth, LEFT_X_POSITION, LEFT_Y_POSITION);
	mLeftBorderDimensions = Geometry(mBorderWidth, LEFT_SCREEN_HEIGHT - 2 * mBorderWidth, LEFT_X_POSITION, mBorderWidth);
	mRightBorderDimensions = Geometry(mBorderWidth, RIGHT_SCREEN_HEIGHT - 2 * mBorderWidth, ALL_WIDTH - mBorderWidth, mBorderWidth);

	mBottomLeftBorderDimensions = Geometry(LEFT_SCREEN_WIDTH, mBorderWidth, LEFT_X_POSITION, LEFT_SCREEN_HEIGHT - mBorderWidth);
	mBottomCenterBorderDimensions = Geometry(CENTER_SCREEN_WIDTH, mBorderWidth, CENTER_X_POSITION, CENTER_SCREEN_HEIGHT - mBorderWidth);
	mBottomRightBorderDimensions = Geometry(RIGHT_SCREEN_WIDTH, mBorderWidth, RIGHT_X_POSITION, RIGHT_SCREEN_HEIGHT - mBorderWidth);
}
