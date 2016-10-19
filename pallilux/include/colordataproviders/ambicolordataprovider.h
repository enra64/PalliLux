#ifndef AMBICOLORDATAPROVIDER_H
#define AMBICOLORDATAPROVIDER_H

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
     * @param d how many leds are on which border
     */
    AmbiColorDataProvider(LedConfig d);

    /**
     * @brief Retrieves border images and provides rgb output data
     * @param[out] resultSpace Storage provided for the rgb output data; length must be at least the value of "::"<getRequiredBufferLength>
     */
    float getData(uint8_t* resultSpace) override;

    /**
     * @brief This function gives ownership of the last line of pixels used for color extraction as a Image.
     *
     * This data may be used for debugging, but is also nice to look at in runtime.
     */
    std::unique_ptr<Image> getLastLineImage() {
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

    /**
     * @brief Enum to de-magic-numberize the cimg interpolation type in use
     */
    enum struct CImgInterpolationType {
        RawMemResize = -1,
        FillUsingBoundaryConditions = 0,
        NearestNeighbor = 1,
        MovingAverage = 2,
        LinearInterpolation = 3,
        GridInterpolation = 4,
        CubicInterpolation = 5,
        LanczosInterpolation = 6
    };

    /**
     * @brief Set the resize interpolation mode
     */
    void setResizeInterpolationMode(CImgInterpolationType mode){
        mInterpolationType = mode;
    }

    /**
     * @brief Get the currently used resize interpolation mode
     */
    CImgInterpolationType getResizeInterpolationMode(){
        return mInterpolationType;
    }


private:
    // functions
    /**
     * @brief Rotate the border images to align them for fusing to a line of pixels
     * @return pointer to the new, aligned image.
     */
    std::unique_ptr<Image> alignBorders();

    /**
     * @brief Scale down the image data to a line of pixels as long as we have leds
     */
    void flattenBorders();

    /**
     * @brief Read the rgb data from the one-pixel-high-image
     * @param lineBorder an image with a width equal to the number of leds, and a height of 1.
     * @param result buffer space for the resulting data. size requirement may be requested with getRequiredBufferLength()
     */
    void imageToRgb(std::unique_ptr<Image> lineBorder, uint8_t* result);

    /**
     * @brief save the current border images to disk
     */
    void debugSaveBorders();

    //members
    CImgInterpolationType mInterpolationType = CImgInterpolationType::LanczosInterpolation;///< the CImg interpolation type used for resizing the border images

    /**  @name images
     *   Magick++-images used to store the borders while processing them
     */
    ///@{
    Image mRightImage, mTopImage, mLeftImage, mBottomImage;
    ///@}

    std::unique_ptr<Image> mLastLineImage;///< last complete border image, scaled down

	Geometry mBottomLedGeometry;///< bottom led row length, basically
	Geometry mRightLedGeometry;///< right led row length, basically
	Geometry mTopLedGeometry;///< top led row length, basically
	Geometry mLeftLedGeometry;///< left led row length, basically

    bool mCounterClockWise;///< whether the led data is pushed clock- or counterclockwise

    std::shared_ptr<BorderProvider> mBorderProvider; ///< class instance used to create the images of each border
};

#endif // AMBICOLORDATAPROVIDER_H
