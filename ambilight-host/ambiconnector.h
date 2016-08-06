#ifndef LEDCONNECTOR_H
#define LEDCONNECTOR_H

#include "rgbconverter.h"
#include "ledexceptions.h"

/**
 * @brief The AmbiConnector retrieves the image data from a BorderProvider and sends it to an Arduino.
 */
class AmbiConnector {
public:
    /**
     * @brief update screen data
     */
    void update();

    /**
     * @brief push current data to leds
     */
    void draw();

    /**
     * @brief connect to the arduino; because it waits for established comms, it is blocking
     * @return true if connection establishment successfull
     */
    bool connect(const std::__cxx11::string &port);

    /**
     * @brief calculate and return the current fps, measured by completed draw calls
     * @return current fps
     */
    inline float getCurrentFps() {
        return mCurrentFps;
    }

    /**
     * @brief construct an AmbiConnector, responsible for obtaining and updating the led data
     * @param borderProvider will be used to obtain the border images
     * @param horizontalLedCount how many leds are on each horizontal border
     * @param verticalLedCount how many leds are on each vertical border
     */
    AmbiConnector(std::shared_ptr<BorderProvider> borderProvider, unsigned int horizontalLedCount, unsigned int verticalLedCount);

    ~AmbiConnector();

    /**
     * @brief Close the connection. Optionally, the leds may be shut down
     * @param blackoutLeds whether an all-black signal should be sent before disconnecting
     */
    void disconnect(bool blackoutLeds);

    /**
     * @brief Add a datafilter (brightness scaler, gliding filter)
     * @param id how to reference to the filter
     * @param filter the filter itself; the rgb converter will have sole ownership.
     */
    void addFilter(std::string id, std::unique_ptr<DataFilter> filter);

    /**
     * @brief Remove a filter from the list
     * @param id filter name
     * @return the filter
     */
    std::unique_ptr<DataFilter> removeFilter(std::string id);

    /**
     * @brief Retrieve the bufferspace required for a rgb dataset
     * @return number of bytes required
     */
    size_t getRequiredBufferLength(){
        return mRgbConverter->getRequiredBufferLength();
    }

private:

    /**
     * @brief Another debug function; can be used to print out the led buffer to a text file
     * @param buffer the buffer to write; uint8_t RGB-triplets are expected
     * @param length the buffer length in bytes (number of uint8_t)
     * @param path where to write to
     */
    void writeRgbBufferToText(std::string path);

    /**
     * @brief this function implements non-busy waiting for a response on the serial file descriptor
     */
    void waitForSerialInput();

    /**
     * @brief update "::"<mCurrentFps> and "::"<mLastDraw>
     */
    void updateFps();

    std::unique_ptr<RgbConverter> mRgbConverter;//!< the object that provides the RGB led data as a uint8_t array

    uint8_t* mRgbBuffer;//!< our very own bufferspace for led data

    char mCommBuffer[128];//!< buffer for arduino responses

    int mSerialFd = -1;//!< serial connection file descriptor

    clock_t mLastDraw;//!< a timestamp of when the last draw was executed

    float mCurrentFps;//!< current fps, saved so the timekeeping code can always be called at the same moment


};

#endif // LEDCONNECTOR_H
