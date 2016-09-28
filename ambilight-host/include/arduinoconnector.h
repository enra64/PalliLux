#ifndef LEDCONNECTOR_H
#define LEDCONNECTOR_H

#include <string>
#include <assert.h>

#include "ambicolordataprovider.h"
#include "serial.h"

/**
 * @brief The ArduinoConnector retrieves the image data from a ColorDataProvider and sends it to an Arduino.
 *
 * A ColorDataProvider has to be set, and you must call either setPort() or use #connect(const std::string&) to set the port.
 * The amount of required space (in bytes) for frame data set can be retrieved using #getRequiredBufferLength()
 */
class ArduinoConnector {
public:
    /**
     * @brief Construct an ArduinoConnector. A ColorDataProvider has to be set using setColorDataProvider().
     */
    ArduinoConnector();

    /**
     * @brief Empty virtual destructor to ensure derived classes will be properly destructed.
     */
    ~ArduinoConnector();

    /**
     * @brief Refresh frame colors
     */
    void update() const;

    /**
     * @brief Push latest colors to leds
     */
    void draw();

    /**
     * @brief Blocking function to connect to the arduino.
     *
     * An ArduinoConnectorException will be thrown on failure.
     * @param port the tty device to use
     */
    void connect(const std::string& port);

    /**
     * @brief Blocking function to connect to the arduino.
     *
     * A tty device must have been set. An ArduinoConnectorException will be thrown on failure.
     */
    void connect();

    /**
     * @brief calculate and return the current fps, measured by completed draw calls
     * @return current fps
     */
    inline float getCurrentFps() const {
        return mCurrentFps;
    }

    /**
     * @brief construct an ArduinoConnector, responsible for obtaining and updating the led data
     * @param port tty port for comms
     */
    explicit ArduinoConnector(const std::string& port);

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
    void addFilter(std::string id, std::unique_ptr<DataFilter> filter) const;

    /**
     * @brief Remove a filter from the list
     * @param id filter name
     * @return the filter
     */
    std::unique_ptr<DataFilter> removeFilter(std::string id) const;

    /**
     * @brief getFilter get a filter for editing
     * @param id filter name
     * @return reference to the filter
     */
    std::unique_ptr<DataFilter>& getFilter(std::string id) const;

    /**
     * @brief Retrieve the bufferspace required for a rgb dataset
     * @return number of bytes required
     */
    size_t getRequiredBufferLength() const {
        assert(mColorDataProvider);
        return mColorDataProvider->getRequiredBufferLength();
    }

    /**
     * @brief Set a target as to what fps should be achieved
     * @param fps target fps in seconds. if lower than 0, the target is reset.
     */
    void setTargetFps(float fps) {
        mTargetFps = fps;
    }

    /**
     * @brief Get current target fps in seconds
     */
    float getTargetFps() const {
        return mTargetFps;
    }

    /**
     * @brief Set the port that will be used in the next connection attempt
     * @param port device name, like /dev/tty0
     */
    void setPort(const std::string port) {
        mPort = port;
    }

    /**
     * @brief Set the ColorDataProvider used for getting the led color data
     */
    void setColorDataProvider(std::shared_ptr<ColorDataProvider> colorDataProvider);

    /**
     * @brief Get the ColorDataProvider used for getting the led color data
     */
    std::shared_ptr<ColorDataProvider> getColorDataProvider() const {
        return mColorDataProvider;
    }

    /**
     * @brief Retrieve the current serial class
     */
    const Serial* getSerial() const {
        return mSerial;
    }

private:

    /**
     * @brief A debug function; can be used to print out the led buffer to a text file
     * @param path where to write to
     */
    void writeRgbBufferToText(std::string path) const;

    /**
     * @brief update #mCurrentFps and #mLastDraw
     */
    void updateFps();

    std::shared_ptr<ColorDataProvider> mColorDataProvider;///< the object that provides the RGB led data as a uint8_t array

    uint8_t* mRgbBuffer = nullptr;///< our very own bufferspace for led data

    char mCommBuffer[128];///< buffer for arduino responses

    int mSerialFd = -1;///< serial connection file descriptor

    clock_t mLastDraw;///< a timestamp of when the last draw was executed

    float mCurrentFps = 0;///< current fps, saved so the timekeeping code can always be called at the same moment

    float mTargetFps = -1;///< target fps; important when using spectrometer

    std::string mPort;///< arduino connection name

    Serial* mSerial;///< the serial interface instance used to communicate with the arduino
};

#endif // LEDCONNECTOR_H
