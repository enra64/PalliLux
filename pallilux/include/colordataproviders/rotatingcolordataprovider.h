#ifndef CONSTANTCOLORDATAPROVIDER_H
#define CONSTANTCOLORDATAPROVIDER_H

#include "colordataprovider.h"

typedef struct {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;

class RotatingColorDataProvider : public ColorDataProvider
{
public:
    RotatingColorDataProvider(LedConfig d, double rotationSpeed);

    /**
     * @brief setSV set saturation and value parameters
     * @param saturation saturation parameter; inbetween 0 and 100
     * @param value value parameter; inbetween 0 and 100
     */
    void setSV(int saturation, int value){
        mCurrentHsv.s = saturation;
        mCurrentHsv.v = value;
    }

    /**
     * @brief setRange set the hue range where the hsv values rotate in
     * @param min minium hue
     * @param max maximum hue
     */
    void setRange(float min, float max){
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

    hsv mCurrentHsv = {0, 100, 100};

    // ColorDataProvider interface
    double overflow(double in);
    rgb hsv2rgb(hsv in);
public:
    float getData(uint8_t *data);
};

#endif // CONSTANTCOLORDATAPROVIDER_H
