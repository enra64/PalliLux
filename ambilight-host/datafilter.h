#ifndef DATAFILTER_H
#define DATAFILTER_H

/**
 * @brief An interface for functors implementing data filters for the rgb data, like brightness factoring or low-pass filters
 */
class DataFilter {
public:
    /**
     * @brief This operator will be applied if added to an RgbConverter
     * @param rgbData the data modifiable data
     */
    virtual void operator()(uint8_t* rgbData, size_t dataLength);
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
        for(int i = 0; i < dataLength; i++)
            rgbData[i] *= mFactor;
    }
};

class LowPassFilter : public DataFilter {


    // DataFilter interface
public:
    void operator ()(uint8_t *rgbData, size_t dataLength) override{

    }
};

#endif // DATAFILTER_H
