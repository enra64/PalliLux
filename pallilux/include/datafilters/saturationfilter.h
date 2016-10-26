#include "datafilter.h"

class SaturationFilter : public DataFilter
{
    float mFactor; ///< the saturation factor that will be applied to any given data
    const float RED_BRIGHTNESS_FACTOR = .299f;
    const float GREEN_BRIGHTNESS_FACTOR = .587f;
    const float BLUE_BRIGHTNESS_FACTOR = .114f;

    /**
     * @brief Utility function to clamp floats to uint8_t values
     * @return 0 if the float is < 0; 255 if float is larger than 255
     */
    uint8_t clamp(float value);
public:
	/**
     * @brief Construct a SaturationFilter with the given factor
     * @param factor the float factor that will be applied
	 */
    SaturationFilter(float factor);

	void operator ()(uint8_t* rgbData, size_t dataLength) override;

    /**
     * @brief Set the saturation factor that will be applied.
     * @param factor values over 1 will increase the saturation; values below 1 will decrease it.
     */
	void setFactor(float factor);
};
