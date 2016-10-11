#ifndef WINSCREENSHOT_H
#define WINSCREENSHOT_H

#include "screenshotprovider.h"

#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <sal.h>
#include <new>
#include <warning.h>
#include <DirectXMath.h>

#include <vector>
#include <memory>

enum struct Rotation {
	CounterClockWise0 = 0,
	CounterClockWise90 = 90,
	CounterClockWise180 = 180,
	CounterClockWise270 = 270
};

/// \brief the class that abstracts handling each screen seperately with dd api
class ScreenHandler {
public:
	ScreenHandler(ID3D11Device* dxDevice, unsigned int screenNumber) : mDxDevice(dxDevice), mScreenNumber(screenNumber) {
		// increase reference count
		mDxDevice->AddRef();

		// get dxgi device
		IDXGIDevice* dxgiDevice = nullptr;
		HRESULT dxgiDeviceQuery = mDxDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));

		// check success
		if (FAILED(dxgiDeviceQuery)) throw std::runtime_error("could not get dxgi device");

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

	~ScreenHandler() {
		mDxDevice->Release();
	}

	/// \brief copy the texture into cpu-usable space, and then create a CImg from it
	std::shared_ptr<Image> getImageFromTexture(ID3D11Texture2D * inputTexture, ID3D11DeviceContext* dxDeviceContext){
		// get input texture information
		D3D11_TEXTURE2D_DESC inputDesc;
		inputTexture->GetDesc(&inputDesc);

		// create new texture information
		ID3D11Texture2D* newTexture;
		D3D11_TEXTURE2D_DESC newTextureDesc;
		ZeroMemory(&newTextureDesc, sizeof(newTextureDesc));
		newTextureDesc.Width = inputDesc.Width;
		newTextureDesc.Height = inputDesc.Height;
		newTextureDesc.MipLevels = 1;
		newTextureDesc.ArraySize = 1;
		newTextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		newTextureDesc.SampleDesc.Count = 1;
		newTextureDesc.SampleDesc.Quality = 0;
		newTextureDesc.Usage = D3D11_USAGE_STAGING;
		newTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		newTextureDesc.MiscFlags = 0;

		// create new texture
		HRESULT textureCreationCheck = mDxDevice->CreateTexture2D(&newTextureDesc, NULL, &newTexture);
		assert(SUCCEEDED(textureCreationCheck));

		dxDeviceContext->CopyResource(newTexture, inputTexture);

		D3D11_MAPPED_SUBRESOURCE  mapResource;
		HRESULT mapCheck = dxDeviceContext->Map(newTexture, 0, D3D11_MAP_READ, NULL, &mapResource);

		assert(SUCCEEDED(mapCheck));

		std::shared_ptr<Image> result = std::make_shared<Image>();

		result->read((char*)mapResource.pData, "BGRA", 4, newTextureDesc.Width, newTextureDesc.Height, 3, nullptr, mapResource.RowPitch);
		
		dxDeviceContext->Unmap(newTexture, 0);
		newTexture->Release();

		return result;
	}

	std::shared_ptr<Image> getScreenshot(ID3D11DeviceContext* dxDeviceContext) {
		IDXGIResource* dxgiDesktopResource = nullptr;
		DXGI_OUTDUPL_FRAME_INFO FrameInfo;

		// Get new frame
		HRESULT duplicationResult = mDesktopDuplication->AcquireNextFrame(500, &FrameInfo, &dxgiDesktopResource);
		//if (duplicationResult == DXGI_ERROR_WAIT_TIMEOUT)
		//	return;
		
		if (FAILED(duplicationResult))
			return nullptr;

		// screenshot as a dx 11 texture
		ID3D11Texture2D* screenshot;

		// query for IDXGIResource
		HRESULT desktopResult = dxgiDesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&screenshot));
		dxgiDesktopResource->Release();
		dxgiDesktopResource = nullptr;
		if (FAILED(desktopResult)) throw new std::runtime_error("failed to QueryInterface for screenshot");

		std::shared_ptr<Image> result = getImageFromTexture(screenshot, dxDeviceContext);

		// release data
		HRESULT desktopDuplicationFrameRelease = mDesktopDuplication->ReleaseFrame();
		if (FAILED(desktopDuplicationFrameRelease)) throw std::runtime_error("could not release last frame");
		screenshot->Release();
		
		return result;
	}
private:
	/// \brief the object enabling the desktop duplication
	IDXGIOutputDuplication* mDesktopDuplication;
	
	/// \brief dx device connection
	ID3D11Device* mDxDevice;

	/// \brief description of the dxgi output
	DXGI_OUTPUT_DESC mDxgiOutputDescription;

	/// \brief which screen does this ScreenHandler handle
	unsigned int mScreenNumber;
};

/**
* @brief An implementation of the Screenshot interface for windows
*/
class DdApiScreenshotProvider : public ScreenshotProvider {
public:
	DdApiScreenshotProvider();
	~DdApiScreenshotProvider();

	void takeScreenshot();

	/**
	* @brief takeScreenshot Take a screenshot, converting it to a Magick++ image
	* @param result the resulting image
	* @param d the dimensions the image will be; these are the dimensions requested from X
	* @return the time in seconds required
	*/
	float getScreenCrop(Image& result, const Geometry& d) override;

	// custom stuff
private:
	/// \brief storage space for the whole image
	Image mImage;
	
	// ScreenHandler stuff
private:
	/// \brief add a ScreenHandler
	void addScreenHandler(int screenNumber, Rotation rotation);
	
	/// \brief contains the rotation for each screen
	std::vector<Rotation> mRotationVector;

	/// \brief contains the ScreenHandler for each screen
	std::vector<ScreenHandler*> mScreenHandlerVector;
	
	/// \brief contains the last successfully taken image for each ScreenHandler
	std::vector<std::shared_ptr<Image>> mFallbackImageVector;

	std::vector<char> mFallBackImageAgeVector;

	const char FALLBACK_IMAGE_INVALID = -1;

	// dx stuff
private:
	ID3D11Device* mDxDevice = nullptr;
	ID3D11DeviceContext* mDxDeviceContext = nullptr;

	/// \brief navigate through the parameter maze of D3D11CreateDevice to get us a dx device; returns true upon success
	bool getDxDevice();

	// static const dx stuff
private:
	/// \brief number of supported d3d driver types
	static const uint8_t D3D_DRIVER_TYPE_COUNT = 3;

	/// \brief A best-to-worst sorted array of direct x driver types
	const D3D_DRIVER_TYPE D3D_DRIVER_TYPES[D3D_DRIVER_TYPE_COUNT] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	/// \brief number of supported d3d versions
	static const unsigned char D3D_VERSION_COUNT = 4;

	/// \brief A best-to-worst sorted array of direct x feature levels
	const D3D_FEATURE_LEVEL D3D_VERSIONS[D3D_VERSION_COUNT] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1,
	};
};

#endif // WINSCREENSHOT_H
