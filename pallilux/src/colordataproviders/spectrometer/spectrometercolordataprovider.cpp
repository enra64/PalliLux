#include "spectrometer/spectrometercolordataprovider.h"

using namespace std;

SpectrometerColorDataProvider::SpectrometerColorDataProvider(LedConfig ledConfig, int fps, float gain) :
    ColorDataProvider(ledConfig),
    mGain(gain),
    FRAMES_PER_SECOND(fps),
    OVERALL_LED_COUNT(ledConfig.getLedCount()),
    SIZE(44100 / fps) {

    // allocate and zero out internal led data buffer
    mLedBuffer = new uint8_t[OVERALL_LED_COUNT * 3];
    memset(mLedBuffer, 0, OVERALL_LED_COUNT * 3);

    // initialize automatic gain control
    mAutoGainController = new AutomaticGainControl(OVERALL_LED_COUNT);
}

SpectrometerColorDataProvider::~SpectrometerColorDataProvider(){
    mKeepRunning = false;

    if(mHelperThread != nullptr)
        mHelperThread->join();

    if(mAutoGainController != nullptr)
        delete mAutoGainController;
}

void SpectrometerColorDataProvider::start(){
    // start helper in separate thread
    mHelperThread = new thread(&SpectrometerColorDataProvider::start_, this);
}

float SpectrometerColorDataProvider::getData(uint8_t *data){
    // benchmarking
    clock_t start = clock();

    // copy over the led data
    memcpy(data, mLedBuffer, OVERALL_LED_COUNT * 3);

    this_thread::sleep_for(chrono::milliseconds(9));

    applyFilters(data);

    // return benchmarking value
    return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

void SpectrometerColorDataProvider::setParameters(int ledOffset, int numberOfLedsPerStereoChannel, double gain){
    // store updated parameters
    mLedsPerChannel = numberOfLedsPerStereoChannel;
    mLedOffset = ledOffset;
    mGain = gain;

    // erase unused leds
    memset(mLedBuffer, 0, ledOffset * 3);// before used leds
    uint8_t* afterPointer = mLedBuffer + 3 * (ledOffset + mLedsPerChannel * 2), *endPointer = mLedBuffer + OVERALL_LED_COUNT * 3;
    memset(afterPointer, 0, endPointer - afterPointer);

    // how many leds are currently activated
    mSelectedLedCount = 2 * numberOfLedsPerStereoChannel;   
}

void SpectrometerColorDataProvider::start_() {
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
    // apparently, hanning window is a window function: a function zero outside of some desired range, and
    // having whatever fucking form we want inside because we can.
    // we calculate it here, so we only need to do it once, and can later just multiply it
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

    // result buffer for bin amplitudes with constant gain (for beat detection)
    int constantGainBarsL[mLedsPerChannel];
    int constantGainBarsR[mLedsPerChannel];

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
        calculateAmplitude(fftwOut, barsL, mGain);

        // calculate amplitudes of right input.
        for(int i = 0; i < SIZE; i++) mFftwIn[i] = (double) (window[i] * pulseAudioBuffer[i * 2 + 1]);
        fftw_execute(fftwPlan);
        calculateAmplitude(fftwOut, barsR, mGain);

        // do AGC if enabled
        if (mAutoGain)
            mGain = mAutoGainController->getGain(barsL, mLedsPerChannel, barsR, mLedsPerChannel);

        // save the amplitudes to the led buffer
        mapAmplitudes(barsL, barsR);
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

void SpectrometerColorDataProvider::setAgcEnabled(bool enabled)
{
    mAutoGain = enabled;
}

void SpectrometerColorDataProvider::mapAmplitudes(uint8_t* barsL, uint8_t* barsR) {
    // copy over the amplitude data to our led buffer
    for(int i = 0; i < mLedsPerChannel; i++){
        mColorMapper->map(barsL, mLedsPerChannel, mLedBuffer + 3 * mLedsPerChannel + 3 * mLedOffset, true);
        mColorMapper->map(barsR, mLedsPerChannel, mLedBuffer + 3 * mLedOffset, false);
    }
}

void SpectrometerColorDataProvider::calculateAmplitude(fftw_complex *fft, uint8_t *amplitudes, double gain) {
    // calculate bar width using black magic
    double ledWidth = UPPER_FREQUENCY / (FRAMES_PER_SECOND * mLedsPerChannel);

    // scale variable. determined through gain, a magic number, and sprinkled with a bit of dark magic
    double scale = (gain * 200.0) / SIZE;

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
