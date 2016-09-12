#ifndef AMBICOLORDATAPROVIDER_H
#define AMBICOLORDATAPROVIDER_H

#include <Magick++.h>

#include <cstdint>
#include <memory>

#include "borderprovider.h"
#include "colordataprovider.h"

/**
 * @brief The AmbiColorDataProvider provides a rgb array derived from screenshots of the borders.
 * It implements the interface defined by ColorDataProvider
 */
class AmbiColorDataProvider : public ColorDataProvider {
public:
    /**
     * @brief create a new AmbiColorDataProvider
     * @param provider the BorderProvider that will be used to capture the screen borders
     * @param verticalLedCount how many leds are on each vertical border
     * @param horizontalLedCount how many leds are on each horizontal border
     */
    AmbiColorDataProvider(unsigned int horizontalLedCount, unsigned int verticalLedCount);

    /**
     * @brief Retrieves border images and provides rgb output data
     * @param[out] resultSpace Storage provided for the rgb output data; length must be at least the value of "::"<getRequiredBufferLength>
     */
    float getData(uint8_t* resultSpace) override;

    /**
     * @brief This function gives ownership of the last line of pixels used for color extraction as a Magick::Image.
     *
     * This data may be used for debugging, but is also nice to look at in runtime.
     */
    std::unique_ptr<Magick::Image> getLastLineImage() {
        return move(mLastLineImage);
    }

    /**
     * @brief Set the BorderProvider instance used for capturing border images.
     */
    void setBorderProvider(std::shared_ptr<BorderProvider> provider) {
        mBorderProvider = provider;
    }

    /**
     * @brief Retrieve the currently used BorderProvider
     */
    std::shared_ptr<BorderProvider> getBorderProvider() const {
        return mBorderProvider;
    }

private:
    // functions
    /**
     * @brief Rotate the border images to align them for fusing to a line of pixels
     * @return pointer to the new, aligned image.
     */
    std::unique_ptr<Magick::Image> alignBorders();

    /**
     * @brief Scale down the image data to a line of pixels as long as we have leds
     */
    void flattenBorders();

    /**
     * @brief Read the rgb data from the one-pixel-high-image
     * @param lineBorder an image with a width equal to the number of leds, and a height of 1.
     * @param result buffer space for the resulting data. size requirement may be requested with getRequiredBufferLength()
     */
    void imageToRgb(std::unique_ptr<Magick::Image> lineBorder, uint8_t* result);

    /**
     * @brief save the current border images to disk
     */
    void debugSaveBorders();

    //members
    /**  @name images
     *   Magick++-images used to store the borders while processing them
     */
    ///@{
    Magick::Image mRightImage, mTopImage, mLeftImage, mBottomImage;
    ///@}

    std::unique_ptr<Magick::Image> mLastLineImage;///< last complete border image, scaled down

    Magick::Geometry mHorizontalLedGeometry;///< horizontal scaling target geometry

    Magick::Geometry mVerticalLedGeometry; ///< vertical scaling target geometry

    std::shared_ptr<BorderProvider> mBorderProvider; ///< class instance used to create the images of each border
};

#endif // AMBICOLORDATAPROVIDER_H
