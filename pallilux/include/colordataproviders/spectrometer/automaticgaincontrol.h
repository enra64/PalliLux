#ifndef AUTOMATICGAINCONTROL_H
#define AUTOMATICGAINCONTROL_H

#include <cmath>
#include <cstdint>
#include <algorithm>
#include <stddef.h>

class AutomaticGainControl
{
    size_t mLedCount;///< max amplitude count, basically
    uint8_t* mAmplitudeBuffer;///< buffer for the stored current amplitudes
    const double NEW_DATA_FACTOR = 0.8;///< how fast the low pass filter acts
    const double ACTION_SPEED = 1.0;///< how fast the agent should act. does integer mult, so should be >= 1
    const double MAX_GAIN_VALUE = 2500;///< maximum allowed gain value
    const uint8_t MINIMUM_ACTION_AMPLITUDE = 10;///< below this threshold, no action is taken to change the gain
    const uint8_t TARGET_AMPLITUDE = 240;///< where should the max amplitude typically be
    double mGain = 100;///< current gain
    bool mGainHasChanged = false;
public:
    AutomaticGainControl(uint32_t ledCount);
    ~AutomaticGainControl();
    /**
     * @brief getGain get the next gain value
     * @param barsL left amplitude array
     * @param barLsize number of left amplitudes
     * @param barsR right amplitude array
     * @param barRsize number of right amplitudes
     * @return new gain value
     */
    double getGain(uint8_t *barsL, size_t barLsize, uint8_t* barsR, size_t barRsize);

    /**
     * @brief getGain get the current gain value without adjusting it
     */
    double getGain();

    /**
     * @brief hasGainChanged returns true if the gain has changed since the last time this function was called
     */
    bool hasGainChanged();
};

#endif // AUTOMATICGAINCONTROL_H
