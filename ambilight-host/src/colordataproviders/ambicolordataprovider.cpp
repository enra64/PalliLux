#include "ambicolordataprovider.h"


using namespace std;
using namespace cimg_library;

#include <vector>
#include <ctime>
#include <iostream>
#include <assert.h>

AmbiColorDataProvider::AmbiColorDataProvider(size_t bottomLedCount, size_t rightLedCount, size_t topLedCount, size_t leftLedCount)
    : ColorDataProvider(bottomLedCount, rightLedCount, topLedCount, leftLedCount)
{
	// set geometries
	mBottomLedGeometry = Geometry(bottomLedCount, 1);
	mTopLedGeometry = Geometry(topLedCount, 1);

	mLeftLedGeometry = Geometry(1, leftLedCount);
	mRightLedGeometry = Geometry(1, rightLedCount);
}

AmbiColorDataProvider::AmbiColorDataProvider(size_t horizontalLedCount, size_t verticalLedCount) : 
	AmbiColorDataProvider(horizontalLedCount, verticalLedCount, horizontalLedCount, verticalLedCount)
{
}

float AmbiColorDataProvider::getData(uint8_t* resultBuffer)
{
    //check whether we have a BorderProvider
    assert(mBorderProvider);

	clock_t start = clock();

	// take the border screenshot
	mBorderProvider->retrieveBorders(mRightImage, mTopImage, mLeftImage, mBottomImage);

	// save all borders
	//debugSaveBorders();

	// scale the images down first to minimize processing time
	flattenBorders();

	// save all borders
	//debugSaveBorders();
	
	// second, align the borders so they form a line
	unique_ptr<Image> pixelLine = alignBorders();

	// debug print
	//pixelLine->save("flataligned.bmp");

	// last, convert the line to rgb data
	imageToRgb(move(pixelLine), resultBuffer);

	// apply all filters in the list
	applyFilters(resultBuffer);

	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

std::unique_ptr<Image> AmbiColorDataProvider::alignBorders()
{
	// rotate so the border ends align
	mRightImage.rotate(90);
	mTopImage.rotate(180);
	mLeftImage.rotate(270);

	// create result image space on heap
	Image* result = new Image();

	// append the borders to a single image
	result->append(mBottomImage).append(mRightImage).append(mTopImage).append(mLeftImage);

	return std::unique_ptr<Image>(result);
}

void AmbiColorDataProvider::flattenBorders() {
	// scale all border images to a line one pixel high
	mBottomImage.resize(mBottomLedGeometry.width, mBottomLedGeometry.height, CIMG_2D_Z_LEVEL_COUNT, 4);
	mRightImage.resize(mRightLedGeometry.width, mRightLedGeometry.height, CIMG_2D_Z_LEVEL_COUNT, 4);
	mTopImage.resize(mTopLedGeometry.width, mTopLedGeometry.height, CIMG_2D_Z_LEVEL_COUNT, 4);
	mLeftImage.resize(mLeftLedGeometry.width, mLeftLedGeometry.height, CIMG_2D_Z_LEVEL_COUNT, 4);
}

void AmbiColorDataProvider::imageToRgb(std::unique_ptr<Image> lineBorder, uint8_t* result)
{
	// for each led
	for (unsigned int i = 0; i < LED_COUNT; i++)
	{
		// convert from 0-1 to 0-255
		result[i * 3 + 0] = lineBorder->atXY(i, 0, CIMG_2D_Z_LEVEL, CIMG_RED_CHANNEL);
		result[i * 3 + 1] = lineBorder->atXY(i, 0, CIMG_2D_Z_LEVEL, CIMG_GREEN_CHANNEL);
		result[i * 3 + 2] = lineBorder->atXY(i, 0, CIMG_2D_Z_LEVEL, CIMG_BLUE_CHANNEL);
		//cout << "R" << to_string(result[i + 0]) << " G" << to_string(result[i + 1]) << " B" << to_string(result[i + 2]) << std::endl;
	}

	mLastLineImage = move(lineBorder);
}

void AmbiColorDataProvider::debugSaveBorders()
{
	mRightImage.save("r.bmp");
	mLeftImage.save("l.bmp");
	mTopImage.save("t.bmp");
	mBottomImage.save("b.bmp");
}
