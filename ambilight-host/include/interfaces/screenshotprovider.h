#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <Magick++.h>

/**
 * @brief Exceptions occuring in the Screenshot code
 */
class ScreenshotException : public std::runtime_error
{
public:
    /**
     * @brief std::string constructor
     * @param message the error message
     */
    ScreenshotException(const std::string& message) : runtime_error(message)
    {
    }
};

/**
 * @brief Interface for capturing screen areas
 */
class ScreenshotProvider {
public:
    /**
     * @brief Empty virtual destructor to ensure derived classes will be properly destructed.
     */
    virtual ~ScreenshotProvider() {
    }

    /**
     * @brief Provide a crop of the requested geometry
     * @param[out] result the resulting image
     * @param d the geometry saved in the result, relative to the complete screen
     * @return the time in seconds required
     */
    virtual float getScreenCrop(Magick::Image& result, const Magick::Geometry& d) = 0;

    /**
     * @brief Take a screenshot.
     *
     * This should be called once per frame by any BorderProvider, to avoid taking multiple screenshots for
     * the different borders if the screenshot implementation is faster capturing only once.
     */
    virtual void takeScreenshot() {
    }
};

#endif // SCREENSHOT_H
