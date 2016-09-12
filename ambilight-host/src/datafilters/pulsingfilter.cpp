#include "pulsingfilter.h"

#define _USE_MATH_DEFINES
#include <cmath>

// define M_PI if _USE_MATH_DEFINES did not fix undeclared identifier error
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


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
