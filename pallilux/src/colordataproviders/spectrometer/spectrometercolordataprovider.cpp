#include "spectrometer/spectrometercolordataprovider.h"

using namespace std;


SpectrometerColorDataProvider::SpectrometerColorDataProvider(LedConfig ledConfig, int fps, float gain) : ColorDataProvider(ledConfig) {
    mHelper = new SpectrometerHelper(ledConfig, fps, gain);
}

SpectrometerColorDataProvider::~SpectrometerColorDataProvider(){
    delete mHelper;
    if(mHelperThread != nullptr)
        mHelperThread->join();
}

void SpectrometerColorDataProvider::start(){
    // start helper in separate thread
    mHelperThread = new thread(&SpectrometerHelper::start, mHelper);
}

float SpectrometerColorDataProvider::getData(uint8_t *data){
    return mHelper->getData(data);
}
