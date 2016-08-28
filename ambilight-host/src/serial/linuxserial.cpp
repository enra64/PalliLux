#include "/home/arne/Documents/Development/ShittyAmbilight/ambilight-host/include/serial/linuxserial.h"

// assert
// who would have guessed
#include "assert.h"

// serial
#include <termios.h>
#include <unistd.h>
#include <sys/poll.h>

// file access
#include <fcntl.h>
#include <unistd.h>// access()

#include <string>

void LinuxSerial::waitForData() const {
    // create poll struct, watching our serial file descriptor for input events
    struct pollfd pollStruct[1];
    pollStruct[0].fd = mFd;
    pollStruct[0].events = POLLIN ;

    // poll, checking for failure
    if (poll(pollStruct, 1, 1000) < 0)
        throw SerialException(std::string("polling timeout: ") + strerror(errno));
}

void LinuxSerial::send(const uint8_t *buf, size_t len) const {
    // check whether our fd is ok
    if(mFd < 0)
        throw SerialException("Connection closed");

    // write data buffer
    write(mFd, buf, len);
}

bool LinuxSerial::good(const std::__cxx11::string &ttyDevice) const {
    return mFd >= 0 && deviceExists(ttyDevice);
}

size_t LinuxSerial::receive(uint8_t *buf, size_t len) const {
    // read from fd
    return read(mFd, buf, len);
}

void LinuxSerial::open(const std::string &port) {
    // close previous serial connection (anonymous namespace)
    ::close(mFd);

    // check whether the tty device exists
    if(!deviceExists(port))
        throw SerialException(port + std::string(" does not exist"));

    // open serial connection
    mFd = ::open(port.c_str(), O_RDWR | O_NOCTTY);

    // exception on error
    if(mFd == -1)
        throw SerialException(std::string("could not open ") + port);

    // get current control struct
    struct termios options;
    tcgetattr(mFd, &options);

    // set our options:
    // https://chrisheydrick.com/2012/06/17/how-to-read-serial-data-from-an-arduino-in-linux-with-c-part-3/

    // 115200 baud in and out
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

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

    // disable: canonical input, echo, visually erase chars, terminal-generated signals
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // disable output processing
    options.c_oflag &= ~OPOST;

    // commit the options
    tcsetattr(mFd, TCSANOW, &options);

    // wait for arduino reset
    usleep(1000*2000);

    // flush serial buffer
    tcflush(mFd, TCIFLUSH);
}

void LinuxSerial::close() {
    ::close(mFd);
    mFd = -1;
}

bool LinuxSerial::deviceExists(const std::string& port) const {
    return access(port.c_str(), F_OK) >= 0;
}
