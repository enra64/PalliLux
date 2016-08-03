#ifndef BORDERPROVIDER_H
#define BORDERPROVIDER_H

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/image.h>

class BorderProvider
{
public:
    /**
     * @brief takeBorderShot This function creates a shot of each border; the rgb constructor can then use it to create the LED data
     */
    virtual void retrieveBorders(Magick::Image& right, Magick::Image& top, Magick::Image& left, Magick::Image& bottom) const = 0;
};

#endif // BORDERPROVIDER_H
