#include "rgbconstructor.h"

using namespace Magick;
using namespace std;

#include <vector>
#include <ctime>

float RgbConstructor::takeAndParseScreenShot(uint8_t* resultSpace)
{
    clock_t start = clock();

    // take the border screenshot
    mBorderProvider.retrieveBorders(mRightImage, mTopImage, mLeftImage, mBottomImage);

    // debug save
    //debugSaveBorders();

    // scale the images down first to minimize processing time
    flattenBorders();

    // second, align the borders so they form a line
    unique_ptr<Image> pixelLine = alignBorders();

    // debug print
    //pixelLine->write("test/flataligned.jpg");

    // last, convert the line to rgb data
    imageToRgb(move(pixelLine), resultSpace);

    return float(clock() - start) / CLOCKS_PER_SEC;
}

std::unique_ptr<Image> RgbConstructor::alignBorders() {
    // rotate so the border ends align
    mRightImage.rotate(270);
    mTopImage.rotate(180);
    mLeftImage.rotate(270);
    // no rotation necessary for bottomBorder

    // append the borders to a single image
    vector<Image> borderList = vector<Image>();
    borderList.push_back(mRightImage);
    borderList.push_back(mTopImage);
    borderList.push_back(mLeftImage);
    borderList.push_back(mBottomImage);

    // create new image on heap
    Image* result = new Image();

    // append via magick++
    appendImages(result, borderList.begin(), borderList.end());

    return std::unique_ptr<Image>(result);
}

void RgbConstructor::flattenBorders() {
    // reduce size of the border images
    mRightImage.sample(mVerticalLedGeometry);
    mLeftImage.sample(mVerticalLedGeometry);
    mTopImage.sample(mHorizontalLedGeometry);
    mBottomImage.sample(mHorizontalLedGeometry);
}

void RgbConstructor::imageToRgb(std::unique_ptr<Image> lineBorder, uint8_t* result) {
    // for each led
    for(unsigned int i = 0; i < LED_COUNT; i+=3) {
        // retrieve rgb data from the line
        ColorRGB data = lineBorder->pixelColor(i, 0);
        // convert from 0-1 to 0-255
        result[i + 0] = (255 * data.red());
        result[i + 1] = (255 * data.green());
        result[i + 2] = (255 * data.blue());
    }
}

void RgbConstructor::debugSaveBorders()
{
    mRightImage.write("test/r.jpg");
    mLeftImage.write("test/l.jpg");
    mTopImage.write("test/t.jpg");
    mBottomImage.write("test/b.jpg");
}