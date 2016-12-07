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
    float* mPulseAudioBuffer = nullptr; ///< pulseaudio output buffer
    float* mWindow = nullptr;
    int mSize;
    float mGain;

    // fft
    double *mFftwIn;
    fftw_complex *mFftwOut;
    fftw_plan mFftwPlan;

    // pulse audio
    pa_simple* mPulseAudioDevice;

    volatile bool mKeepRunning = true;

    void calculateAmplitude(fftw_complex* fft, int fftSize, double *bars, int numLeds);

    uint8_t* mBuffer;

    int LED_COUNT;

    // ColorDataProvider interface
public:
    float getData(uint8_t *data);
    void start();
};

#endif // SPECTROMETERCOLORDATAPROVIDER_H
