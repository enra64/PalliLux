#ifndef RGBCONSTRUCTOR_H
#define RGBCONSTRUCTOR_H

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/image.h>

#include <cstdint>
#include <memory>

#include "dimensions.h"
#include "borderprovider.h"

class RgbConstructor
{
public:
    RgbConstructor(){}
    /**
     * @brief takeAndParseScreenShot
     * @param screenShotter
     * @param resultSpace
     */
    float takeAndParseScreenShot(uint8_t *resultSpace);

private:
    /**
     * @brief parseScreenshot parse a screenshot, beginning
     * @param rightBorder image data for right border
     * @param topBorder image data for top border
     * @param leftBorder image data for left border
     * @param bottomBorder image data for bottom border
     * @param resultSpace resulting buffer; if this is not big enough to hold all led data (see LED_DATA_BYTE_COUNT), you will have problems
     */
    float parseScreenshot(uint8_t* resultSpace);
    /**
     * @brief alignBorderImages take the borders, and bring them into the correct order, fusing them at the edges
     * @param rightBorder image data for right border
     * @param topBorder image data for top border
     * @param leftBorder image data for left border
     * @param bottomBorder image data for bottom border
     * @return pointer to the new, aligned image.
     */
    std::unique_ptr<Magick::Image> alignBorders();

    /**
     * @brief scaleImages Scale down the image data to a flat line as wide as we have leds to save on processing time
     * @param rightBorder image data for right border
     * @param topBorder image data for top border
     * @param leftBorder image data for left border
     * @param bottomBorder image data for bottom border
     */
    void flattenBorders();

    /**
     * @brief convertToPixels parse the border line to a set of rgb data
     * @param lineBorder an image, as wide as we have leds, and a single pixel high
     * @param result buffer space for the resulting data, must be at least LED_DATA_BYTE_COUNT
     */
    void imageToRgb(std::unique_ptr<Magick::Image> lineBorder, uint8_t *result);

    void debugSaveBorders();

    /**
     * @brief mHorizontalLedCount amount of leds on each horizontal border
     */
    const static unsigned int HORIZONTAL_LED_COUNT = 30;

    /**
     * @brief mVerticalLedCount amount of leds on each vertical border
     */
    const static unsigned int VERTICAL_LED_COUNT = 20;

    /**
     * @brief mLedCount overall led count in use
     */
    const static unsigned int LED_COUNT = VERTICAL_LED_COUNT * 2 + HORIZONTAL_LED_COUNT * 2;

    /**
     * @brief LED_DATA_BYTE_COUNT amount of bytes needed for the led data
     */
    const static unsigned int LED_DATA_BYTE_COUNT = 3 * LED_COUNT;

    /**
     * @brief mVerticalLedGeometry imagemagick expects geometry objects for scaling; this is the horizontal geometry
     */
    const Magick::Geometry mHorizontalLedGeometry = Magick::Geometry(HORIZONTAL_LED_COUNT, 1);

    /**
     * @brief mVerticalLedGeometry imagemagick expects geometry objects for scaling; this is the vertical geometry
     */
    const Magick::Geometry mVerticalLedGeometry = Magick::Geometry(1, VERTICAL_LED_COUNT);

    /**
     * @brief mScreenShotter class instance used to create the images of each border
     */
    BorderProvider mBorderProvider;

    /**  @name images
     *   Magick++-images used to store the borders while processing them
     */
    ///@{
    Magick::Image mRightImage, mTopImage, mLeftImage, mBottomImage;
    ///@}
};

#endif // RGBCONSTRUCTOR_H
