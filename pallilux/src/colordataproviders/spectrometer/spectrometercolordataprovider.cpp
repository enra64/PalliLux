#include "spectrometer/spectrometercolordataprovider.h"

using namespace std;


SpectrometerColorDataProvider::SpectrometerColorDataProvider(LedConfig ledConfig, int fps, float gain) : ColorDataProvider(ledConfig) {
    mHelper = new SpectrometerHelper(ledConfig, fps, gain);
}

SpectrometerColorDataProvider::~SpectrometerColorDataProvider(){
    delete mHelper;
    mHelperThread.join();
}

void SpectrometerColorDataProvider::start(){
    // start helper in separate thread
    mHelperThread = thread(&SpectrometerHelper::start, mHelper);
}

float SpectrometerColorDataProvider::getData(uint8_t *data){
    return mHelper->getData(data);
}
