#ifndef PALLILUX_SINGLECOLORMAPPER_H
#define PALLILUX_SINGLECOLORMAPPER_H

#include <cstdint>

#include "abstractcolormapper.h"
#include "hsvutils.h"

class SingleColorMapper : public AbstractColorMapper {
private:
    hsv mBaseColor;///< base color
public:
    /**
     * @brief Set the base color. Transformed to hsv, the value of this color will be changed
     *        to reflect amplitude changes
     */
    void setColor(uint8_t r, uint8_t g, uint8_t b){
        mBaseColor = rgb2hsv(r, g, b);
        mBaseColor.s = 1;
    }

    void setColor(double hue){
        mBaseColor.h = hue;
    }

    SingleColorMapper(uint8_t r, uint8_t g, uint8_t b) :
        AbstractColorMapper() {
        setColor(r, g, b);
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
    }
};

#endif //PALLILUX_SINGLECOLORMAPPER_H
