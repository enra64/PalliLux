#include "lowpassfilter.h"
#include <cstring>

/**
 * @brief Create somewhat of a gliding filter
 * @param bufferLength how large the acquired buffer space must be
 * @param newFactor the factor by which the new value should be used; old will use (1-newFactor)
 */
LowPassFilter::LowPassFilter(size_t bufferLength, float newDataFactor) : mNewDataFactor(newDataFactor)
{
	mBuffer = new uint8_t[bufferLength];
}

/**
  * Release buffer
  */
LowPassFilter::~LowPassFilter()
{
	delete[] mBuffer;
}

void LowPassFilter::operator ()(uint8_t* rgbData, size_t dataLength)
{
	// replace the rgb data with weighted old + new data
	for (size_t i = 0; i < dataLength; i++)
		rgbData[i] = mNewDataFactor * rgbData[i] + (1 - mNewDataFactor) * mBuffer[i];


	// set buffer to latest value
	memcpy(mBuffer, rgbData, dataLength);
}

void LowPassFilter::setNewDataFactor(float factor)
{
	mNewDataFactor = factor;
}
