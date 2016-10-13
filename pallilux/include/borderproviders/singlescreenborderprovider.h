#ifndef SINGLESCREENBORDERPROVIDER_H
#define SINGLESCREENBORDERPROVIDER_H

#include "borderprovider.h"

/**
 * @brief A BorderProvider that considers a single screen.
 *
 * The position may be offset to select a screen out of multiple ones, and offsets to consider letterboxing are supported.
 */
class SingleScreenBorderProvider : public BorderProvider
{
public:
    /**
     * @brief SingleScreenBorderProvider
     * @param width screen width in pixels
     * @param height screen height in pixels
     * @param xOffset horizontal screen offset in pixels
     * @param yOffset vertical screen offset in pixels
     * @param letterboxOffsetX the width of the vertical letterbox bars
     * @param letterboxOffsetY the height of the horizontal letterbox bars
     */
    SingleScreenBorderProvider(size_t width, size_t height, size_t xOffset = 0, size_t yOffset = 0, size_t letterboxOffsetX = 0, size_t letterboxOffsetY = 0);

	void retrieveBorders(Image& right, Image& top, Image& left, Image& bottom) override;

protected:
	void updateGeometry() override;

private:
    Geometry mRightGeometry;///< right border geometry
    Geometry mTopGeometry;///< top border geometry
    Geometry mLeftGeometry;///< left border geometry
    Geometry mBottomGeometry;///< bottom border geometry

    size_t mWidth;///< screen geometry in pixels
    size_t mHeight;///< screen geometry in pixels
    size_t mXOffset;///< screen offset in pixels
    size_t mYOffset;///< screen offset in pixels
    size_t mVerticalLetterboxBarWidth;///< the height of the horizontal letterbox bars
    size_t mHorizontalLetterboxBarHeight;///< the width of the vertical letterbox bars
};

#endif // SINGLESCREENBORDERPROVIDER_H
