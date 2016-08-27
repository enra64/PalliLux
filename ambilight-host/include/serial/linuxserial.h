#ifndef LINUX_SERIAL_H
#define LINUX_SERIAL_H

#include "serial.h"

#include <cstring>
#include <cstdint>
#include <string>

/**
 * @brief An interface for serial communication with arduino
 */
class LinuxSerial : public Serial{
public:
    void waitForData() override;
    void send(uint8_t *buf, size_t len) override;
    void receive(uint8_t *buf, size_t len) override;
    void open(const std::string &port) override;
    void close() override;
    bool deviceExists(const std::string& port) override;
private:
    int mFd;///!< serial file descriptor
};



#endif // LINUX_SERIAL_H
