#ifndef PALLILUX_ABSTRACTCOLORMAPPER_H
#define PALLILUX_ABSTRACTCOLORMAPPER_H

class AbstractColorMapper {
private:

    int mLedCount;
public:
    AbstractColorMapper(int ledCount) {
        mLedCount = ledCount;
    }


    virtual void map(uint8_t amplitude, int position, uint8_t* r, uint8_t* g, uint8_t* b);
};

#endif //PALLILUX_ABSTRACTCOLORMAPPER_H
