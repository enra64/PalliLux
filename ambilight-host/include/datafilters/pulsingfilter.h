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
    PulsingFilter(unsigned int cycleLength, uint8_t maximumAmplitude);

    void operator ()(uint8_t* rgbData, size_t dataLength) override;

    void setCycleLength(unsigned int length){
        if(length <= 0)
            throw std::invalid_argument("Cycle length must be larger than 0!");
        mCycleLength = length;
    }

    void setMaximumAmplitude(uint8_t amplitude){
        mMaximumAmplitude = amplitude;
    }

private:
    unsigned int mCycleLength; ///< how long a full cycle is, measured in ticks
    unsigned int mTickCount = 0;///< how many times the operator has been applied
    uint8_t mMaximumAmplitude;///< the maximum derivation from the normal value
};
