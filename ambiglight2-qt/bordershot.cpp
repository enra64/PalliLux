#include "bordershot.h"

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/image.h>

#include <vector>

#include "screenshot.h"

using namespace Magick;
using namespace std;

void BorderShot::takeBorderShot(Image &right, Image &top, Image &left, Image &bottom)
{
    // take shots of all but the bottom border
    mScreenshotAccess.takeScreenshot(left, mLeftBorderDimensions);
    mScreenshotAccess.takeScreenshot(top, mTopBorderDimensions);
    mScreenshotAccess.takeScreenshot(right, mRightBorderDimensions);

    // the bottom bar is located over several positions, so a different approach is required:
    // we take single shots of each screen bottom, and then append those.

    // list of images
    vector<Image> bottomBorderVector = vector<Image>(3);

    mScreenshotAccess.takeScreenshot(bottomBorderVector[0], mBottomLeftBorderDimensions);
    mScreenshotAccess.takeScreenshot(bottomBorderVector[1], mBottomCenterBorderDimensions);
    mScreenshotAccess.takeScreenshot(bottomBorderVector[2], mBottomRightBorderDimensions);

    appendImages(&bottom, bottomBorderVector.begin(), bottomBorderVector.end());
}
