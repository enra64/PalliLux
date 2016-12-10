#ifndef SPECTROMETERHELPER_H
#define SPECTROMETERHELPER_H

#include "fftw3.h"

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/pulseaudio.h>

#include <cstring>
#include <cmath>
#include <colordataprovider.h>

class SpectrometerHelper
{
public:
    SpectrometerHelper(LedConfig ledConfig, int fps = 60, float gain = 1.f);

    ~SpectrometerHelper();
private:
    LedConfig mLedConfig;
    const int mFramesPerSecond;///< FPS of the spectrogram.
    const double UPPER_FREQUENCY = 3520;///< highest displayed frequency
    pa_sample_spec mSampleSpecifications; ///< pulseaudio sampling specifcations
    int mSize;
    float mGain = 1.f;

    // pulse audio
    pa_simple* mPulseAudioDevice;

    uint8_t* mDataBuffer;

    volatile bool mKeepRunning = true;

    void calculateAmplitude(fftw_complex* fft, int fftSize, uint8_t *bars, int numLeds);

    int mLedOffset;
    int mLedsPerChannel;
    int mSelectedLedCount;
    int OVERALL_LED_COUNT;

    // ColorDataProvider interface
public:
    void setParameters(int ledOffset, int numberOfLedsPerStereoChannel, double gain){
        memset(mDataBuffer, 0, OVERALL_LED_COUNT * 3);
        mLedOffset = (OVERALL_LED_COUNT - 2 * numberOfLedsPerStereoChannel) / 2;
        mSelectedLedCount = 2 * numberOfLedsPerStereoChannel;
        mLedsPerChannel = numberOfLedsPerStereoChannel;
        mLedOffset = ledOffset;
        mGain = gain;
    }

    float getData(uint8_t *data);
    void start();
};

#endif // SPECTROMETERCOLORDATAPROVIDER_H
