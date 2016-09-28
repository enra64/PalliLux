#include "datafilter.h"

class LinearFactorFilter : public DataFilter
{
    // uhh this is kinda unfinished
	float getFactor(uint8_t point) const;

public:
	void operator ()(uint8_t* rgbData, size_t dataLength) override;
};
