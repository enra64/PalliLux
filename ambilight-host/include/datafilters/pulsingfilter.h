#include "datafilter.h"

#include <stdexcept>

/**
 * @brief A filter that applies a pulsing effect using sin.
 */
class PulsingFilter : public DataFilter
{
public:
    /**
     * @brief PulsingFilter
     * @param cycleLength how long a full cycle is, measured in ticks
     * @param maximumAmplitude the maximum derivation from the normal value
     */
    PulsingFilter(unsigned int cycleLength, double maximumFactor);

    void operator ()(uint8_t* rgbData, size_t dataLength) override;

    void setCycleLength(unsigned int length){
        if(length <= 0)
            throw std::invalid_argument("Cycle length must be larger than 0!");
        mCycleLength = length;
    }

    /**
     * @brief Set the maximum amplitude.
     *
     * The best factor values are between 0 and 1. When factor is 0,
     * the filter will not change the data; the larger the factor is
     * above 1, the more the leds will be all black or all white.
     */
    void setMaximumAmplitude(double factor){
        mMaximumFactor = factor;
    }

private:
    unsigned int mCycleLength; ///< how long a full cycle is, measured in ticks
    unsigned int mTickCount = 0;///< how many times the operator has been applied
    double mMaximumFactor;///< the maximum derivation from the normal value
};
