#ifndef SERIAL_H
#define SERIAL_H

#include <cstdint>

#include <stdexcept>

class SerialException : public std::runtime_error {
public:
    /**
     * @brief std::string constructor
     * @param message the error message
     */
    SerialException(const std::string& message) : runtime_error(message) {
    }
};

/**
 * @brief An interface for serial communication with arduino
 */
class Serial {
public:
    /**
     * @brief Empty virtual destructor to ensure derived classes will be properly destructed.
     */
    virtual ~Serial() {}

    /**
     * @brief Block until data arrives
     * @param timeout timeout in seconds until the function returns even without data. 0 disables timeout
     * @return true if data has arrived, false if timeout has occurred
     */
    virtual bool waitForData(long timeout=0) const = 0;

    /**
     * @brief Transmit the data to the serial device
     * @param buf data to send
     * @param len length of the data
     */
    virtual void send(const uint8_t* buf, size_t len) const = 0;

    /**
     * @brief Receive incoming serial data
     * @param buf buffer for incoming data
     * @param len maximum read length
     * @return amount of bytes read
     */
    virtual size_t receive(uint8_t* buf, size_t len) const = 0;

    /**
     * @brief Create a serial connection
     * @param port the port to use (like "/dev/ttyUSB0")
     */
    virtual void open(const std::string& port) = 0;

    /**
     * @brief Close the serial connection.
     */
    virtual void close() = 0;

    /**
     * @brief Check whether a serial device exists at the given port
     * @param port port to check, like "/dev/ttyUSB0" or "COM10"
     * @return true if the device exists
     */
    virtual bool deviceExists(const std::string& port) const = 0;

    /**
     * @brief Check whether this connection is alive
     * @return true if the connection seems to be alive
     */
    virtual bool good() const = 0;

public:// send and receive overloads because c++ likes to whine
    /**
     * @brief Receive incoming serial data
     * @param buf buffer for incoming data
     * @param len maximum read length
     * @return amount of bytes read
     */
    virtual size_t receive(char* buf, size_t len) const {
        return this->receive(reinterpret_cast<uint8_t*>(buf), len);
    }

    /**
     * @brief Transmit the data to the serial device
     * @param buf data to send
     * @param len length of the data
     */
    virtual void send(const char* buf, size_t len) const {
        this->send(reinterpret_cast<const uint8_t*>(buf), len);
    }
};

#endif // SERIAL_H
