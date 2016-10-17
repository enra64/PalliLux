#ifndef SPECTROMETERCOLORDATAPROVIDER_H
#define SPECTROMETERCOLORDATAPROVIDER_H

#include "colordataprovider.h"

#include "fftw3.h"

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/pulseaudio.h>

#include <cmath>

class SpectrometerColorDataProvider : public ColorDataProvider
{
public:
    SpectrometerColorDataProvider(LedConfig ledConfig, int fps = 30, float gain = 1.f);

    ~SpectrometerColorDataProvider();
private:
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

    void calculateAmplitude(fftw_complex* fft, int fftSize, double *bars, int numLeds);

    // ColorDataProvider interface
public:
    float getData(uint8_t *data);
};

#endif // SPECTROMETERCOLORDATAPROVIDER_H
