#include "datafilter.h"

class BrightnessFilter : public DataFilter
{
    float mFactor; ///< the factor that will be applied to any given data

public:
	/**
	 * @brief Construct a BrightnessFilter with the given factor
     * @param factor the float factor that will be applied
	 */
	BrightnessFilter(float factor);

	void operator ()(uint8_t* rgbData, size_t dataLength) override;

    /**
     * @brief Set the brightness factor that will be applied to every single value.
     */
	void setFactor(float factor);
};
