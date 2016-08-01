#include <iostream>

using namespace std;

#include "rgbconstructor.h"

int main()
{
    RgbConstructor rgb;
    uint8_t rgbArray[480];

    rgb.takeAndParseScreenShot(rgbArray);
    cout << "Hello World!" << endl;
    return 0;
}
