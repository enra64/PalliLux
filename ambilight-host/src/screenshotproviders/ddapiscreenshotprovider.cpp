#include "ddapiscreenshotprovider.h"

#include <ctime>
#include <iostream>
#include <assert.h>

using namespace cimg_library;
using namespace std;

DdApiScreenshotProvider::DdApiScreenshotProvider() {
	// initialize direct x device and connection
	
	// resulting feature level nobody cares about
	D3D_FEATURE_LEVEL tmpFeatureLvl;

	// hresult for device creation
	HRESULT deviceCreation;

	// iterate over all sufficient driver types
	for (int i = 0; i < D3D_DRIVER_TYPE_COUNT; i++) {
		// we 
		// need
		// MORE
		// PAARAAAAMETERS
		deviceCreation = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPES[i],
			nullptr,
			0,
			D3D_VERSIONS,
			D3D_VERSION_COUNT,
			D3D11_SDK_VERSION,
			&mDxDevice,
			&tmpFeatureLvl,
			&mDxDeviceContext);

		// we have successfully gotten some device, abort mission
		if (SUCCEEDED(deviceCreation))
			break;
	}

	if (FAILED(deviceCreation))
		throw std::runtime_error("could not create dx device");
}

DdApiScreenshotProvider::~DdApiScreenshotProvider() {
	if (mDxDevice) {
		mDxDevice->Release();
		mDxDevice = nullptr;
	}
		
	if (mDxDeviceContext) {
		mDxDeviceContext->Release();
		mDxDeviceContext = nullptr;
	}

	for (DdApiScreen* s : mScreenVector)
		delete s;
}

void DdApiScreenshotProvider::addScreenHandler(int screenNumber, Rotation rotation) {
	mScreenVector.push_back(new DdApiScreen(mDxDevice, rotation, screenNumber));
}

void DdApiScreenshotProvider::takeScreenshot() {
	// panic: if we dont have a single ScreenHandler, this house of cards will come crashing down
	if (mScreenVector.size() == 0)
		// "safe default" since 0 is main screen
		addScreenHandler(0, Rotation::CounterClockWise0);

	for (unsigned int i = 0; i < mScreenVector.size(); i++)
		mScreenVector.at(i)->takeScreenshot(mDxDeviceContext);
}

float DdApiScreenshotProvider::getScreenCrop(Image& result, const Geometry& d) {
	// benchmarking
	clock_t start = clock();
	
	std::shared_ptr<Image> img = mScreenVector[0]->getScreenshot(mDxDeviceContext, d);

	// crop out the relevant part
	if (img->is_empty())
		result = Image();
	else
		result = img->get_crop(d.left(), d.top(), d.right(), d.bottom());

	// benchmarking end
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}
