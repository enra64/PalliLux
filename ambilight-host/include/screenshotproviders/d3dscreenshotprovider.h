#ifndef D3DSCREENSHOTPROVIDER_H
#define D3DSCREENSHOTPROVIDER_H

#include "screenshotprovider.h"

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
};

#endif // D3DSCREENSHOTPROVIDER_H
