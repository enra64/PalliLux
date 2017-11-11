#ifndef PALLILUX_SINGLEROTATINGCOLORMAPPER_H
#define PALLILUX_SINGLEROTATINGCOLORMAPPER_H

#include <cstdint>

#include "abstractcolormapper.h"
#include "hsvutils.h"

class SingleRotatingColorMapper : public AbstractColorMapper {
private:
    hsv mBaseColor;///< base color
    double mSpeed;///< the color rotation speed
public:
    /**
     * @brief SingleRotatingColorMapper initialize the colormapper with red at full saturation
     */
    SingleRotatingColorMapper() : AbstractColorMapper() {
        mBaseColor = rgb2hsv(255, 0, 0);
        mBaseColor.s = 1;
    }

    /**
     * @brief setSpeed set the color rotation speed
     * @param speed a double measuring the speed of color rotation. assuming the default FPS of 60,
     *              a single full rotation of color (360 degrees) is done once per second for a speed of 1.
     */
    void setSpeed(double speed) {
        mSpeed = speed * 6;
    }

    /**
     * @brief increaseHue increases the hue by the specified amount every *map*-call, so that the color
     *                    will appear to be rotating
     */
    void increaseHue() {
        // rotate hue
        mBaseColor.h += mSpeed;

        // allow positive and negative wrap-around
        while (mBaseColor.h > 360) mBaseColor.h -= 360;
        while (mBaseColor.h < 0) mBaseColor.h += 360;
    }

    /**
     * @brief translate the given amplitudes into led colors
     * @param amplitudes pointer to a buffer where the amplitudes are stored. They are expected to
     *                   be stored with ascending frequency buckets.
     * @param amplitudeCount how many amplitudes are available
     * @param data buffer where the led data should be written. 3*amplitudeCount required
     * @param flip true if the smallest led index should reflect the highest frequency intensity
     */
    void map(uint8_t* amplitudes, int amplitudeCount, uint8_t* data, bool flip = false) override {
        hsv color = mBaseColor;
        rgb rgbColor;

        for(int i = 0; i < amplitudeCount; i++){
            // flip the indexing if required
            int amplitudeIndex = !flip ? i : amplitudeCount - 1 - i;

            // set the value depending on the amplitude
            color.v = double(amplitudes[amplitudeIndex]) / 255.0;

            // get a rgb version
            rgbColor = hsv2rgb(color);

            // apply to led
            data[i * 3 + 0] = 255.0 * rgbColor.r;
            data[i * 3 + 1] = 255.0 * rgbColor.g;
            data[i * 3 + 2] = 255.0 * rgbColor.b;
        }

        // rotate the color by increasing hue gradually
        increaseHue();
    }
};

#endif //PALLILUX_SINGLEROTATINGCOLORMAPPER_H
