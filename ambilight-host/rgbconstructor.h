#ifndef RGBCONSTRUCTOR_H
#define RGBCONSTRUCTOR_H

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/image.h>

#include <cstdint>
#include <memory>

#include "dimensions.h"
#include "xlibborderprovider.h"

class RgbConverter {
public:
    /**
     * @brief RgbConstructor create a new rgb constructor object
     * @param provider the BorderProvider that will be used to capture the screen borders
     * @param verticalLedCount how many leds are on each vertical border
     * @param horizontalLedCount how many leds are on each horizontal border
     */
    RgbConverter(std::shared_ptr<BorderProvider> provider, unsigned int horizontalLedCount, unsigned int verticalLedCount);

    /**
     * @brief takeAndParseScreenShot
     * @param screenShotter
     * @param resultSpace rgb data result buffer; must be as large as
     */
    float takeAndParseScreenShot(uint8_t *resultSpace);

    /**
     * @brief getRequiredBufferLength
     * @return the number of bytes required to buffer the resulting led data
     */
    size_t getRequiredBufferLength() const {
        return 3 * LED_COUNT;
    }

    /**
     * @brief setBrightnessFactor set a factor for all channel brightnesses
     * @param val new factor; values greater than 1 will be interesting...
     */
    void setBrightnessFactor(float val){
        mBrightnessFactor = val;
    }

private:
    /**
     * @brief parseScreenshot parse a screenshot, beginning
     * @param resultSpace resulting buffer; if this is not big enough to hold all led data (see LED_DATA_BYTE_COUNT), you will have problems
     */
    float parseScreenshot(uint8_t* resultSpace);

    /**
     * @brief alignBorderImages take the borders, and bring them into the correct order, fusing them at the edges
     * @return pointer to the new, aligned image.
     */
    std::unique_ptr<Magick::Image> alignBorders();

    /**
     * @brief scaleImages Scale down the image data to a flat line as wide as we have leds to save on processing time
     */
    void flattenBorders();

    /**
     * @brief convertToPixels parse the border line to a set of rgb data
     * @param lineBorder an image, as wide as we have leds, and a single pixel high
     * @param result buffer space for the resulting data, must be at least LED_DATA_BYTE_COUNT
     */
    void imageToRgb(std::unique_ptr<Magick::Image> lineBorder, uint8_t *result);

    /**
     * @brief debugSaveBorders save the current border images to disk
     */
    void debugSaveBorders();

    /**
     * @brief mHorizontalLedCount amount of leds on each horizontal border
     */
    const unsigned int HORIZONTAL_LED_COUNT;

    /**
     * @brief mVerticalLedCount amount of leds on each vertical border
     */
    const unsigned int VERTICAL_LED_COUNT;

    /**
     * @brief mLedCount overall led count in use
     */
    const unsigned int LED_COUNT = VERTICAL_LED_COUNT * 2 + HORIZONTAL_LED_COUNT * 2;

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
    std::shared_ptr<BorderProvider> mBorderProvider;

    /**  @name images
     *   Magick++-images used to store the borders while processing them
     */
    ///@{
    Magick::Image mRightImage, mTopImage, mLeftImage, mBottomImage;
    ///@}
    ///

    /**
     * @brief mBrightnessFactor factor for rgb data brightness regulation
     */
    float mBrightnessFactor = 1.f;
};

#endif // RGBCONSTRUCTOR_H
