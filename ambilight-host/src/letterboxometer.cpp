#include "letterboxometer.h"

#include <utility>

using namespace std;
using namespace cimg_library;

LetterboxOMeter::LetterboxOMeter(
    std::shared_ptr<ScreenshotProvider> screenshotProvider, size_t w, size_t h, size_t xOff, size_t yOff, uint8_t blackThreshold) :
    mScreenshotProvider(screenshotProvider), mScreenWidth(w), mScreenHeight(h),
    mScreenXOffset(xOff), mScreenYOffset(yOff), mBlackThreshold(blackThreshold) {
}

std::pair<int, int> LetterboxOMeter::measure() {
    Geometry horizontalBarGeometry, verticalBarGeometry;
    Image horizontalBar, verticalBar;
    int letterboxVerticalBarWidth = -1, letterboxHorizontalBarHeight = -1;

    // move the bar as far down as is the screen is black
    do {
        // move the bar
        horizontalBarGeometry = getHorizontalBar(1, ++letterboxHorizontalBarHeight);

        // refresh screenshot data
        mScreenshotProvider->takeScreenshot();
        mScreenshotProvider->getScreenCrop(horizontalBar, horizontalBarGeometry);

    // only continue if the bar is still completely black
    } while (isBlack(horizontalBar));


    // move the bar as far right as is the screen is black
    do {
        // move the bar
        verticalBarGeometry = getVerticalBar(1, ++letterboxVerticalBarWidth);

        // refresh screenshot data
        mScreenshotProvider->takeScreenshot();
        mScreenshotProvider->getScreenCrop(verticalBar, verticalBarGeometry);

    // only continue if the bar is still completely black
    } while (isBlack(verticalBar));

    return make_pair(letterboxHorizontalBarHeight, letterboxVerticalBarWidth);
}

bool LetterboxOMeter::isBlack(Image img) {
    for (size_t x = 0; x < img.width(); x++) {
        for (size_t y = 0; y < img.height(); y++) {
            if (img(x, y, CIMG_2D_Z_LEVEL, CIMG_RED_CHANNEL) > mBlackThreshold) return false;
            if (img(x, y, CIMG_2D_Z_LEVEL, CIMG_GREEN_CHANNEL) > mBlackThreshold) return false;
            if (img(x, y, CIMG_2D_Z_LEVEL, CIMG_BLUE_CHANNEL) > mBlackThreshold) return false;
        }
    }
    return true;
}

Geometry LetterboxOMeter::getHorizontalBar(size_t height, size_t topOffset) const {
    return Geometry(mScreenWidth, height, mScreenXOffset, mScreenYOffset + topOffset);
}

Geometry LetterboxOMeter::getVerticalBar(size_t width, size_t leftOffset) const {
    return Geometry(width, mScreenHeight, mScreenXOffset + leftOffset, mScreenYOffset);
}









