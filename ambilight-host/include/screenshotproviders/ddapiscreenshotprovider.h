#ifndef WINSCREENSHOT_H
#define WINSCREENSHOT_H

#include "screenshotprovider.h"

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
private:
	// storage space for the image
	Image mImage;
};

#endif // WINSCREENSHOT_H
