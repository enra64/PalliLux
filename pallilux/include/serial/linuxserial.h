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
    bool waitForData(long timeout=0) const override;
    void send(const uint8_t *buf, size_t len) const override;
    size_t receive(uint8_t *buf, size_t len) const override;
    void open(const std::string &port) override;
    void close() override;
    bool deviceExists(const std::string& port) const override;
    bool good() const override;
private:
    int mFd;///< serial file descriptor
};



#endif // LINUX_SERIAL_H
