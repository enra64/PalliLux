#ifndef LEDCONNECTOR_H
#define LEDCONNECTOR_H

#include "ambirgblineprovider.h"
#include "customexceptions.h"
#include <string>

/**
 * @brief The AmbiConnector retrieves the image data from a BorderProvider and sends it to an Arduino.
 */
class ArduinoConnector {
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
     * @brief connect to the arduino; because it waits for established comms, it is blocking. An exception will be thrown on failure.
     * @param port the tty device to use
     */
    void connect(const std::__cxx11::string &port);

    /**
     * @brief connect to the arduino; because it waits for established comms, it is blocking.
     * It is assumed the tty device has been set. An exception will be thrown on failure.
     */
    void connect();

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
     */
    ArduinoConnector(std::unique_ptr<RgbLineProvider> borderProvider);

    /**
     * @brief construct an AmbiConnector, responsible for obtaining and updating the led data
     * @param borderProvider will be used to obtain the border images
     * @param port tty port for comms
     */
    ArduinoConnector(std::unique_ptr<RgbLineProvider> borderProvider, const std::string port);

    ~ArduinoConnector();

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
     * @brief getFilter get a filter for editing
     * @param id filter name
     * @return reference to the filter
     */
    std::unique_ptr<DataFilter>& getFilter(std::string id);

    /**
     * @brief Retrieve the bufferspace required for a rgb dataset
     * @return number of bytes required
     */
    size_t getRequiredBufferLength(){
        return mRgbLineProvider->getRequiredBufferLength();
    }

    /**
     * @brief Set a target as to what fps should be achieved
     * @param fps target fps in seconds. if lower than 0, the target is reset.
     */
    void setTargetFps(float fps){
        mTargetFps = fps;
    }

    /**
     * @brief Get current target fps
     * @return current target fps in seconds
     */
    float getTargetFps() const {
        return mTargetFps;
    }

    /**
     * @brief Set the port that will be used in the next connection attempt
     * @param port device name, like /dev/tty0
     */
    void setPort(const std::string& port){
        mTtyDevice = port;
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

    std::unique_ptr<RgbLineProvider> mRgbLineProvider;//!< the object that provides the RGB led data as a uint8_t array

    uint8_t* mRgbBuffer;//!< our very own bufferspace for led data

    char mCommBuffer[128];//!< buffer for arduino responses

    int mSerialFd = -1;//!< serial connection file descriptor

    clock_t mLastDraw;//!< a timestamp of when the last draw was executed

    float mCurrentFps;//!< current fps, saved so the timekeeping code can always be called at the same moment

    float mTargetFps = -1;//!< target fps; important when using spectrometer

    std::string mTtyDevice;///!< arduino connection name
};

#endif // LEDCONNECTOR_H
