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
    float mGain = 1.f; ///< gain factor. applied before log10(), so the effect is limited

    // variables const for an instance
    const int FRAMES_PER_SECOND;///< FPS of the spectrogram.
    const double UPPER_FREQUENCY = 3520;///< highest displayed frequency
    const int OVERALL_LED_COUNT;///< how many leds does the user have
    const int SIZE;///< mystery variable


    uint8_t* mLedBuffer;///< internal buffer for saving the led data

    volatile bool mKeepRunning = true;///< true if the working loop should continue

    /**
     * @brief calculate amplitude values given a fft result for a single channel
     *
     * for more explanation see https://groups.google.com/d/msg/comp.dsp/cZsS1ftN5oI/rEjHXKTxgv8J
     * also, https://gitlab.com/nitroxis/pasa/blob/master/pasa.c was most helpful
     *
     * @param fft the fft result to work with
     * @param bars our result buffer. length is determined through mLedsPerChannel
     */
    void calculateAmplitude(fftw_complex* fft, uint8_t *bars);

    int mLedOffset;///< how far off the start the spectrum should begin
    int mLedsPerChannel;///< how many leds are in each of the 2 channels
    int mSelectedLedCount;///< how many leds are in the spectrum


    /**
     * @brief Apply the calculated amplitudes to the led buffer
     * @param barsL the amplitudes of the left channel
     * @param barsR the amplitudes of the right channel
     */
    void applyAmplitudes(uint8_t *barsL, uint8_t *barsR);

    /**
     * @brief Calculate the hanning window.
     * @param n dunno
     * @param N dunno
     * @return dunno
     */
    float windowFunction(int n, int N) {
        return 0.5f * (1.0f - cosf(2.0f * M_PI * n / (N - 1.0f)));
    }
public:
    /**
     * @brief Change the parameters of this algorithm
     * @param ledOffset how far off the start of your led strip the spectrogram should start
     * @param numberOfLedsPerStereoChannel how many leds each channel should have
     * @param gain a gain factor. applied before a logarithm, so the effect is limited
     */
    void setParameters(int ledOffset, int numberOfLedsPerStereoChannel, double gain);

    /**
     * @brief Get the led data from an internal buffer
     * @param data the target array
     * @return the time it took to copy the array
     */
    float getData(uint8_t *data);

    /**
     * @brief Start the spectrometer. This function is blocking, you probably want to call it in a thread.
     */
    void start();
};

#endif // SPECTROMETERCOLORDATAPROVIDER_H
