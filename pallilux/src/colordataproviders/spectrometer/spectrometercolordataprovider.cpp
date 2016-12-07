#include "spectrometer/spectrometercolordataprovider.h"

using namespace std;


SpectrometerColorDataProvider::SpectrometerColorDataProvider(LedConfig ledConfig, int fps, float gain) : ColorDataProvider(ledConfig) {
    mHelper = new SpectrometerHelper(ledConfig, fps, gain);
}

SpectrometerColorDataProvider::~SpectrometerColorDataProvider(){
    delete mHelper;
}

void SpectrometerColorDataProvider::start(){
    mHelper->start();
}

float SpectrometerColorDataProvider::getData(uint8_t *data){
    return mHelper->getData(data);
}
