#include "ddapiscreenshotprovider.h"

#include <ctime>
#include <iostream>
#include <assert.h>

using namespace cimg_library;
using namespace std;

DdApiScreenshotProvider::DdApiScreenshotProvider() {
}

DdApiScreenshotProvider::~DdApiScreenshotProvider() {
}

void DdApiScreenshotProvider::takeScreenshot() {
	
}

float DdApiScreenshotProvider::getScreenCrop(Image& result, const Geometry& d) {
	// benchmarking
	clock_t start = clock();

	// TODO: this is somewhat fucked up if you have more than one monitor, as currently only the center one will be captured
	result = mImage.get_crop(d.left(), d.top(), d.right(), d.bottom());

	// benchmarking end
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}