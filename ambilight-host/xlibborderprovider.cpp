#include "xlibborderprovider.h"

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/image.h>

#include <vector>
#include <ctime>

#include "screenshot.h"

using namespace Magick;
using namespace std;

void XlibBorderProvider::retrieveBorders(Image &right, Image &top, Image &left, Image &bottom) const {
    // take shots of all but the bottom border
    mScreenshot.takeScreenshot(left, mLeftBorderDimensions);
    mScreenshot.takeScreenshot(top, mTopBorderDimensions);
    mScreenshot.takeScreenshot(right, mRightBorderDimensions);

    // the bottom bar is located over several positions, so a different approach is required:
    // we take single shots of each screen bottom, and then append those.

    // list of images
    vector<Image> bottomBorderVector = vector<Image>(3);

    mScreenshot.takeScreenshot(bottomBorderVector[0], mBottomLeftBorderDimensions);
    mScreenshot.takeScreenshot(bottomBorderVector[1], mBottomCenterBorderDimensions);
    mScreenshot.takeScreenshot(bottomBorderVector[2], mBottomRightBorderDimensions);

    appendImages(&bottom, bottomBorderVector.begin(), bottomBorderVector.end());
}
