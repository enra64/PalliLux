#include "linearfactorfilter.h"

float LinearFactorFilter::getFactor(uint8_t point) const 
{
	if (point < 172)
		return 1.f;
	return (255 - point);
}

void LinearFactorFilter::operator ()(uint8_t* rgbData, size_t dataLength)
{
	for (size_t i = 0; i < dataLength; i++)
		rgbData[i] *= getFactor(rgbData[i]);
}
