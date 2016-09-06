#include "datafilter.h"

class LinearFactorFilter : public DataFilter
{
	float getFactor(uint8_t point) const;

public:
	void operator ()(uint8_t* rgbData, size_t dataLength) override;
};
