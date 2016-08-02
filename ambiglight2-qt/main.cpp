#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

#include "rgbconstructor.h"

using namespace std;

int main()
{
    RgbConstructor rgb;
    uint8_t rgbArray[480];

    unsigned int runs = 100;

    float dur = 0;

    for(unsigned int i = 0; i < runs; i++)
        dur += rgb.takeAndParseScreenShot(rgbArray);

    dur /= (float) runs;

    cout << "avg dur:" << dur << " -> fps:" << (1.f / dur) << endl;

    for(int i = 0; i < 480; i+=3){
        cout << "R:" << to_string(rgbArray[i]) << " G:" << to_string(rgbArray[i+1]) << " B:" << to_string(rgbArray[i+2]) << endl;

    }

    return 0;
}
