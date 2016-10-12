#ifndef DD_API_SCREEN_H
#define DD_API_SCREEN_H

//#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
//#include <sal.h>
//#include <new>
//#include <warning.h>
//#include <DirectXMath.h>

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

	/// \brief constant value if the fallback image is not valid
	const char FALLBACK_IMAGE_INVALID = -1;

	/// \brief age of the fallback image
	char mFallbackImageAge = FALLBACK_IMAGE_INVALID;



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

	std::shared_ptr<Image> getScreenshot(ID3D11DeviceContext* dxDeviceContext) {
		IDXGIResource* dxgiDesktopResource = nullptr;
		DXGI_OUTDUPL_FRAME_INFO FrameInfo;

		// Get new frame
		HRESULT duplicationResult = mDesktopDuplication->AcquireNextFrame(10, &FrameInfo, &dxgiDesktopResource);

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
		if (screenshot)
			screenshot->Release();

		return result;
	}

	// functions for handling screenshots
private:
	/// \brief copy the texture into cpu-usable space, and then create a CImg from it
	std::shared_ptr<Image> getImageFromTexture(ID3D11Texture2D * inputTexture, ID3D11DeviceContext* dxDeviceContext) {
		// get input texture information
		D3D11_TEXTURE2D_DESC inputDesc;
		inputTexture->GetDesc(&inputDesc);

		// create new texture information
		ID3D11Texture2D* newTexture;
		D3D11_TEXTURE2D_DESC newTextureDesc;
		// memset for when you just dont give a f*ck any more about f*cking macros like zeromemory
		::memset((void*) &newTextureDesc, 0, sizeof(newTextureDesc));
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

		updateFallbackSystem(result);

		result->read((char*)mapResource.pData, "BGRA", 4, newTextureDesc.Width, newTextureDesc.Height, 3, nullptr, mapResource.RowPitch);

		dxDeviceContext->Unmap(newTexture, 0);
		newTexture->Release();

		return result;
	}

	void updateFallbackSystem(std::shared_ptr<Image> latestImage) {
		// screenshot success!
		if (latestImage) {
			// rotate according to the screen rotation
			latestImage->rotate((float) mScreenRotation);

			// if the fallback image is not too old or invalid
			if (mFallbackImageAge >= 0 && mFallbackImageAge < 100) {
				// fallback image is now older
				mFallbackImageAge++;
			}
			// the fallback image must be refreshed
			else {
				// save fallback image
				mFallbackImage = latestImage;
				// update age to zero
				mFallbackImageAge = 0;
			}
		}
		// the screenshot was not successfully taken - use the fallback image
		else {
			latestImage = mFallbackImage;
		}
	}

	
};

#endif