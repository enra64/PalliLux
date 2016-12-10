#include "spectrometer/spectrometerhelper.h"

#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <algorithm>

using namespace std;

// hanning window. // i have no idea
// something like https://en.wikipedia.org/wiki/Window_function#Hann_.28Hanning.29_window
float windowFunction(int n, int N) {
    return 0.5f * (1.0f - cosf(2.0f * M_PI * n / (N - 1.0f)));
}

SpectrometerHelper::SpectrometerHelper(LedConfig ledConfig, int fps, float gain) : mLedConfig(ledConfig), mFramesPerSecond(fps), mGain(gain)
{
    OVERALL_LED_COUNT = ledConfig.getLedCount();
    // set sample specs
    mSampleSpecifications.channels = 2;
    mSampleSpecifications.format = PA_SAMPLE_FLOAT32LE;
    mSampleSpecifications.rate = 44100;

    mSize = mSampleSpecifications.rate / fps;

    mDataBuffer = new uint8_t[OVERALL_LED_COUNT * 3];
    memset(mDataBuffer, 0, OVERALL_LED_COUNT * 3);
}

void SpectrometerHelper::start() {
    // open record device
    int error;
    mPulseAudioDevice = pa_simple_new(
                NULL,                                   // default server
                "Pallilux",                             // application name
                PA_STREAM_RECORD,
                "2",                                   // default device
                "Data source for the AmbiSpectrometer", // description
                &mSampleSpecifications,                 // sample specs
                NULL,                                   // default channel map
                NULL,                                   // default buffering attributes
                &error);

    // check for errors
    if(!mPulseAudioDevice)
        throw runtime_error("pa_simple_new() failed:" + string(pa_strerror(error)));

    // allocate buffers
    float mPulseAudioBuffer[mSampleSpecifications.channels * mSize];
    float mWindow[mSize];

    for(int n = 0; n < mSize; n++)
        mWindow[n] = windowFunction(n, mSize);

    // fftw setup - use fftw_malloc to conform to fftw algorithm alignment requirements
    double* mFftwIn = (double*) fftw_malloc(sizeof(double) * mSize);
    fftw_complex* fftwOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * mSize);
    fftw_plan fftwPlan = fftw_plan_dft_r2c_1d(mSize, mFftwIn, fftwOut, FFTW_MEASURE);

    while(mKeepRunning) {
        uint8_t barsL[mLedsPerChannel];
        uint8_t barsR[mLedsPerChannel];

        // try to read from the audio device
        int err = 0;
        if(pa_simple_read(mPulseAudioDevice, mPulseAudioBuffer, sizeof(mPulseAudioBuffer), &err) < 0) {
            pa_simple_free(mPulseAudioDevice);
            throw std::runtime_error("pa_simple_read() failed: " + string(pa_strerror(err)));
        }

        // calculate amplitudes of left input.
        for(int i = 0; i < mSize; i++)
            mFftwIn[i] = (double) (mWindow[i] * mPulseAudioBuffer[i * 2]);
        fftw_execute(fftwPlan);
        calculateAmplitude(fftwOut, mSize, barsL, mLedsPerChannel);

        // calculate amplitudes of right input.
        for(int i = 0; i < mSize; i++)
            mFftwIn[i] = (double) (mWindow[i] * mPulseAudioBuffer[i * 2 + 1]);
        fftw_execute(fftwPlan);
        calculateAmplitude(fftwOut, mSize, barsR, mLedsPerChannel);

        // make more red the higher the amplitude
        for(int i = 0; i < mLedsPerChannel; i++){
            mDataBuffer[(mLedOffset + i) * 3] = barsR[i];
            mDataBuffer[(mLedsPerChannel + mLedOffset + i) * 3] = barsL[mLedsPerChannel - 1 - i];
        }
    }

    try {
        // clean up fftw
        fftw_destroy_plan(fftwPlan);
        fftw_free(mFftwIn);
        fftw_free(fftwOut);

        // clean up pulseaudio
        pa_simple_free(mPulseAudioDevice);
    } catch(std::exception){
    }
}

float SpectrometerHelper::getData(uint8_t *data) {
    // benchmarking
    clock_t start = clock();

    // copy over the led data
    memcpy(data, mDataBuffer, OVERALL_LED_COUNT * 3);

    this_thread::sleep_for(chrono::milliseconds(9));

    // return benchmarking value
    return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

SpectrometerHelper::~SpectrometerHelper() {
    // stop the running thread
    mKeepRunning = false;
}

void SpectrometerHelper::calculateAmplitude(
        fftw_complex *fft,
        int fftSize,
        uint8_t *amplitudes,
        int numLeds)
{
    // for more explanation see https://groups.google.com/d/msg/comp.dsp/cZsS1ftN5oI/rEjHXKTxgv8J
    // also, https://gitlab.com/nitroxis/pasa/blob/master/pasa.c was most helpful

    // calculate bar width using black magic
    double ledWidth = UPPER_FREQUENCY / (mFramesPerSecond * numLeds);

    double scale = 200.0 / fftSize * mGain;

    // assert minimum bar width
    assert(ledWidth > 0);

    // index of the fft values
    int fftIndex = 0;

    // interpolate led values by calculating an average over the bin magnitudes
    for(int ledIndex = 0; ledIndex < numLeds; ledIndex++)
    {
        double magnitudeSum = 0.0;
        // sum all bin magnitudes for this led
        for(int i = 0; i < ledWidth && fftIndex < fftSize; fftIndex++, i++) {
            // calculate squares of real and imaginary fft result parts
            double real = pow(fft[fftIndex][0] * scale, 2);
            double imaginary = pow(fft[fftIndex][1] * scale, 2);
            // normalized bin magnitude
            magnitudeSum += real + imaginary;
        }

        // calculate average (divzero safe)
        magnitudeSum *= (1.0 / ledWidth);

        // prevent overflows
        if(magnitudeSum < 1e-15) magnitudeSum = 1e-15;

        // calculate average from the sum of normalized bin magnitudes
        int dB = (int) 100.0 * log10(magnitudeSum);

        // clamp to uint8 range
        if(dB > 255) dB = 255;
        if(dB < 0) dB = 0;

        // compute decibels.
        amplitudes[ledIndex] = dB;
    }
}
