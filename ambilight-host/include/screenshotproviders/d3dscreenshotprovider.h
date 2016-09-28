#ifndef D3DSCREENSHOTPROVIDER_H
#define D3DSCREENSHOTPROVIDER_H

#include "screenshotprovider.h"

#include <d3d9.h>
#include <d3dx9.h>

/**
* @brief An implementation of the Screenshot interface for windows
*/
class D3DScreenshotProvider : public ScreenshotProvider {
public:
	D3DScreenshotProvider();
	~D3DScreenshotProvider();

	void takeScreenshot();

	/**
	* @brief takeScreenshot Take a screenshot, converting it to a Magick++ image
	* @param result the resulting image
	* @param d the dimensions the image will be; these are the dimensions requested from X
	* @return the time in seconds required
	*/
	float getScreenCrop(Image& result, const Geometry& d) override;
private:
	Image mImage;
	uint8_t* mImageBuffer = nullptr;

	IDirect3D9*			g_pD3D = nullptr;
	IDirect3DDevice9*	g_pd3dDevice = nullptr;
	IDirect3DSurface9*	g_pSurface = nullptr;
};

#endif // D3DSCREENSHOTPROVIDER_H
