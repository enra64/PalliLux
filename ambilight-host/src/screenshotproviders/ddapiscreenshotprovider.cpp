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

	for (ScreenHandler* s : mScreenHandlerVector)
		delete s;
}

void DdApiScreenshotProvider::addScreenHandler(int screenNumber, Rotation rotation) {
	mRotationVector.push_back(rotation);
	mScreenHandlerVector.push_back(new ScreenHandler(mDxDevice, screenNumber));
	
	// pushes back an empty image as fallback -> when first image creation fails, this shit goes down
	mFallbackImageVector.push_back(shared_ptr<Image>(new Image()));
	
	// push back how old this fallback image is
	mFallBackImageAgeVector.push_back(FALLBACK_IMAGE_INVALID);
}

void DdApiScreenshotProvider::takeScreenshot() {
	// panic: if we dont have a single ScreenHandler, this house of cards will come crashing down
	if (mRotationVector.size() == 0)
		// "safe default" since 0 is main screen
		addScreenHandler(0, Rotation::CounterClockWise0);

	// reset main image
	mImage.clear();
	
	for (int i = 0; i < mScreenHandlerVector.size(); i++) {
		// if this image does not have a fallback, we force waiting until it has - that
		// looks like shit, but the program doesn't go down
		shared_ptr<Image> img = mScreenHandlerVector.at(i)->getScreenshot(mDxDeviceContext);
		
		// screenshot success!
		if (img) {
			// append new screenshot to mImage
			mImage.append(img->rotate(static_cast<float>(mRotationVector.at(i))));
			
			// if the fallback image is not too old or invalid, just rotate the image
			if (mFallBackImageAgeVector[i] >= 0 && mFallBackImageAgeVector[i] < 100) {
				// fallback image is now older
				mFallBackImageAgeVector[i]++;
			}
			// the fallback image must be refreshed
			else {
				// save fallback image
				mFallbackImageVector[i] = img;
				// update age to zero
				mFallBackImageAgeVector[i] = 0;
			}
		}
		// the screenshot was not successfully taken - use the fallback image
		else {
			if (mFallBackImageAgeVector[i] == FALLBACK_IMAGE_INVALID) {
				// force retaking the screenshot
				i--;
				continue;
			}
				
			mImage.append(*mFallbackImageVector[i]);
		}
	}
}

float DdApiScreenshotProvider::getScreenCrop(Image& result, const Geometry& d) {
	// benchmarking
	clock_t start = clock();

	assert(!mImage.is_empty());

	// TODO: this is somewhat fucked up if you have more than one monitor, as currently only the center one will be captured
	result = mImage.get_crop(d.left(), d.top(), d.right(), d.bottom());

	// benchmarking end
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

bool DdApiScreenshotProvider::getDxDevice()
{
	

	return false;
}
