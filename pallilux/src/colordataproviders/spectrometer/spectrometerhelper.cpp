#include "spectrometer/spectrometerhelper.h"

#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <algorithm>

using namespace std;

SpectrometerHelper::SpectrometerHelper(LedConfig ledConfig, int fps, float gain) :
    mGain(gain),
    FRAMES_PER_SECOND(fps),
    OVERALL_LED_COUNT(ledConfig.getLedCount()),
    SIZE(44100 / fps) {
    // allocate and zero out internal led data buffer
    mLedBuffer = new uint8_t[OVERALL_LED_COUNT * 3];
    memset(mLedBuffer, 0, OVERALL_LED_COUNT * 3);
}

void SpectrometerHelper::start() {
    // set sample specs
    pa_sample_spec mSampleSpecifications;
    mSampleSpecifications.channels = 2;
    mSampleSpecifications.format = PA_SAMPLE_FLOAT32LE;
    mSampleSpecifications.rate = 44100;

    // open record device
    int error;
    pa_simple* pulseAudioDevice = pa_simple_new(
                NULL,                                   // default server
                "Pallilux",                             // application name
                PA_STREAM_RECORD,
                "2",                                   // default device
                "Data source for the AmbiSpectrometer", // description
                &mSampleSpecifications,                 // sample specs
                NULL,                                   // default channel map
                NULL,                                   // default buffering attributes
                &error);

    // check for errors while opening the device
    if(!pulseAudioDevice)
        throw runtime_error("pa_simple_new() failed:" + string(pa_strerror(error)));

    // calculate the hanning window we use for scaling or normalizing or whatever the data from pulseaudio
    float window[SIZE];
    for(int n = 0; n < SIZE; n++)
        window[n] = windowFunction(n, SIZE);

    // fftw setup - use fftw_malloc to conform to fftw algorithm alignment requirements
    double* mFftwIn = (double*) fftw_malloc(sizeof(double) * SIZE);
    fftw_complex* fftwOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SIZE);
    fftw_plan fftwPlan = fftw_plan_dft_r2c_1d(SIZE, mFftwIn, fftwOut, FFTW_MEASURE);

    // result buffer for bin amplitudes
    uint8_t barsL[mLedsPerChannel];
    uint8_t barsR[mLedsPerChannel];

    // pulseaudio read buffer
    float pulseAudioBuffer[mSampleSpecifications.channels * SIZE];

    // this is the actual workhorse
    while(mKeepRunning) {
        // try to read from the audio device
        int err = 0;
        if(pa_simple_read(pulseAudioDevice, pulseAudioBuffer, sizeof(pulseAudioBuffer), &err) < 0) {
            pa_simple_free(pulseAudioDevice);
            throw std::runtime_error("pa_simple_read() failed: " + string(pa_strerror(err)));
        }

        // calculate amplitudes of left input.
        for(int i = 0; i < SIZE; i++) mFftwIn[i] = (double) (window[i] * pulseAudioBuffer[i * 2]);
        fftw_execute(fftwPlan);
        calculateAmplitude(fftwOut, barsL);

        // calculate amplitudes of right input.
        for(int i = 0; i < SIZE; i++) mFftwIn[i] = (double) (window[i] * pulseAudioBuffer[i * 2 + 1]);
        fftw_execute(fftwPlan);
        calculateAmplitude(fftwOut, barsR);

        // save the amplitudes to the led buffer
        applyAmplitudes(barsL, barsR);
    }

    try {
        // clean up fftw
        fftw_destroy_plan(fftwPlan);
        fftw_free(mFftwIn);
        fftw_free(fftwOut);
        delete[] mLedBuffer;

        // clean up pulseaudio
        pa_simple_free(pulseAudioDevice);
    } catch(std::exception){
    }
}

void SpectrometerHelper::applyAmplitudes(uint8_t* barsL, uint8_t* barsR){
    // copy over the amplitude data to
    for(int i = 0; i < mLedsPerChannel; i++){
        mLedBuffer[(mLedOffset + i) * 3] = barsR[i];
        mLedBuffer[(mLedsPerChannel + mLedOffset + i) * 3] = barsL[mLedsPerChannel - 1 - i];
    }
}

void SpectrometerHelper::setParameters(int ledOffset, int numberOfLedsPerStereoChannel, double gain){
    memset(mLedBuffer, 0, OVERALL_LED_COUNT * 3);

    // how many leds are currently activated
    mSelectedLedCount = 2 * numberOfLedsPerStereoChannel;

    // directly store the three variables
    mLedsPerChannel = numberOfLedsPerStereoChannel;
    mLedOffset = ledOffset;
    mGain = gain;
}

float SpectrometerHelper::getData(uint8_t *data) {
    // benchmarking
    clock_t start = clock();

    // copy over the led data
    memcpy(data, mLedBuffer, OVERALL_LED_COUNT * 3);

    this_thread::sleep_for(chrono::milliseconds(9));

    // return benchmarking value
    return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

SpectrometerHelper::~SpectrometerHelper() {
    // stop the running thread
    mKeepRunning = false;
}

void SpectrometerHelper::calculateAmplitude(fftw_complex *fft, uint8_t *amplitudes) {
    // calculate bar width using black magic
    double ledWidth = UPPER_FREQUENCY / (FRAMES_PER_SECOND * mLedsPerChannel);

    // scale variable. determined through gain, a magic number, and sprinkled with a bit of dark magic
    double scale = 200.0 / SIZE * mGain;

    // assert minimum bar width
    assert(ledWidth > 0);

    // index of the fft values
    int fftIndex = 0;

    // interpolate led values by calculating an average over the bin magnitudes
    for(int ledIndex = 0; ledIndex < mLedsPerChannel; ledIndex++)
    {
        double magnitudeSum = 0.0;
        // sum all bin magnitudes for this led
        for(int i = 0; i < ledWidth && fftIndex < SIZE; fftIndex++, i++)
            // calculate magnitude of this fft result item
            magnitudeSum += pow(fft[fftIndex][0] * scale, 2) + pow(fft[fftIndex][1] * scale, 2);

        // average the magnitude (divzero safe)
        magnitudeSum *= (1.0 / ledWidth);

        // clamp to prevent overflows
        if(magnitudeSum < 1e-15) magnitudeSum = 1e-15;

        // calculate average from the sum of normalized bin magnitudes
        int dB = (int) 100.0 * log10(magnitudeSum);

        // clamp to uint8 range
        if(dB > 255) dB = 255;
        if(dB < 0) dB = 0;

        // save to out buffer
        amplitudes[ledIndex] = dB;
    }
}
