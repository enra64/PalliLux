#ifndef SPECTROMETERRGBLINEPROVIDER_H
#define SPECTROMETERRGBLINEPROVIDER_H

#include "rgblineprovider.h"

#include "fftw3.h"

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/pulseaudio.h>

#include <cmath>

class SpectrometerRgbLineProvider : public RgbLineProvider
{
public:
    SpectrometerRgbLineProvider(size_t xLeds, size_t yLeds, int fps = 30, float gain = 1.f);

    ~SpectrometerRgbLineProvider();
private:
    const int mFramesPerSecond;///
    const double UPPER_FREQUENCY = 3520;
    pa_sample_spec mSampleSpecifications; ///!< pulseaudio sampling specifcations
    float* mPulseAudioBuffer = nullptr; ///!< pulseaudio output buffer
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

    // RgbLineProvider interface
public:
    float getData(uint8_t *data);
};

#endif // SPECTROMETERRGBLINEPROVIDER_H
