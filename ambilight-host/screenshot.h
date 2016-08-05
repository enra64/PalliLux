#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <ImageMagick-6/Magick++.h>

/**
 * @brief Interface for capturing screen areas
 */
class Screenshot {
public:
    /**
     * @brief Provide a crop of the requested geometry
     * @param[out] result the resulting image
     * @param d the geometry saved in the result, relative to the complete screen
     * @return the time in seconds required
     */
    virtual float getScreenCrop(Magick::Image& result, const Magick::Geometry& d) = 0;

    /**
     * @brief Take a screenshot; this is for implementations where it is faster to only capture the screen once.
     */
    virtual void takeScreenshot() {
    }
};

#endif // SCREENSHOT_H
