#include "vlcscreenshot.h"

#include <ctime>

float VlcScreenshot::getScreenCrop(Magick::Image& result, const Magick::Geometry& d)
{
	// benchmarking start
	clock_t start = clock();

	// "copy" the last screenshot
	result = mLastScreenshot;

	// scale the geometry, as vlc provides 120x58 images
	//TODO

	// crop to the desired dimensions
	result.crop(d);

	// return benchmarking value
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

void VlcScreenshot::takeScreenshot()
{
	//TODO: probably should somehow start vlc here if it is not running...

	// read from the stream path
	mLastScreenshot.read(mStreamPath);
}
