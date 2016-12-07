#ifndef SPECTROMETERCOLORDATAPROVIDER_H
#define SPECTROMETERCOLORDATAPROVIDER_H

#include "colordataprovider.h"

#include "fftw3.h"
#include "spectrometerhelper.h"

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/pulseaudio.h>

#include <thread>
#include <cmath>

class SpectrometerColorDataProvider : public ColorDataProvider
{
public:
    SpectrometerColorDataProvider(LedConfig ledConfig, int fps = 30, float gain = 1.f);
    ~SpectrometerColorDataProvider();
private:
    SpectrometerHelper* mHelper;
    std::thread mHelperThread;

    // ColorDataProvider interface
public:
    void start();
    float getData(uint8_t *data);
};

#endif // SPECTROMETERCOLORDATAPROVIDER_H
