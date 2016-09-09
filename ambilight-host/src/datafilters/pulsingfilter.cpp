#include "pulsingfilter.h"

#include <cmath>

PulsingFilter::PulsingFilter(unsigned int cycleLength, uint8_t maximumAmplitude) :
    mCycleLength(cycleLength), mMaximumAmplitude(maximumAmplitude){
    if(mCycleLength <= 0)
        throw std::invalid_argument("Cycle length must be larger than 0!");
}

void PulsingFilter::operator ()(uint8_t *rgbData, size_t dataLength) {
    // calculate our current position as a number between zero and one
    double betweenZeroAndOne = double(mTickCount++ % mCycleLength) / double(mCycleLength);

    // scale that number to between 0 and 2*PI to get all sin values
    double sinValue = sin(betweenZeroAndOne * 2 * M_PI);

    // calculate the offset from the normal value
    double offset = sinValue * mMaximumAmplitude;

    // apply our factor
    for (size_t i = 0; i < dataLength; i++){
        // avoid underflow
        if(offset < 0 && abs(offset) > rgbData[i])
            rgbData[i] = 0;

        // avoid overflow
        else if (offset >= 0 && (int(offset+0.5) + (int)rgbData[i]) > 255)
            rgbData[i] = 255;

        // just apply the offset
        else
            rgbData[i] += offset;
    }
}
