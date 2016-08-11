#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <cstring>

/**
 * @brief An interface for functors implementing data filters for the rgb data, like brightness factoring or low-pass filters
 */
class DataFilter {
public:
    /**
     * @brief This operator will be applied if added to an RgbConverter
     * @param rgbData the data modifiable data
     */
    virtual void operator()(uint8_t* rgbData, size_t dataLength) = 0;
};

class BrightnessFilter : public DataFilter {

    const float mFactor; ///!< the factor that will be applied to any given data

public:
    /**
     * @brief Construct a BrightnessFilter with the given factor
     * @param factor the float factor that will be applied. Values
     */
    BrightnessFilter(float factor) : mFactor(factor) {
    }

    void operator ()(uint8_t *rgbData, size_t dataLength) override {
        for(size_t i = 0; i < dataLength; i++)
            rgbData[i] *= mFactor;
    }
};

class LowPassFilter : public DataFilter {
    uint8_t* mBuffer;///!< rgb data buffer
    float mNewDataFactor;///!< amount that the new data will take in filter application
    // DataFilter interface
public:
    /**
     * @brief Create somewhat of a gliding filter
     * @param bufferLength how large the acquired buffer space must be
     * @param newFactor the factor by which the new value should be used; old will use (1-newFactor)
     */
    LowPassFilter(size_t bufferLength, float newDataFactor = .9f) : mNewDataFactor(newDataFactor) {
        mBuffer = new uint8_t[bufferLength];
    }

    /**
      * Release buffer
      */
    ~LowPassFilter(){
        delete[] mBuffer;
    }

    void operator ()(uint8_t *rgbData, size_t dataLength) override {
        // replace the rgb data with weighted old + new data
        for(size_t i = 0; i < dataLength; i++)
            rgbData[i] = mNewDataFactor * rgbData[i] + (1 - mNewDataFactor) * mBuffer[i];


        // set buffer to latest value
        memcpy(mBuffer, rgbData, dataLength);
    }
};

class LinearFactorFilter : public DataFilter {
    float getFactor(uint8_t point){
        if(point < 172)
            return 1.f;
        return (255 - point);
    }

public:
    void operator ()(uint8_t *rgbData, size_t dataLength) override {
        for(size_t i = 0; i < dataLength; i++)
            rgbData[i] *= getFactor(rgbData[i]);
    }
};

#endif // DATAFILTER_H
