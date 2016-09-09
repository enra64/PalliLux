#ifndef VLCSCREENSHOT_H
#define VLCSCREENSHOT_H

#include "screenshotprovider.h"

/**
 * @brief An implementation of the Screenshot interface; it reads from a video stream provided by vlc. Not finished!
 */
class VlcScreenshotProvider : public ScreenshotProvider
{
public:
	/**
	 * @brief Crop the requested geometry from the last screenshot
	 * @param result the resulting image
	 * @param d the dimensions the image will be; these are the dimensions cropped from the screenshot
	 * @return the time in seconds required
	 */
	float getScreenCrop(Magick::Image& result, const Magick::Geometry& d) override;

	/**
	 * @brief Read the vlc stream
	 */
	void takeScreenshot() override;
private:
    Magick::Image mLastScreenshot;///< last screenshot taken
    const char* mStreamPath = "/home/arne/stream"; ///< vlc stream input file
};

#endif // VLCSCREENSHOT_H
