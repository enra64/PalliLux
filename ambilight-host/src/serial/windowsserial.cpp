#include "/home/arne/Documents/Development/ShittyAmbilight/ambilight-host/include/serial/windowsserial.h"


void WindowsSerial::waitForData() {
    DWORD dwEventMask;
    // wait for any character
    if(!WaitCommEvent(hComm, &dwEventMask, NULL))
        throw SerialException("failure while waiting for serial event");
}

void WindowsSerial::send(uint8_t *buf, size_t len) {
    DWORD bytesSent;

    // Try to write the buffer on the Serial port
    if(!WriteFile(mSerialHandle, buf, len, &bytesSent, 0)) {
        // In case it don't work get comm error
        //ClearCommError(hSerial, &errors, &status);
        throw new SerialException("could not write");
    }
}

void WindowsSerial::receive(uint8_t *buf, size_t len) {
    // Number of bytes we'll have read
    DWORD bytesRead;

    // Use the ClearCommError function to get status info on the Serial port
    ClearCommError(mSerialHandle, &errors, &status);

    // Check if there is something to read
    if(status.cbInQue <= 0)
        return;

    // read only as much as available, and dont read more than our buffer can handle
    unsigned int bytesToRead = status.cbInQue < len ? status.cbInQue : len;

    // Try to read the require number of chars, and return the number of read bytes on success
    if(ReadFile(hSerial, buffer, bytesToRead, &bytesRead, NULL) == 0)
        throw SerialException("no data read");
}

void WindowsSerial::open(const std::string &port) {
    // Try to connect to the given port throuh CreateFile
    mSerialHandle = CreateFile(portName,
                               GENERIC_READ | GENERIC_WRITE,
                               0,
                               NULL,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);

    // Check if the connection was successfull
    if(mSerialHandle==INVALID_HANDLE_VALUE) {
        if(GetLastError()==ERROR_FILE_NOT_FOUND)
            throw SerialException(std::string(port + std::string(" is not available")));
        else
            throw SerialException("unknown error occured during serial open");
    }

    // If connected we try to set the comm parameters
    DCB dcbSerialParams = {0};

    // Try to get the current
    if (!GetCommState(mSerialHandle, &dcbSerialParams))
        throw SerialException("could not get current connection parameters");

    // Define serial connection parameters for the arduino board
    dcbSerialParams.BaudRate=CBR_115200;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;

    // Setting the DTR to Control_Enable ensures that the Arduino is properly
    // reset upon establishing a connection
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

    // Set the parameters and check for their proper application
    if(!SetCommState(mSerialHandle, &dcbSerialParams))
        throw SerialException("could not set new connection parameters");

    // Flush any remaining characters in the buffers
    PurgeComm(mSerialHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
    // We wait 2s as the arduino board will be reseting
    Sleep(ARDUINO_WAIT_TIME);
}

void WindowsSerial::close() {
    // Close the serial handler
    CloseHandle(mSerialHandle);
}

bool WindowsSerial::deviceExists(const std::string& port) {
    // Try to connect to the given port throuh CreateFile
    HANDLE serialHandle = CreateFile(portName,
                               GENERIC_READ | GENERIC_WRITE,
                               0,
                               NULL,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);

    // close the handle again
    CloseHandle(serialHandle);

    // check for success
    return serialHandle != INVALID_HANDLE_VALUE;
}
