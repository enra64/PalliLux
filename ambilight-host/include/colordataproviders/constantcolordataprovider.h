#ifndef CONSTANTCOLORDATAPROVIDER_H
#define CONSTANTCOLORDATAPROVIDER_H

#include "colordataprovider.h"


class ConstantColorDataProvider : public ColorDataProvider
{
public:
    ConstantColorDataProvider(unsigned int horizontalLedCount, unsigned int verticalLedCount, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0);

    /**
     * @brief Update the channel values
     */
    void setRgb(uint8_t red, uint8_t green, uint8_t blue);

    void setRed(uint8_t red){
        mRed = red;
    }

    void setGreen(uint8_t green){
        mGreen = green;
    }

    void setBlue(uint8_t blue){
        mBlue = blue;
    }

private:
    uint8_t mRed;///< the red channel value
    uint8_t mGreen;///< the green channel value
    uint8_t mBlue;///< the blue channel value

    // ColorDataProvider interface
public:
    float getData(uint8_t *data);
};

#endif // CONSTANTCOLORDATAPROVIDER_H
