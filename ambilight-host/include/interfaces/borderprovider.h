#ifndef BORDERPROVIDER_H
#define BORDERPROVIDER_H

#include "Magick++.h"

#include "screenshot.h"

#include <memory>

/**
 * @brief Implementations encapsulate all screen information, providing only border images.
 */
class BorderProvider
{
public:
	virtual ~BorderProvider()
	{
	}

	/**
	 * @brief This function must capture each screen border into a Magick++ image
	 */
	virtual void retrieveBorders(Magick::Image& right, Magick::Image& top, Magick::Image& left, Magick::Image& bottom) = 0;

	size_t getBorderWidth() const
	{
		return mBorderWidth;
	}

    void setScreenshotProvider(std::shared_ptr<Screenshot> provider){
        mScreenshot = provider;
    }

    std::shared_ptr<Screenshot> getScreenshotProvider() {
        return mScreenshot;
    }

	virtual void setBorderWidth(size_t width)
	{
		mBorderWidth = width;
		updateGeometry();
	}

protected:
	virtual void updateGeometry() = 0;
	std::shared_ptr<Screenshot> mScreenshot;
	size_t mBorderWidth = 100;///!< how wide should the borders be
};

#endif // BORDERPROVIDER_H
