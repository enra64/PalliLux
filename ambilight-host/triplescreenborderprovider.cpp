#include "triplescreenborderprovider.h"

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/image.h>

#include <vector>
#include <ctime>

#include "xlibscreenshot.h"

using namespace Magick;
using namespace std;

void TripleScreenBorderProvider::retrieveBorders(Image &right, Image &top, Image &left, Image &bottom) {
    // take the screenshot (if the screenshot class overrides it)
    mScreenshot.takeScreenshot();

    // take shots of all but the bottom border
    mScreenshot.getScreenCrop(left, mLeftBorderDimensions);
    mScreenshot.getScreenCrop(top, mTopBorderDimensions);
    mScreenshot.getScreenCrop(right, mRightBorderDimensions);

    // the bottom bar is located over several positions, so a different approach is required:
    // we take single shots of each screen bottom, and then append those.

    // list of images
    vector<Image> bottomBorderVector = vector<Image>(3);

    mScreenshot.getScreenCrop(bottomBorderVector[0], mBottomLeftBorderDimensions);
    mScreenshot.getScreenCrop(bottomBorderVector[1], mBottomCenterBorderDimensions);
    mScreenshot.getScreenCrop(bottomBorderVector[2], mBottomRightBorderDimensions);

    appendImages(&bottom, bottomBorderVector.begin(), bottomBorderVector.end());
}
