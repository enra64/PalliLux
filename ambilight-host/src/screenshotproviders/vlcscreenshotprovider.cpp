#include "vlcscreenshotprovider.h"

#include <ctime>

float VlcScreenshotProvider::getScreenCrop(Image& result, const Geometry& d)
{
	// benchmarking start
	clock_t start = clock();

	// "copy" the last screenshot
	result = mLastScreenshot;

	// scale the geometry, as vlc provides 120x58 images
	//TODO

	// crop to the desired dimensions
	result.crop(d.xOffset, d.yOffset, d.right(), d.bottom());

	// return benchmarking value
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

void VlcScreenshotProvider::takeScreenshot()
{
	//TODO: probably should somehow start vlc here if it is not running...

	// read from the stream path
	mLastScreenshot.read(mStreamPath);
}
