#include "rgbconstructor.h"

using namespace Magick;
using namespace std;

#include <vector>

void RgbConstructor::takeAndParseScreenShot(uint8_t* resultSpace)
{
    Image right, top, left, bottom;
    mScreenShotter.takeBorderShot(right, top, left, bottom);
    parseScreenshot(right, top, left, bottom, resultSpace);
}

void RgbConstructor::parseScreenshot(Magick::Image &rightBorder, Magick::Image &topBorder, Magick::Image &leftBorder, Magick::Image &bottomBorder, uint8_t* resultSpace) {
    // first, scale the images down to minimize processing time
    scaleImages(rightBorder, topBorder, leftBorder, bottomBorder);

    // second, align the borders so they form a line
    unique_ptr<Image> border = alignBorderImages(rightBorder, topBorder, leftBorder, bottomBorder);

    // at last, convert the line to rgb data
    convertToPixels(move(border), resultSpace);
}

std::unique_ptr<Image> RgbConstructor::alignBorderImages(Image &rightBorder, Image &topBorder, Image &leftBorder, Image &bottomBorder) {
    // rotate so the border ends align
    rightBorder.rotate(270);
    topBorder.rotate(180);
    leftBorder.rotate(270);
    // no rotation necessary for bottomBorder

    // append the borders to a single image
    vector<Image> borderList = vector<Image>();
    borderList.push_back(rightBorder);
    borderList.push_back(topBorder);
    borderList.push_back(leftBorder);
    borderList.push_back(bottomBorder);

    // create new image on heap
    Image* result = new Image();

    // append via magick++
    appendImages(result, borderList.begin(), borderList.end());

    return std::unique_ptr<Image>(result);
}

void RgbConstructor::scaleImages(Image &rightBorder, Image &topBorder, Image &leftBorder, Image &bottomBorder) {
    // reduce size of the border images
    rightBorder.sample(mVerticalLedGeometry);
    leftBorder.sample(mVerticalLedGeometry);
    topBorder.sample(mHorizontalLedGeometry);
    bottomBorder.sample(mHorizontalLedGeometry);
}

void RgbConstructor::convertToPixels(std::unique_ptr<Image> lineBorder, uint8_t* result) {
    for(unsigned int i = 0; i < LED_COUNT; i+=3) {
        ColorRGB data = lineBorder->pixelColor(i, 0);
        // convert from 0-1 to 0-255
        result[i + 0] = (255 * data.red());
        result[i + 1] = (255 * data.green());
        result[i + 2] = (255 * data.blue());
    }
}
