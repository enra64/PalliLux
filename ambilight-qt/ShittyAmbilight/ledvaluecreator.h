#ifndef LEDVALUECREATOR_H
#define LEDVALUECREATOR_H

#include "screenshot.h"

#include <ImageMagick-6/Magick++.h>
#include <ImageMagick-6/magick/image.h>

#include <ctime>
#include <iostream>

using namespace Magick;
using namespace std;

class LedValueCreator {

public:
    LedValueCreator(){
        mLeft = new Image();
        mTop = new Image();
        mRight = new Image();
        mBottom = new Image();
        mResult = new Image();
    }

    float createScreenShots(){
        clock_t start = clock();

        mScreen.takeScreenshot(mLeft, leftBar);
        mScreen.takeScreenshot(mTop, topBar);
        mScreen.takeScreenshot(mRight, rightBar);

        // bottom bar has different heights, so we need a different precedure

        vector<Image> list;
        Image left, center, right;

        mScreen.takeScreenshot(&left, bottomLeftBar);
        mScreen.takeScreenshot(&center, bottomCenterBar);
        mScreen.takeScreenshot(&right, bottomRightBar);

        list.push_back(left);
        list.push_back(center);
        list.push_back(right);

        appendImages(mBottom, list.begin(), list.end());

        return ((float)(clock() - start)) / CLOCKS_PER_SEC;
    }

    float getData(uint8_t** data){
        clock_t start = clock();
        readpixels();
        *data = mLedData;
        return ((float)(clock() - start)) / CLOCKS_PER_SEC;
    }

    void readpixels(){
        for(int i = 0; i < LED_COUNT; i+=3){
            ColorRGB data = mResult->pixelColor(i, 0);
            mLedData[i + 0] = (256 * data.red());
            mLedData[i + 1] = (256 * data.green());
            mLedData[i + 2] = (256 * data.blue());
        }
    }

    float parseScreenshot(){
        clock_t start = clock();

        mBottom->sample(xLeds);
        mTop->sample(xLeds);
        mLeft->sample(yLeds);
        mRight->sample(yLeds);

        mRight->rotate(270);
        mTop->rotate(180);
        mLeft->rotate(270);

        vector<Image> list;

        list.push_back(*mRight);
        list.push_back(*mTop);
        list.push_back(*mLeft);
        list.push_back(*mBottom);

        appendImages(mResult, list.begin(), list.end());

        return ((float)(clock() - start)) / CLOCKS_PER_SEC;
    }

    void printCurrentImages(){
        mLeft->write("zz_left.png");
        mTop->write("zz_top.png");
        mRight->write("zz_right.png");
        mBottom->write("zz_bottom.png");
        mResult->write("zz_result.png");
    }

private:
    ScreenShot mScreen;
    Image* mLeft, *mTop, *mRight, *mBottom, *mResult;

    const static int LED_X = 60, LED_Y = 20;

    const Geometry xLeds = Geometry(LED_X, 1);
    const Geometry yLeds = Geometry(1, LED_Y);


    const static int LED_COUNT = 2 * (LED_X + LED_Y);
    const static int LED_DATA_BYTES = 3 * (LED_COUNT);

    const static int X_DATA_BYTES = 3 * LED_X;
    const static int Y_DATA_BYTES = 3 * LED_Y;

    uint8_t mLedData[LED_DATA_BYTES];

    const int L_OFFSET_X = 0;
    const int L_OFFSET_Y = 0;
    const int L_SIZE_X = 1024;
    const int L_SIZE_Y = 1280;

    const int M_OFFSET_X = L_SIZE_X;
    const int M_OFFSET_Y = 0;
    const int M_SIZE_X = 1920;
    const int M_SIZE_Y = 1080;

    const int R_OFFSET_X = L_SIZE_X + M_SIZE_X;
    const int R_OFFSET_Y = 0;
    const int R_SIZE_X = 1080;
    const int R_SIZE_Y = 1920;

    const int ALL_SIZE_X = L_SIZE_X + M_SIZE_X + R_SIZE_X;

    // how many pixels to use for the border average
    const int BORDER_SIZE = 200;

    const Dimensions topBar =   {L_OFFSET_X,                L_OFFSET_Y,     ALL_SIZE_X,     BORDER_SIZE};
    // right and left bars are inbetween the top and bottom bars, so their height has to be reduced.
    const Dimensions leftBar =  {L_OFFSET_X,                BORDER_SIZE,    BORDER_SIZE,    L_SIZE_Y - 2 * BORDER_SIZE};
    const Dimensions rightBar = {ALL_SIZE_X - BORDER_SIZE,  BORDER_SIZE,    BORDER_SIZE,    R_SIZE_Y - 2 * BORDER_SIZE};

    const Dimensions bottomLeftBar =    {L_OFFSET_X, L_SIZE_Y - BORDER_SIZE, L_SIZE_X, BORDER_SIZE};
    const Dimensions bottomCenterBar =  {M_OFFSET_X, M_SIZE_Y - BORDER_SIZE, M_SIZE_X, BORDER_SIZE};
    const Dimensions bottomRightBar =   {R_OFFSET_X, R_SIZE_Y - BORDER_SIZE, R_SIZE_X, BORDER_SIZE};
};

#endif // LEDVALUECREATOR_H
