#include "xlibborderprovider.h"
#include "ledconnector.h"

#include <memory>
#include <sys/ioctl.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

using namespace std;

LedConnector::LedConnector(std::shared_ptr<BorderProvider> borderProvider, unsigned int horizontalLedCount, unsigned int verticalLedCount) {
    mRgbConverter = make_unique<RgbConverter>(borderProvider, horizontalLedCount, verticalLedCount);
    mRgbBuffer = new uint8_t[mRgbConverter->getRequiredBufferLength()];
}

void LedConnector::update() {
    mUpdateCount++;
    mUpdateDuration += mRgbConverter->takeAndParseScreenShot(mRgbBuffer);
}

void LedConnector::draw() {
    write(mSerialFd, mRgbBuffer, mRgbConverter->getRequiredBufferLength());
}

bool LedConnector::connect(const string& port) {
    // close previous serial connection
    close(mSerialFd);

    // open serial connection
    mSerialFd = open(port.c_str(), O_RDWR | O_NOCTTY);

    // -1 is returned on error
    if(mSerialFd == -1)
        return false;

    // get current control struct
    struct termios options;
    tcgetattr(mSerialFd, &options);

    // set our options:
    // https://chrisheydrick.com/2012/06/17/how-to-read-serial-data-from-an-arduino-in-linux-with-c-part-3/

    // 9600 baud in and out
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // 8 bits, no parity, no stop bits
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // no hardware flow control
    options.c_cflag &= ~CRTSCTS;

    // enable receiver, ignore status lines
    options.c_cflag |= CREAD | CLOCAL;

    // disable input/output flow control, disable restart chars
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* disable canonical input, disable echo,
    disable visually erase chars,
    disable terminal-generated signals */
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // disable output processing
    options.c_oflag &= ~OPOST;

    // commit the options
    tcsetattr(mSerialFd, TCSANOW, &options);

    // wait for arduino reset
    usleep(1000*1000);

    // flush serial buffer
    tcflush(mSerialFd, TCIFLUSH);

    // check connection
    write(mSerialFd, "hello", 5);

    // read arduino response
    size_t rec = read(mSerialFd, &mCommBuffer, 128);

    // null-terminate string
    mCommBuffer[rec] = 0;

    // check whether the arduino responded correctly
    return string(mCommBuffer, rec) == "SAM";
}

LedConnector::~LedConnector() {
    delete[] mRgbBuffer;
    close(mSerialFd);
}
