#include "winscreenshotprovider.h"

#include <ctime>
#include <iostream>
#include <assert.h>

using namespace cimg_library;
using namespace std;

WinScreenshotProvider::WinScreenshotProvider() {
	mScreenshotImage.createScreenDeviceContext();
}

WinScreenshotProvider::~WinScreenshotProvider() {
	mScreenshotImage.releaseScreenDeviceContext();
}

void WinScreenshotProvider::takeScreenshot() {
	// capture screen
	mScreenshotImage.CaptureScreen();

	// the code can only handle BGR -> 3 bytes
	assert(mScreenshotImage.GetBPP() / 8 == 3);

	// create result image with own bufferspace
	mImage = Image(mScreenshotImage.GetWidth(), mScreenshotImage.GetHeight(), CIMG_2D_Z_LEVEL_COUNT, CHANNEL_COUNT);

	// get image data from DIB
	uint8_t* bits = static_cast<uint8_t*>(mScreenshotImage.GetBits());

	int imageWidth = mImage.width();

	for (int row = 0; row < mScreenshotImage.GetHeight(); row++) {
		// get a pointer to the DIB section row start
		// pitch -> how many bytes are used per line (not necessarily equal to the amount of RGB bytes due to padding)
		uint8_t* rowStart = bits + mScreenshotImage.GetPitch() * row;

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

	// TODO: this is somewhat fucked up if you have more than one monitor, as currently only the center one will be captured
	result = mImage.get_crop(d.left(), d.top(), d.right(), d.bottom());

	// benchmarking end
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}