#include "pulsingfilter.h"

#include <cmath>

PulsingFilter::PulsingFilter(unsigned int cycleLength, double maximumFactor) :
    mCycleLength(cycleLength), mMaximumFactor(maximumFactor){
    if(mCycleLength <= 0)
        throw std::invalid_argument("Cycle length must be larger than 0!");
}

void PulsingFilter::operator ()(uint8_t *rgbData, size_t dataLength) {
    // calculate our current position as a number between zero and two pi - the full range of sin
    double sinX = (double(mTickCount++ % mCycleLength) / double(mCycleLength)) * 2 * M_PI;

    // mMaximumFactor moves the maximum amplitude of sin
    // at 0, the resulting factor is always 0%
    // at 1, the resulting maximum factor is 200%
    double factor = sin(sinX) * mMaximumFactor + 1;

    // apply our factor
    for (size_t i = 0; i < dataLength; i++){
        int factorized = rgbData[i] * factor;
        // avoid underflow
        if(factorized < 0)
            rgbData[i] = 0;

        // avoid overflow
        else if (factorized > 255)
            rgbData[i] = 255;

        // neither over- nor underflow - just apply the factor
        else
            rgbData[i] = static_cast<uint8_t>(factorized);
    }
}
