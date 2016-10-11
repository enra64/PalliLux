#include "ddapiscreenshotprovider.h"

#include <ctime>
#include <iostream>
#include <assert.h>

using namespace cimg_library;
using namespace std;

DdApiScreenshotProvider::DdApiScreenshotProvider() {
	// initialize direct x device and connection
	getDxDevice();


	HDESK CurrentDesktop = nullptr;
	while(!CurrentDesktop)
		CurrentDesktop = OpenInputDesktop(0, FALSE, GENERIC_ALL);
	
	// Attach desktop to this thread
	bool DesktopAttached = SetThreadDesktop(CurrentDesktop) != 0;
	
	CloseDesktop(CurrentDesktop);
	CurrentDesktop = nullptr;
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

	for (ScreenHandler* s : mScreenHandlerList)
		delete s;
}

void DdApiScreenshotProvider::addScreenHandler(int screenNumber, Rotation rotation) {
	mRotationList.push_back(rotation);
	mScreenHandlerList.push_back(new ScreenHandler(mDxDevice, screenNumber));
}

void DdApiScreenshotProvider::takeScreenshot() {
	// panic: if we dont have a single ScreenHandler, this house of cards will come crashing down
	if (mRotationList.size() == 0)
		// "safe default" since 0 is main screen
		addScreenHandler(0, Rotation::CounterClockWise0);

	// reset main image
	mImage.clear();
	
	for (int i = 0; i < mScreenHandlerList.size(); i++) {
		shared_ptr<Image> img = mScreenHandlerList.at(i)->getScreenshot(mDxDeviceContext);
		if (img) {
			mImage.append(img->rotate(static_cast<float>(mRotationList.at(i))));
		}
	}

	
}

float DdApiScreenshotProvider::getScreenCrop(Image& result, const Geometry& d) {
	// benchmarking
	clock_t start = clock();

	// TODO: this is somewhat fucked up if you have more than one monitor, as currently only the center one will be captured
	result = mImage.get_crop(d.left(), d.top(), d.right(), d.bottom());

	// benchmarking end
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

bool DdApiScreenshotProvider::getDxDevice()
{
	// d3d feature level nobody cares about
	D3D_FEATURE_LEVEL tmpFeatureLvl;

	// iterate over all sufficient driver types
	for (int i = 0; i < D3D_DRIVER_TYPE_COUNT; i++) {
		// we 
		// need
		// MORE
		// PAARAAAAMETERS
		HRESULT deviceCreation = D3D11CreateDevice(
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

		// we have successfully gotten some device
		if (SUCCEEDED(deviceCreation))
			return true;
	}

	return false;
}
