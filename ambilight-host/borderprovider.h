#ifndef BORDERPROVIDER_H
#define BORDERPROVIDER_H

#include "ImageMagick-6/Magick++.h"
#include "ImageMagick-6/magick/image.h"

/**
 * @brief The BorderProvider class should contain all information about the screen configuration,
 * and only be responsible for providing border images to the rgb converter.
 */
class BorderProvider {
public:
    /**
     * @brief takeBorderShot This function must capture each screen border into a Magick++ image
     */
    virtual void retrieveBorders(Magick::Image& right, Magick::Image& top, Magick::Image& left, Magick::Image& bottom) const = 0;
};

#endif // BORDERPROVIDER_H
