#ifndef WINSCREENSHOT_H
#define WINSCREENSHOT_H

#include "screenshotprovider.h"
#include "ddapiscreen.h"

#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <sal.h>
#include <new>
#include <warning.h>
#include <DirectXMath.h>

#include <vector>
#include <memory>

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
	
	/// \brief contains the DdApiHandler for each screen
	std::vector<DdApiScreen*> mScreenVector;

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
