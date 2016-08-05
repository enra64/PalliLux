#ifndef BORDERSHOT_H
#define BORDERSHOT_H

#include "borderprovider.h"
#include "xlibscreenshot.h"

/**
 * @brief An implementation of BorderProvider, accessing three monitors via the ScreenShot class.
 */
class TripleScreenBorderProvider : public BorderProvider {
public:
    /**
     * @brief This function creates a shot of each border; the rgb constructor can then use it to create the LED data
     */
    void retrieveBorders(Magick::Image& right, Magick::Image& top, Magick::Image& left, Magick::Image& bottom) override;

private: // screen config info
    //@{
    /** left screen size and position */
    const size_t LEFT_X_POSITION = 0;
    const size_t LEFT_Y_POSITION = 0;
    const size_t LEFT_SCREEN_WIDTH = 1024;
    const size_t LEFT_SCREEN_HEIGHT = 1280;
    //@}

    //@{
    /** center screen size and position */
    const size_t CENTER_X_POSITION = LEFT_SCREEN_WIDTH;
    const size_t CENTER_Y_POSITION = 0;
    const size_t CENTER_SCREEN_WIDTH = 1920;
    const size_t CENTER_SCREEN_HEIGHT = 1080;
    //@}

    //@{
    /** right screen size and position */
    const size_t RIGHT_X_POSITION = LEFT_SCREEN_WIDTH + CENTER_SCREEN_WIDTH;
    const size_t RIGHT_Y_POSITION = 0;
    const size_t RIGHT_SCREEN_WIDTH = 1080;
    const size_t RIGHT_SCREEN_HEIGHT = 1920;
    //@}

    //@{
    /** overall size */
    const size_t ALL_WIDTH = LEFT_SCREEN_WIDTH + CENTER_SCREEN_WIDTH + RIGHT_SCREEN_WIDTH;
    const size_t ALL_HEIGHT = 1920;
    //@}

    /**
     * @brief how wide the border images should be
     */
    const size_t BORDER_SIZE = 100;

    //@{
    /** resulting dimensions using the constants defined for the screen sizes  */
    const Magick::Geometry mTopBorderDimensions = Magick::Geometry(ALL_WIDTH, BORDER_SIZE, LEFT_X_POSITION, LEFT_Y_POSITION);
    const Magick::Geometry mLeftBorderDimensions = Magick::Geometry(BORDER_SIZE, LEFT_SCREEN_HEIGHT - 2 * BORDER_SIZE, LEFT_X_POSITION, BORDER_SIZE);
    const Magick::Geometry mRightBorderDimensions = Magick::Geometry(BORDER_SIZE, RIGHT_SCREEN_HEIGHT - 2 * BORDER_SIZE, ALL_WIDTH - BORDER_SIZE,  BORDER_SIZE);

    const Magick::Geometry mBottomLeftBorderDimensions = Magick::Geometry(LEFT_SCREEN_WIDTH, BORDER_SIZE, LEFT_X_POSITION, LEFT_SCREEN_HEIGHT - BORDER_SIZE);
    const Magick::Geometry mBottomCenterBorderDimensions = Magick::Geometry(CENTER_SCREEN_WIDTH,    BORDER_SIZE, CENTER_X_POSITION, CENTER_SCREEN_HEIGHT - BORDER_SIZE);
    const Magick::Geometry mBottomRightBorderDimensions = Magick::Geometry(RIGHT_SCREEN_WIDTH,     BORDER_SIZE, RIGHT_X_POSITION, RIGHT_SCREEN_HEIGHT - BORDER_SIZE);
    //@}

private:
    /**
     * @brief this class provides access to xlib screenshot functionality
     */
    XlibScreenshot mScreenshot;
};

#endif // BORDERSHOT_H
