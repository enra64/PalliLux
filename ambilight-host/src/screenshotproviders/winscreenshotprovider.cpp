#include "winscreenshotprovider.h"

#include <ctime>
#include <iostream>
#include <assert.h>

using namespace cimg_library;
using namespace std;

WinScreenshotProvider::WinScreenshotProvider() {
}

WinScreenshotProvider::~WinScreenshotProvider() {
}

void WinScreenshotProvider::takeScreenshot() {
	// capture screen
	CScreenImage img;
	img.CaptureScreen();

	// the code can only handle BGR -> 3 bytes
	assert(img.GetBPP() / 8 == 3);

	// create result image with own bufferspace
	mImage = Image(img.GetWidth(), img.GetHeight(), CIMG_2D_Z_LEVEL_COUNT, CHANNEL_COUNT);

	// get image data from DIB
	uint8_t* bits = static_cast<uint8_t*>(img.GetBits());

	int imageWidth = mImage.width();

	for (int row = 0; row < img.GetHeight(); row++) {
		// get a pointer to the DIB section row start
		// pitch -> how many bytes are used per line (not necessarily equal to the amount of RGB bytes due to padding)
		uint8_t* rowStart = bits + img.GetPitch() * row;

		// get pointers to the CImg color locations
		uint8_t* redStart = mImage.data(0, row, 0, CIMG_RED_CHANNEL);
		uint8_t* greenStart = mImage.data(0, row, 0, CIMG_GREEN_CHANNEL);
		uint8_t* blueStart = mImage.data(0, row, 0, CIMG_BLUE_CHANNEL);

		// copy R, then G, then B into the result images buffer
		for (int pixelIndex = 0; pixelIndex < imageWidth; pixelIndex++) {
			blueStart[pixelIndex] = rowStart[3 * pixelIndex + 0];
			greenStart[pixelIndex] = rowStart[3 * pixelIndex + 1];
			redStart[pixelIndex] = rowStart[3 * pixelIndex + 2];
		}
	}
}

float WinScreenshotProvider::getScreenCrop(Image& result, const Geometry& d) {
	// benchmarking
	clock_t start = clock();

	result = mImage.crop(d.xOffset, d.yOffset, d.right(), d.bottom());

	// benchmarking end
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}