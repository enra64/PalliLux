#ifndef BORDERSHOT_H
#define BORDERSHOT_H

#include "borderprovider.h"

#include <algorithm>

/**
 * @brief An implementation of BorderProvider, accessing three monitors via the ScreenShot class.
 */
class TripleScreenBorderProvider : public BorderProvider
{
public:
    TripleScreenBorderProvider(size_t w1, size_t h1, size_t w2, size_t h2, size_t w3, size_t h3);

	/**
	 * @brief This function creates a shot of each border; the rgb constructor can then use it to create the LED data
	 */
	void retrieveBorders(Image& right, Image& top, Image& left, Image& bottom) override;

protected:
	void updateGeometry() override;

private: // screen config info
	//@{
	/** left screen size and position */
	size_t LEFT_X_POSITION;
	size_t LEFT_Y_POSITION;
	const size_t LEFT_SCREEN_WIDTH;
	const size_t LEFT_SCREEN_HEIGHT;
	//@}

	//@{
	/** center screen size and position */
	size_t CENTER_X_POSITION;
	size_t CENTER_Y_POSITION;
	const size_t CENTER_SCREEN_WIDTH;
	const size_t CENTER_SCREEN_HEIGHT;
	//@}

	//@{
	/** right screen size and position */
	size_t RIGHT_X_POSITION;
	size_t RIGHT_Y_POSITION;
	const size_t RIGHT_SCREEN_WIDTH;
	const size_t RIGHT_SCREEN_HEIGHT;
	//@}

	//@{
	/** overall size */
	const size_t ALL_WIDTH = LEFT_SCREEN_WIDTH + CENTER_SCREEN_WIDTH + RIGHT_SCREEN_WIDTH;
	const size_t ALL_HEIGHT = std::max(LEFT_SCREEN_HEIGHT, std::max(CENTER_SCREEN_HEIGHT, RIGHT_SCREEN_HEIGHT));
	//@}

	//@{
	/** resulting dimensions using the constants defined for the screen sizes  */
	Geometry mTopBorderDimensions;
	Geometry mLeftBorderDimensions;
	Geometry mRightBorderDimensions;

	Geometry mBottomLeftBorderDimensions;
	Geometry mBottomCenterBorderDimensions;
	Geometry mBottomRightBorderDimensions;
	//@}
};

#endif // BORDERSHOT_H
