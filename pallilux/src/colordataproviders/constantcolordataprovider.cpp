#include "constantcolordataprovider.h"

#include <chrono>
#include <thread>

using namespace std;

ConstantColorDataProvider::ConstantColorDataProvider(LedConfig d, uint8_t red, uint8_t green, uint8_t blue)
    : ColorDataProvider(d) {

}

void ConstantColorDataProvider::setRgb(uint8_t red, uint8_t green, uint8_t blue) {
    mRed = red;
    mGreen = green;
    mBlue = blue;
}

float ConstantColorDataProvider::getData(uint8_t *data) {
    // benchmarking
    clock_t start = clock();

    // wait to allow arduino to push led colors
    this_thread::sleep_for(chrono::milliseconds(9));

    // apply channel data for all pixels
    for (int i = 0; i < LED_COUNT; i++) {
        data[i * 3 + 0] = mRed;
        data[i * 3 + 1] = mGreen;
        data[i * 3 + 2] = mBlue;
    }

    // apply all filters in the list
    applyFilters(data);

    // return benchmarking value
    return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}
