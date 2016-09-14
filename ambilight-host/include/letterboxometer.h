#ifndef LETTERBOXOMETER_H
#define LETTERBOXOMETER_H

#include <memory>

#include <Magick++.h>

#include "screenshotprovider.h"

/**
 * @brief Easily calculate the letterbox size to avoid computing colors for black bars.
 */
class LetterboxOMeter
{
public:
    /**
     * @brief Create a LetterboxOMeter
     * @param screenshotProvider The ScreenshotProvider used to take screenshots for bar calculation
     * @param w The horizontal screen resolution (in pixels)
     * @param h The vertical screen resolution (in pixels)
     * @param xOff The horizontal screen offset (to, for example, use your second screen instead of the first) (in pixels)
     * @param yOff The vertical screen offset (to, for example, use your second screen instead of the first) (in pixels)
     * @param blackTreshold If any channel of any pixel is brighter than this, the letterbox has ended. Values 0-255.
     */
    LetterboxOMeter(std::shared_ptr<ScreenshotProvider> screenshotProvider, size_t w, size_t h, size_t xOff, size_t yOff, uint8_t blackThreshold);

    /**
     * @brief Measure the letterbox sizes
     * @return a pair: <measured height of horizontal bars, measured width of vertical bars>
     */
    std::pair<int, int> measure();

private:// constant or input members
    const std::shared_ptr<ScreenshotProvider> mScreenshotProvider;///< our screenshot provider
    const size_t mScreenWidth;///< how wide the screen is in pixels
    const size_t mScreenHeight;///< how high the screen is in pixels
    const size_t mScreenXOffset;///< what horizontal offset the screen has, in pixels
    const size_t mScreenYOffset;///< what vertical offset the screen has, in pixels
    const uint8_t mBlackThreshold;///< the maximum channel brightness to be accepted as black

private://functions
    /**
     * @brief Check whether a bar is completely black (In accordance with LetterboxOMeter::mBlackThreshold)
     * @param img the Magick::Image to be checked
     * @return true if no pixel had a channel brighter than LetterboxOMeter::mBlackThreshold.
     */
    bool isBlack(Magick::Image img);

    /**
     * @brief Returns a horizontal bar
     * @param height how many pixels the bar is high
     * @param topOffset how many pixels the bar is removed from the top
     * @return a Magick::Geometry specification of the bar
     */
	Magick::Geometry getHorizontalBar(size_t height, size_t topOffset) const;

    /**
     * @brief Returns a vertical bar
     * @param width how many pixels the bar is wide
     * @param leftOffset how many pixels the bar is removed from the left
     * @return a Magick::Geometry specification of the bar
     */
	Magick::Geometry getVerticalBar(size_t width, size_t leftOffset) const;
};

#endif
