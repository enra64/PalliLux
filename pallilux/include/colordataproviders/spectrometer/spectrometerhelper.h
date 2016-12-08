#ifndef SPECTROMETERHELPER_H
#define SPECTROMETERHELPER_H

#include "fftw3.h"

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/pulseaudio.h>

#include <cmath>
#include <colordataprovider.h>

class SpectrometerHelper
{
public:
    SpectrometerHelper(LedConfig ledConfig, int fps = 30, float gain = 1.f);

    ~SpectrometerHelper();
private:
    LedConfig mLedConfig;
    const int mFramesPerSecond;///< FPS of the spectrogram.
    const double UPPER_FREQUENCY = 3520;///< highest displayed frequency
    pa_sample_spec mSampleSpecifications; ///< pulseaudio sampling specifcations
    int mSize;
    float mGain;

    // pulse audio
    pa_simple* mPulseAudioDevice;

    uint8_t* mDataBuffer;


    volatile bool mKeepRunning = true;

    void calculateAmplitude(fftw_complex* fft, int fftSize, uint8_t *bars, int numLeds);

    int LED_COUNT;

    // ColorDataProvider interface
public:
    float getData(uint8_t *data);
    void start();
};

#endif // SPECTROMETERCOLORDATAPROVIDER_H
