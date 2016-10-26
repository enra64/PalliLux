#include "saturationfilter.h"

#include <cmath>

/**
 * @brief Construct a SaturationFilter with the given factor
 * @param factor the float factor that will be applied.
 */
SaturationFilter::SaturationFilter(float factor) : mFactor(factor)
{
}

uint8_t SaturationFilter::clamp(float value)
{
    if(value > 255)
        return 255;
    else if(value < 0)
        return 0;
    else
        return static_cast<uint8_t>(value);
}

void SaturationFilter::operator ()(uint8_t* rgbData, size_t dataLength)
{
    // thanks to http://alienryderflex.com/saturation.html

    // for each pixel, change the saturation according to the following formula
    for (size_t i = 0; i < dataLength; i+=3){
        float pixelBrightness = sqrt(
                    rgbData[i] * RED_BRIGHTNESS_FACTOR+
                    rgbData[i+1]* BLUE_BRIGHTNESS_FACTOR+
                    rgbData[i+2]* GREEN_BRIGHTNESS_FACTOR);

        float r = pixelBrightness + (rgbData[i]  -  pixelBrightness) * mFactor;
        float g = pixelBrightness + (rgbData[i+1] - pixelBrightness) * mFactor;
        float b = pixelBrightness + (rgbData[i+2] - pixelBrightness) * mFactor;

        rgbData[i]  = clamp(r);
        rgbData[i+1] = clamp(g);
        rgbData[i+2] = clamp(b);
    }
}

void SaturationFilter::setFactor(float factor)
{
	mFactor = factor;
}
