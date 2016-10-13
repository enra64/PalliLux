#ifndef BORDERPROVIDER_H
#define BORDERPROVIDER_H

#include "screenshotprovider.h"

#include <memory>

/**
 * @brief Implementations encapsulate all knowledge about the screen configuration, providing only screen captures
 *        of the right, top, left and bottom screen borders.
 */
class BorderProvider {
public:
    /**
     * @brief Empty virtual destructor to ensure derived classes will be properly destructed.
     */
    virtual ~BorderProvider() {
    }

    /**
     * @brief This function captures each screen border into a Magick++ image. The size of the returned images does not matter
     */
    virtual void retrieveBorders(Image& right, Image& top, Image& left, Image& bottom) = 0;

    /**
     * @brief Retrieve the current border width in pixels
     */
    size_t getBorderWidth() const {
        return mBorderWidth;
    }

    /**
     * @brief Set the ScreenshotProvider used for capturing the screen.
     */
    void setScreenshotProvider(std::shared_ptr<ScreenshotProvider> provider) {
        mScreenshotProvider = provider;
    }

    /**
     * @brief Retrieve the currently used ScreenshotProvider
     */
    std::shared_ptr<ScreenshotProvider> getScreenshotProvider() {
        return mScreenshotProvider;
    }

    /**
     * @brief Handles border width updates
     * @param width new width in pixels
     */
    virtual void setBorderWidth(size_t width) {
        mBorderWidth = width;
        updateGeometry();
    }

protected:
    /**
     * @brief This function updates the screenshot geometry when a parameter (for example the border width) has changed
     */
    virtual void updateGeometry() = 0;
    std::shared_ptr<ScreenshotProvider> mScreenshotProvider;///< the ScreenshotProvider to be used
    size_t mBorderWidth = 100;///< how wide should the borders be
};

#endif // BORDERPROVIDER_H
