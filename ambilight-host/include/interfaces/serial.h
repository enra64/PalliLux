#ifndef SERIAL_H
#define SERIAL_H

#include <cstring>
#include <cstdint>
#include <string>

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
    virtual void waitForData() = 0;
    virtual void send(uint8_t* buf, size_t len) = 0;
    virtual void receive(uint8_t* buf, size_t len) = 0;
    virtual void open(const std::string& port) = 0;
    virtual void close() = 0;
    virtual bool deviceExists(const std::string& port) = 0;
};

#endif // SERIAL_H
