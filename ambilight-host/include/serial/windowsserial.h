#ifndef WINDOWS_SERIAL_H
#define WINDOWS_SERIAL_H

#include "serial.h"

#include <cstring>
#include <cstdint>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief An interface for serial communication with arduino on windows. Thanks to http://playground.arduino.cc/Interfacing/CPPWindows!
 */
class WindowsSerial : public Serial{
public:
    void waitForData() const override;
    void send(uint8_t* buf, size_t len) const override;
    size_t receive(uint8_t *buf, size_t len) const override;
    void open(const std::string& port) override;
    void close() override;
    static bool deviceExists(const std::string& port) const override;
    bool good(const std::string &ttyDevice) const override;
private:
    HANDLE mSerialHandle;
    COMSTAT status;
    DWORD errors;
};

#endif // WINDOWS_SERIAL_H
