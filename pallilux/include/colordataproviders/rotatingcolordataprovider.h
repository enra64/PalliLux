#ifndef CONSTANTCOLORDATAPROVIDER_H
#define CONSTANTCOLORDATAPROVIDER_H

#include "colordataprovider.h"
#include <assert.h>
#include "hsvutils.h"


class RotatingColorDataProvider : public ColorDataProvider
{
public:
    RotatingColorDataProvider(LedConfig d, double rotationSpeed);

    /**
     * @brief setSV set saturation and value parameters
     * @param saturation saturation parameter; inbetween 0 and 1
     * @param value value parameter; inbetween 0 and 1
     */
    void setSV(double saturation, double value){
        assert(saturation <= 1 && value <= 1);
        mCurrentHsv.s = saturation;
        mCurrentHsv.v = value;
    }

    /**
     * @brief setRange set the hue range where the hsv values rotate in
     * @param min minium hue
     * @param max maximum hue
     */
    void setRange(double min, double max){
        mMinimumHue = min;
        mMaximumHue = max;
    }

    /**
     * @brief setSpeed set the delta of hue per rotation
     * @param speed hue difference between iterations
     */
    void setSpeed(double speed){
        mRotationSpeed = speed;
    }

private:
    double mMinimumHue = 0;
    double mMaximumHue = 360;
    double mRotationSpeed;

    hsv mCurrentHsv = {0, 1, 1};

    // ColorDataProvider interface
    double overflow(double in);
public:
    float getData(uint8_t *data);
};

#endif // CONSTANTCOLORDATAPROVIDER_H
