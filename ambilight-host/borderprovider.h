#ifndef BORDERPROVIDER_H
#define BORDERPROVIDER_H

#include "ImageMagick-6/Magick++.h"
#include "ImageMagick-6/magick/image.h"

/**
 * @brief Implementations encapsulate all screen information, providing only border images.
 */
class BorderProvider {
public:
    /**
     * @brief This function must capture each screen border into a Magick++ image
     */
    virtual void retrieveBorders(Magick::Image& right, Magick::Image& top, Magick::Image& left, Magick::Image& bottom) = 0;
};

#endif // BORDERPROVIDER_H
