#ifndef RGBCONSTRUCTOR_H
#define RGBCONSTRUCTOR_H

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/image.h>

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "datafilter.h"
#include "borderprovider.h"

/**
 * @brief The RgbConverter class is the link between the BorderProvider images and raw RGB data for the arduino
 */
class RgbConverter {
public:
    /**
     * @brief create a new rgb constructor object
     * @param provider the BorderProvider that will be used to capture the screen borders
     * @param verticalLedCount how many leds are on each vertical border
     * @param horizontalLedCount how many leds are on each horizontal border
     */
    RgbConverter(std::shared_ptr<BorderProvider> provider, unsigned int horizontalLedCount, unsigned int verticalLedCount);

    /**
     * @brief Retrieves border images and provides rgb output data
     * @param[out] resultSpace Storage provided for the rgb output data; length must be at least the value of "::"<getRequiredBufferLength>
     */
    float takeAndParseScreenShot(uint8_t *resultSpace);

    /**
     * @brief Access the required result buffer length
     * @return the number of bytes required to buffer the resulting led data
     */
    size_t getRequiredBufferLength() const {
        return 3 * LED_COUNT;
    }

    /**
     * @brief Set a factor for all channel brightnesses
     * @param val new factor; values greater than 1 will be interesting...
     */
    void setBrightnessFactor(float val) {
        mBrightnessFactor = val;
    }

private:
// functions
    /**
     * @brief take the borders, and bring them into the correct order, fusing them at the edges
     * @return pointer to the new, aligned image.
     */
    std::unique_ptr<Magick::Image> alignBorders();

    /**
     * @brief Scale down the image data to a flat line as wide as we have leds to save on processing time
     */
    void flattenBorders();

    /**
     * @brief parse the border line to a set of rgb data
     * @param lineBorder an image, as wide as we have leds, and a single pixel high
     * @param result buffer space for the resulting data, must be at least LED_DATA_BYTE_COUNT
     */
    void imageToRgb(std::unique_ptr<Magick::Image> lineBorder, uint8_t *result);

    /**
     * @brief save the current border images to disk
     */
    void debugSaveBorders();

    /**
     * @brief Apply all filters set for this RgbConverter.
     */
    void applyFilters();

//members
    /**  @name images
     *   Magick++-images used to store the borders while processing them
     */
    ///@{
    Magick::Image mRightImage, mTopImage, mLeftImage, mBottomImage;
    ///@}

    const unsigned int HORIZONTAL_LED_COUNT;///!< amount of leds on each horizontal border

    const unsigned int VERTICAL_LED_COUNT;///!< amount of leds on each vertical border

    const unsigned int LED_COUNT = VERTICAL_LED_COUNT * 2 + HORIZONTAL_LED_COUNT * 2; ///!< aggregateled count

    const Magick::Geometry mHorizontalLedGeometry = Magick::Geometry(HORIZONTAL_LED_COUNT, 1);///!< horizontal scaling target geometry

    const Magick::Geometry mVerticalLedGeometry = Magick::Geometry(1, VERTICAL_LED_COUNT); ///!< vertical scaling target geometry

    std::shared_ptr<BorderProvider> mBorderProvider; ///!< class instance used to create the images of each border

    float mBrightnessFactor = 1.f;///!< factor for rgb data brightness regulation

    std::unordered_map<std::string, std::unique_ptr<DataFilter>> mDataFilters;///!< List of DataFilter objects that will be applied to the rgb data
};

#endif // RGBCONSTRUCTOR_H
