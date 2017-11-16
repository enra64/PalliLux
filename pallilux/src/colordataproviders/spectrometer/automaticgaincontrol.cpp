#include "automaticgaincontrol.h"

AutomaticGainControl::AutomaticGainControl(uint32_t ledCount)
{
    mLedCount = ledCount;
    mAmplitudeBuffer = new uint8_t[mLedCount];
}

AutomaticGainControl::~AutomaticGainControl()
{
    if(mAmplitudeBuffer != nullptr)
        delete[] mAmplitudeBuffer;
}

double AutomaticGainControl::getGain(uint8_t *barsL, size_t barLsize, uint8_t* barsR, size_t barRsize)
{
    // update the low pass filtered buffer
    for (size_t i = 0; i < barLsize + barRsize; i++){
        if (i < barLsize)
            mAmplitudeBuffer[i] = NEW_DATA_FACTOR * barsL[i] + (1 - NEW_DATA_FACTOR) * mAmplitudeBuffer[i];
        else
            mAmplitudeBuffer[i] = NEW_DATA_FACTOR * barsR[i - barLsize] + (1 - NEW_DATA_FACTOR) * mAmplitudeBuffer[i];
    }

    // find the current maximum amplitude
    uint8_t maxAmplitude = *std::max_element(mAmplitudeBuffer, mAmplitudeBuffer + barLsize + barRsize);

    // avoid increasing gain infinitely because no music is even played
    if (maxAmplitude < MINIMUM_ACTION_AMPLITUDE)
        return mGain;

    // calculate adjustment value
    int16_t targetDifference = TARGET_AMPLITUDE - maxAmplitude;
    double adjustValue = (ACTION_SPEED * targetDifference);

    // if gain is smaller than max allowed value, change it
    if(mGain + adjustValue < MAX_GAIN_VALUE){
        mGain += adjustValue;

        // set hasChanged flag for significant changes
        if(abs(adjustValue) > .05)
            mGainHasChanged = true;
    }

    // return latest value
    return mGain;
}

double AutomaticGainControl::getGain()
{
    // return current gain value
    return mGain;
}

bool AutomaticGainControl::hasGainChanged()
{
    if(mGainHasChanged){
        mGainHasChanged = false;
        return true;
    }
    return false;
}
