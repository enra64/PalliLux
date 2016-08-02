#ifndef BORDERSHOT_H
#define BORDERSHOT_H

#include "dimensions.h"
#include "screenshot.h"

/**
 * @brief The BorderProvider class is designed to keep the other code somewhat clean by containing all information about the screen configuration.
 */
class BorderProvider {
public:
    /**
     * @brief takeBorderShot This function creates a shot of each border; the rgb constructor can then use it to create the LED data
     */
    void retrieveBorders(Magick::Image& right, Magick::Image& top, Magick::Image& left, Magick::Image& bottom);

private: // screen config info
    // left screen size and position
    const int LEFT_X_POSITION = 0;
    const int LEFT_Y_POSITION = 0;
    const int LEFT_SCREEN_WIDTH = 1024;
    const int LEFT_SCREEN_HEIGHT = 1280;

    // center screen size and position
    const int CENTER_X_POSITION = LEFT_SCREEN_WIDTH;
    const int CENTER_Y_POSITION = 0;
    const int CENTER_SCREEN_WIDTH = 1920;
    const int CENTER_SCREEN_HEIGHT = 1080;

    // right screen size and position
    const int RIGHT_X_POSITION = LEFT_SCREEN_WIDTH + CENTER_SCREEN_WIDTH;
    const int RIGHT_Y_POSITION = 0;
    const int RIGHT_SCREEN_WIDTH = 1080;
    const int RIGHT_SCREEN_HEIGHT = 1920;

    // composite width
    const int ALL_WIDTH = LEFT_SCREEN_WIDTH + CENTER_SCREEN_WIDTH + RIGHT_SCREEN_WIDTH;
    const int ALL_HEIGHT = 1920;

    /**
     * @brief BORDER_SIZE how wide the border images should be
     */
    const int BORDER_SIZE = 100;

    const Dimensions mTopBorderDimensions =          {LEFT_X_POSITION,           LEFT_Y_POSITION,                    ALL_WIDTH,             BORDER_SIZE};
    // right and left bars are inbetween the top and bottom bars, so their height has to be reduced.
    const Dimensions mLeftBorderDimensions =         {LEFT_X_POSITION,           BORDER_SIZE,                        BORDER_SIZE,            LEFT_SCREEN_HEIGHT - 2 * BORDER_SIZE};
    const Dimensions mRightBorderDimensions =        {ALL_WIDTH - BORDER_SIZE,  BORDER_SIZE,                        BORDER_SIZE,            RIGHT_SCREEN_HEIGHT - 2 * BORDER_SIZE};

    const Dimensions mBottomLeftBorderDimensions =    {LEFT_X_POSITION,           LEFT_SCREEN_HEIGHT - BORDER_SIZE,   LEFT_SCREEN_WIDTH,      BORDER_SIZE};
    const Dimensions mBottomCenterBorderDimensions =  {CENTER_X_POSITION,         CENTER_SCREEN_HEIGHT - BORDER_SIZE, CENTER_SCREEN_WIDTH,    BORDER_SIZE};
    const Dimensions mBottomRightBorderDimensions =   {RIGHT_X_POSITION,          RIGHT_SCREEN_HEIGHT - BORDER_SIZE,  RIGHT_SCREEN_WIDTH,     BORDER_SIZE};

private://member variables
    /**
     * @brief mScreenshotAccess this class provides access to screenshot functionality
     */
    Screenshot mScreenshot = Screenshot(ALL_WIDTH, ALL_HEIGHT);
};

#endif // BORDERSHOT_H
