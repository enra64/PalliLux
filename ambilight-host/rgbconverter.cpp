#include "rgbconverter.h"

using namespace Magick;
using namespace std;

#include <utility>
#include <vector>
#include <ctime>
#include <iostream>
#include <assert.h>

RgbConverter::RgbConverter(std::shared_ptr<BorderProvider> provider, unsigned int horizontalLedCount, unsigned int verticalLedCount)
    : HORIZONTAL_LED_COUNT(horizontalLedCount), VERTICAL_LED_COUNT(verticalLedCount), mBorderProvider(provider) {
    // we need a borderProvider, so check for nullptr
    assert(provider.get());

    // set geometries, ignoring aspect ratio
    mVerticalLedGeometry = Magick::Geometry(1, VERTICAL_LED_COUNT);
    mVerticalLedGeometry.aspect(true);

    mHorizontalLedGeometry = Geometry(HORIZONTAL_LED_COUNT, 1);
    mHorizontalLedGeometry.aspect(true);
}

float RgbConverter::takeAndParseScreenShot(uint8_t *resultBuffer) {
    clock_t start = clock();

    // take the border screenshot
    mBorderProvider->retrieveBorders(mRightImage, mTopImage, mLeftImage, mBottomImage);


    // scale the images down first to minimize processing time
    flattenBorders();

    // debug save
    debugSaveBorders();
    // second, align the borders so they form a line
    unique_ptr<Image> pixelLine = alignBorders();

    // debug print
    pixelLine->write("test/flataligned.jpg");

    // last, convert the line to rgb data
    imageToRgb(move(pixelLine), resultBuffer);

    // apply all filters in the list
    applyFilters(resultBuffer);

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
    mRightImage.sample(mVerticalLedGeometry);//mVerticalLedGeometry);
    mLeftImage.sample(mVerticalLedGeometry);
    mTopImage.sample(mHorizontalLedGeometry);
    mBottomImage.sample(mHorizontalLedGeometry);
}

void RgbConverter::imageToRgb(std::unique_ptr<Image> lineBorder, uint8_t *result) {
    cout << "begin" << endl;
    // for each led
    for(unsigned int i = 0; i < LED_COUNT; i++) {
        // retrieve rgb data from the line
        ColorRGB data = lineBorder->pixelColor(i, 0);
        // convert from 0-1 to 0-255, applying the brightness factor
        result[i*3 + 0] = (255 * data.red());
        result[i*3 + 1] = (255 * data.green());
        result[i*3 + 2] = (255 * data.blue());
        cout << "R" << to_string(result[i + 0]) << " G" << to_string(result[i + 1]) << " B" << to_string(result[i + 2]) << std::endl;
    }
}

void RgbConverter::debugSaveBorders() {
    mRightImage.write("test/r.jpg");
    mLeftImage.write("test/l.jpg");
    mTopImage.write("test/t.jpg");
    mBottomImage.write("test/b.jpg");
}

void RgbConverter::applyFilters(uint8_t *resultBuffer)
{
    // iterate over the filter pairs: pair<string, unique_ptr<DataFilter>>&
    for(auto& filter : mDataFilters){
        // call the functor
        (*filter.second)(resultBuffer, getRequiredBufferLength());
    }
}

void RgbConverter::addFilter(string id, unique_ptr<DataFilter> filter){
    // insert (key, value) pair into filter list
    mDataFilters.emplace(std::make_pair(id, move(filter)));
}

unique_ptr<DataFilter> RgbConverter::removeFilter(string id)
{
    // find the filter
    const auto& result = mDataFilters.find(id);

    // return "null" when id not found
    if(result == mDataFilters.end())
        return unique_ptr<DataFilter>(nullptr);

    // move out of map before erasing the value
    unique_ptr<DataFilter> tmp = std::move(result->second);

    // erase
    mDataFilters.erase(id);

    // return
    return move(tmp);
}
