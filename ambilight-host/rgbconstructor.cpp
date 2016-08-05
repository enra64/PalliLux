#include "rgbconstructor.h"

using namespace Magick;
using namespace std;

#include <vector>
#include <ctime>
#include <iostream>
#include <assert.h>

RgbConverter::RgbConverter(std::shared_ptr<BorderProvider> provider, unsigned int horizontalLedCount, unsigned int verticalLedCount)
    : HORIZONTAL_LED_COUNT(horizontalLedCount), VERTICAL_LED_COUNT(verticalLedCount), mBorderProvider(provider) {
    // we need a borderProvider, so check for nullptr
    assert(provider.get());
}

float RgbConverter::takeAndParseScreenShot(uint8_t *resultBuffer) {
    clock_t start = clock();

    // take the border screenshot
    mBorderProvider->retrieveBorders(mRightImage, mTopImage, mLeftImage, mBottomImage);

    // debug save
    //debugSaveBorders();

    // scale the images down first to minimize processing time
    flattenBorders();

    // second, align the borders so they form a line
    unique_ptr<Image> pixelLine = alignBorders();

    // debug print
    //pixelLine->write("test/flataligned.jpg");

    // last, convert the line to rgb data
    imageToRgb(move(pixelLine), resultBuffer);

    return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

std::unique_ptr<Image> RgbConverter::alignBorders() {
    // rotate so the border ends align
    mRightImage.rotate(90);
    mTopImage.rotate(180);
    mLeftImage.rotate(270);

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

void RgbConverter::flattenBorders() {
    // reduce size of the border images
    mRightImage.sample(mVerticalLedGeometry);
    mLeftImage.sample(mVerticalLedGeometry);
    mTopImage.sample(mHorizontalLedGeometry);
    mBottomImage.sample(mHorizontalLedGeometry);
}

void RgbConverter::imageToRgb(std::unique_ptr<Image> lineBorder, uint8_t *result) {
    // for each led
    for(unsigned int i = 0; i < LED_COUNT; i++) {
        // retrieve rgb data from the line
        ColorRGB data = lineBorder->pixelColor(i, 0);
        // convert from 0-1 to 0-255, applying the brightness factor
        result[i*3 + 0] = (255 * data.red() * mBrightnessFactor);
        result[i*3 + 1] = (255 * data.green() * mBrightnessFactor);
        result[i*3 + 2] = (255 * data.blue() * mBrightnessFactor);
        //cout << "R" << to_string(result[i + 0]) << " G" << to_string(result[i + 1]) << " B" << to_string(result[i + 2]) << std::endl;
    }
}

void RgbConverter::debugSaveBorders() {
    mRightImage.write("test/r.jpg");
    mLeftImage.write("test/l.jpg");
    mTopImage.write("test/t.jpg");
    mBottomImage.write("test/b.jpg");
}
