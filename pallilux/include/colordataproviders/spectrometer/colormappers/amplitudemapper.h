#ifndef PALLILUX_AMPLITUDEMAPPER_H
#define PALLILUX_AMPLITUDEMAPPER_H

#include <cstdint>

#include "abstractcolormapper.h"
#include "hsvutils.h"

class AmplitudeMapper : public AbstractColorMapper {
private:
    double mMinimumHue;
    double mMaximumHue;
public:
    void setMinimumHue(double minHue){
        mMinimumHue = minHue;
    }

    void setMaximumHue(double maxHue){
        mMaximumHue = maxHue;
    }

    AmplitudeMapper(double minHue = 0, double maxHue = 360) :
        AbstractColorMapper() {
        mMinimumHue = minHue;
        mMaximumHue = maxHue;
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
        hsv color = {0, 1, 1};
        rgb rgbColor;

        for(int i = 0; i < amplitudeCount; i++){
            // flip the indexing if required
            int amplitudeIndex = !flip ? i : amplitudeCount - 1 - i;

            // set the hue depending on the amplitude
            color.h = mMinimumHue + ((mMaximumHue - mMinimumHue) * double(amplitudes[amplitudeIndex])) / 255.0;

            // get a rgb version
            rgbColor = hsv2rgb(color);

            // apply to led
            data[i * 3 + 0] = 255.0 * rgbColor.r;
            data[i * 3 + 1] = 255.0 * rgbColor.g;
            data[i * 3 + 2] = 255.0 * rgbColor.b;
        }
    }
};

#endif //PALLILUX_AMPLITUDEMAPPER_H
