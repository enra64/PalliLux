#include "ambicolordataprovider.h"

using namespace Magick;
using namespace std;

#include <vector>
#include <ctime>
#include <iostream>
#include <assert.h>

AmbiRgbLineProvider::AmbiRgbLineProvider(unsigned int horizontalLedCount, unsigned int verticalLedCount)
    : ColorDataProvider(horizontalLedCount, verticalLedCount)
{
	// set geometries, ignoring aspect ratio
	mVerticalLedGeometry = Magick::Geometry(1, VERTICAL_LED_COUNT);
	mVerticalLedGeometry.aspect(true);

	mHorizontalLedGeometry = Geometry(HORIZONTAL_LED_COUNT, 1);
	mHorizontalLedGeometry.aspect(true);
}

float AmbiRgbLineProvider::getData(uint8_t* resultBuffer)
{
    //check whether we have a BorderProvider
    assert(mBorderProvider);

	clock_t start = clock();

	// take the border screenshot
	mBorderProvider->retrieveBorders(mRightImage, mTopImage, mLeftImage, mBottomImage);

	// scale the images down first to minimize processing time
	flattenBorders();

	// debug save
	//debugSaveBorders();
	// second, align the borders so they form a line
	unique_ptr<Image> pixelLine = alignBorders();

	// debug print
	//pixelLine->write("flataligned.jpg");

	// last, convert the line to rgb data
	imageToRgb(move(pixelLine), resultBuffer);

	// apply all filters in the list
	applyFilters(resultBuffer);

	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

std::unique_ptr<Image> AmbiRgbLineProvider::alignBorders()
{
	// rotate so the border ends align
	mRightImage.rotate(90);
	mTopImage.rotate(180);
	mLeftImage.rotate(270);

	// create result image space on heap
	Image* result = new Image();

	// append the borders to a single image
	vector<Image> borderList = vector<Image>();
	borderList.push_back(mBottomImage);
	borderList.push_back(mRightImage);
	borderList.push_back(mTopImage);
	borderList.push_back(mLeftImage);

	// append the rotated images to a line
	appendImages(result, borderList.begin(), borderList.end());

	return std::unique_ptr<Image>(result);
}

void AmbiRgbLineProvider::flattenBorders()
{
	// scale the vertical border images to a line
	mRightImage.scale(mVerticalLedGeometry);
	mLeftImage.scale(mVerticalLedGeometry);

	// scale the horizontal border images to a line
	mTopImage.scale(mHorizontalLedGeometry);
	mBottomImage.scale(mHorizontalLedGeometry);
}

void AmbiRgbLineProvider::imageToRgb(std::unique_ptr<Image> lineBorder, uint8_t* result)
{
	// for each led
	for (unsigned int i = 0; i < LED_COUNT; i++)
	{
		// retrieve rgb data from the line
		ColorRGB data = lineBorder->pixelColor(i, 0);
		// convert from 0-1 to 0-255
		result[i * 3 + 0] = (255 * data.red());
		result[i * 3 + 1] = (255 * data.green());
		result[i * 3 + 2] = (255 * data.blue());
		//cout << "R" << to_string(result[i + 0]) << " G" << to_string(result[i + 1]) << " B" << to_string(result[i + 2]) << std::endl;
	}

	mLastLineImage = move(lineBorder);
}

void AmbiRgbLineProvider::debugSaveBorders()
{
	mRightImage.write("test/r.jpg");
	mLeftImage.write("test/l.jpg");
	mTopImage.write("test/t.jpg");
	mBottomImage.write("test/b.jpg");
}
