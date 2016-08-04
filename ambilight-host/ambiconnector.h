#ifndef LEDCONNECTOR_H
#define LEDCONNECTOR_H

#include "rgbconstructor.h"
#include "ledexceptions.h"

/**
 * @brief The LedConnector class is the master class. It interfaces with the RGB leds and obtains the image data.
 */
class AmbiConnector {
public:
    /**
     * @brief update update screen data
     */
    void update();

    /**
     * @brief draw push current data to leds
     */
    void draw();

    /**
     * @brief connect connect to the arduino; because it waits for established comms, it is blocking
     * @return true if connection establishment successfull
     */
    bool connect(const std::__cxx11::string &port);

    /**
     * @brief getCurrentFps calculate and return the current fps, measured by completed draw calls
     * @return current fps
     */
    inline float getCurrentFps() {
        return mCurrentFps;
    }

    /**
     * @brief LedConnector construct a LedConnector, responsible for obtaining and updating the led data
     * @param borderProvider will be used to obtain the border images
     * @param horizontalLedCount how many leds are on each horizontal border
     * @param verticalLedCount how many leds are on each vertical border
     */
    AmbiConnector(std::shared_ptr<BorderProvider> borderProvider, unsigned int horizontalLedCount, unsigned int verticalLedCount);

    ~AmbiConnector();
private:

    /**
     * @brief mRgbConverter the object that provides the RGB uint8_t led data
     */
    std::unique_ptr<RgbConverter> mRgbConverter;

    /**
     * @brief mRgbBuffer our very own bufferspace for led data
     */
    uint8_t* mRgbBuffer;

    /**
     * @brief mCommBuffer smaller buffer for arduino responses
     */
    char mCommBuffer[128];

    /**
     * @brief mSerialFd serial connection file descriptor
     */
    int mSerialFd = -1;

    /**
     * @brief writeRgbBufferToText Another debug function; can be used to print out the led buffer to a text file
     * @param buffer the buffer to write; uint8_t RGB-triplets are expected
     * @param length the buffer length in bytes (number of uint8_t)
     * @param path where to write to
     */
    void writeRgbBufferToText(std::string path);

    /**
     * @brief waitForSerialInput this function implements non-busy waiting for a response on the serial file descriptor
     */
    void waitForSerialInput();

    /**
     * @brief mLastDraw a timestamp of when the last draw was executed
     */
    clock_t mLastDraw;

    /**
     * @brief mCurrentFps current fps, saved so the timekeeping code can always be called at the same moment
     */
    float mCurrentFps;

    /**
     * @brief updateFps update "::"<mCurrentFps> and "::"<mLastDraw>
     */
    void updateFps();
};

#endif // LEDCONNECTOR_H
