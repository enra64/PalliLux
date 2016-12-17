#ifndef PALLILUX_ABSTRACTCOLORMAPPER_H
#define PALLILUX_ABSTRACTCOLORMAPPER_H

#include <cstdint>

class AbstractColorMapper {
public:
    AbstractColorMapper() {}

    virtual ~AbstractColorMapper() {}

    /**
     * @brief translate the given amplitudes into led colors
     * @param amplitudes pointer to a buffer where the amplitudes are stored. They are expected to
     *                   be stored with ascending frequency buckets.
     * @param amplitudeCount how many amplitudes are available
     * @param data buffer where the led data should be written. 3*amplitudeCount required
     * @param flip true if the smallest led index should reflect the highest frequency intensity
     */
    virtual void map(uint8_t* amplitudes, int amplitudeCount, uint8_t* data, bool flip) = 0;
};

#endif //PALLILUX_ABSTRACTCOLORMAPPER_H
