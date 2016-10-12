#ifndef DD_API_SCREEN_H
#define DD_API_SCREEN_H

// define our custom plugin to allow easier loading of interleaved BGR-or-BGRA-in-whatever-order images
#define cimg_plugin1 "load_custom_channel_order.h"

// display support is disabled in CImg under unix to avoid including X11 here, which would break building with Qt. (line 266)
#include "CImg.h"

#include <d3d11.h>
#include <dxgi1_2.h>

#include <memory>
#include <assert.h>

/// \brief An enum describing the possible screen rotations
enum struct Rotation {
	CounterClockWise0 = 0,
	CounterClockWise90 = 90,
	CounterClockWise180 = 180,
	CounterClockWise270 = 270
};

/// \brief DdApiScreen handles taking screenshots from a single screen using the desktop duplication api
class DdApiScreen {
	// variables for handling screenshots
private:
	/// \brief at what rotation the desktop duplication api will send this image
	Rotation mScreenRotation;

	/// \brief fallback image should the image creation fail
	std::shared_ptr<Image> mFallbackImage;

	// dx stuff
private:
	/// \brief the object enabling the desktop duplication
	IDXGIOutputDuplication* mDesktopDuplication;

	/// \brief dx device connection
	ID3D11Device* mDxDevice;

	/// \brief description of the dxgi output
	DXGI_OUTPUT_DESC mDxgiOutputDescription;

	/// \brief which screen does this ScreenHandler handle
	const unsigned int mScreenNumber;

	/// \brief stores the screenshot last taken as a dx texture
	D3D11_MAPPED_SUBRESOURCE mLastScreenshotMappedSubresource;

	/// \brief stores the texture of the last screenshot taken
	ID3D11Texture2D* mLastScreenshotTexture;

	/// \brief stores the texture description of the last screenshot taken
	D3D11_TEXTURE2D_DESC mLastScreenshotTextureDescription;

	// public interface
public:
	DdApiScreen(ID3D11Device* dxDevice, Rotation r, unsigned int screenNumber) : mDxDevice(dxDevice), mScreenRotation(r), mScreenNumber(screenNumber) {
		// increase reference count
		mDxDevice->AddRef();

		// get dxgi device
		IDXGIDevice* dxgiDevice = nullptr;
		HRESULT dxgiDeviceQuery = mDxDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));

		// check success
		if (FAILED(dxgiDeviceQuery)) 
			throw std::runtime_error("could not get dxgi device");

		// get dxgi adapter
		IDXGIAdapter* dxgiAdapter = nullptr;
		HRESULT dxgiAdapterQuery = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));

		// release dxgi device after getting the adapter
		dxgiDevice->Release();
		dxgiDevice = nullptr;

		// check success
		if (FAILED(dxgiAdapterQuery)) throw std::runtime_error("could not get parent dxgi adapter");

		// Get output
		IDXGIOutput* dxgiOutput = nullptr;
		HRESULT dxgiOutputQuery = dxgiAdapter->EnumOutputs(mScreenNumber, &dxgiOutput);

		// release dxgi adapter after getting the output
		dxgiAdapter->Release();
		dxgiAdapter = nullptr;

		// check for success
		if (FAILED(dxgiOutputQuery)) throw std::runtime_error("could not get specified output");

		// save the output description
		dxgiOutput->GetDesc(&mDxgiOutputDescription);

		// QI for Output 1
		IDXGIOutput1* DxgiOutput1 = nullptr;
		dxgiOutputQuery = dxgiOutput->QueryInterface(__uuidof(DxgiOutput1), reinterpret_cast<void**>(&DxgiOutput1));

		// release first dxgi output
		dxgiOutput->Release();
		dxgiOutput = nullptr;

		// check for second dxgi output query success
		if (FAILED(dxgiOutputQuery)) throw std::runtime_error("failed to get second dxgi output");

		// Create desktop duplication object
		HRESULT duplicationResult = DxgiOutput1->DuplicateOutput(mDxDevice, &mDesktopDuplication);
		DxgiOutput1->Release();
		DxgiOutput1 = nullptr;

		if (FAILED(duplicationResult)) {
			if (duplicationResult == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE)
				throw std::runtime_error("maximum dd api connections reached, cannot connect");
			throw std::runtime_error("failed to duplicate output");
		}
	}

	~DdApiScreen() {
		mDxDevice->Release();
	}

	/// \brief take the screenshot, extract the pixels to member variables, and return
	void takeScreenshot(ID3D11DeviceContext* dxDeviceContext) {
		// if we previously took a screenshot, we have to release it now
		if (mLastScreenshotTexture) {
			dxDeviceContext->Unmap(mLastScreenshotTexture, 0);
			mLastScreenshotTexture->Release();
		}

		// begin taking screenshot
		IDXGIResource* dxgiDesktopResource = nullptr;
		DXGI_OUTDUPL_FRAME_INFO FrameInfo;

		// Get new frame
		HRESULT duplicationResult = mDesktopDuplication->AcquireNextFrame(10, &FrameInfo, &dxgiDesktopResource);

		if (FAILED(duplicationResult))
			return;

		ID3D11Texture2D* screenshotTexture;
		
		// query for IDXGIResource
		HRESULT desktopResult = dxgiDesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&screenshotTexture));
		dxgiDesktopResource->Release();
		dxgiDesktopResource = nullptr;
		if (FAILED(desktopResult)) throw new std::runtime_error("failed to QueryInterface for screenshot");

		// retrieve the texture data into our mapped result structure
		mLastScreenshotMappedSubresource = retrieveTexture(dxDeviceContext, screenshotTexture);

		// release texture & duplication frame data
		HRESULT desktopDuplicationFrameRelease = mDesktopDuplication->ReleaseFrame();
		if (FAILED(desktopDuplicationFrameRelease)) throw std::runtime_error("could not release last frame");
		if (screenshotTexture)
			screenshotTexture->Release();
	}

	/// \brief create a CImg from the member variables
	std::shared_ptr<Image> getScreenshot(ID3D11DeviceContext* dxDeviceContext, Geometry g) {
		return getImageFromTexture(dxDeviceContext, g);
	}

	// functions for handling screenshots
private:
	D3D11_MAPPED_SUBRESOURCE retrieveTexture(ID3D11DeviceContext* dxDeviceContext, ID3D11Texture2D* inputTexture) {
		// get input texture information
		D3D11_TEXTURE2D_DESC inputDesc;
		inputTexture->GetDesc(&inputDesc);

		// create new texture information
		ID3D11Texture2D* copiedTexture;
		// memset for when you just dont give a f*ck any more about f*cking macros like zeromemory
		::memset((void*)&mLastScreenshotTextureDescription, 0, sizeof(mLastScreenshotTextureDescription));
		mLastScreenshotTextureDescription.Width = inputDesc.Width;
		mLastScreenshotTextureDescription.Height = inputDesc.Height;
		mLastScreenshotTextureDescription.MipLevels = 1;
		mLastScreenshotTextureDescription.ArraySize = 1;
		mLastScreenshotTextureDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		mLastScreenshotTextureDescription.SampleDesc.Count = 1;
		mLastScreenshotTextureDescription.SampleDesc.Quality = 0;
		mLastScreenshotTextureDescription.Usage = D3D11_USAGE_STAGING;
		mLastScreenshotTextureDescription.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		mLastScreenshotTextureDescription.MiscFlags = 0;

		// create new texture
		HRESULT textureCreationCheck = mDxDevice->CreateTexture2D(&mLastScreenshotTextureDescription, NULL, &copiedTexture);
		assert(SUCCEEDED(textureCreationCheck));

		dxDeviceContext->CopyResource(copiedTexture, inputTexture);

		D3D11_MAPPED_SUBRESOURCE  mapResource;
		HRESULT mapCheck = dxDeviceContext->Map(copiedTexture, 0, D3D11_MAP_READ, NULL, &mapResource);
		assert(SUCCEEDED(mapCheck));

		return mapResource;
	}

	/// \brief copy the texture into cpu-usable space, and then create a CImg from it
	std::shared_ptr<Image> getImageFromTexture(ID3D11DeviceContext* dxDeviceContext, Geometry g) {
		std::shared_ptr<Image> result = std::make_shared<Image>();

		// read from screenshot texture
		result->read((char*)mLastScreenshotMappedSubresource.pData, "BGRA", 4, g.width, g.height, 3, nullptr, mLastScreenshotMappedSubresource.RowPitch, g.xOffset, g.yOffset);
		
		// rotate according to the screen rotation
		result->rotate((float)mScreenRotation);

		return result;
	}	
};

#endif