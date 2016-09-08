#include "brightnessfilter.h"

/**
 * @brief Construct a BrightnessFilter with the given factor
 * @param factor the float factor that will be applied. Values
 */
BrightnessFilter::BrightnessFilter(float factor) : mFactor(factor)
{
}

void BrightnessFilter::operator ()(uint8_t* rgbData, size_t dataLength)
{
    // apply our factor to every data value
	for (size_t i = 0; i < dataLength; i++)
		rgbData[i] *= mFactor;
}

void BrightnessFilter::setFactor(float factor)
{
	mFactor = factor;
}
