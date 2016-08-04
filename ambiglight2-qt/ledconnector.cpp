#include "xlibborderprovider.h"
#include "ledconnector.h"

#include "assert.h"
#include <memory>
#include <sys/ioctl.h>
#include <cstdio>
#include <iostream>
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

    // write data buffer
    // 160 led/default
    write(mSerialFd, mRgbBuffer, mRgbConverter->getRequiredBufferLength());

    // 18 leds
    //write(mSerialFd, mRgbBuffer, (18*3));

    // wait for arduino acknowledgement
    size_t receiveCount = 0;

    while(receiveCount <= 0)
        receiveCount += read(mSerialFd, mCommBuffer, 128);

    assert(mCommBuffer[0] == 'k');

    /*
    FILE* f = fopen("test/rgbBuffer.txt", "w+");

    for(int i = 0; i < 18*3; i+=3){
        string line = "R" + to_string(mRgbBuffer[i]) + "G" + to_string(mRgbBuffer[i+1]) + "B" + to_string(mRgbBuffer[i+2]) + "\n";
        fputs(line.c_str(), f);
    }

    fclose(f);*/

    cout << "ACK" << ++mDrawCount << endl;
}

bool LedConnector::connect(const string& port) {
    // close previous serial connection
    close(mSerialFd);

    // open serial connection
    mSerialFd = open(port.c_str(), O_RDWR | O_NOCTTY);

    // -1 is returned on error
    if(mSerialFd == -1){
        cout << "could not open tty - fd is -1" << endl;
        return false;
    }

    // get current control struct
    struct termios options;
    tcgetattr(mSerialFd, &options);

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

    /* disable canonical input, disable echo,
    disable visually erase chars,
    disable terminal-generated signals */
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // disable output processing
    options.c_oflag &= ~OPOST;

    // commit the options
    tcsetattr(mSerialFd, TCSANOW, &options);

    // wait for arduino reset
    usleep(1000*2000);

    // flush serial buffer
    tcflush(mSerialFd, TCIFLUSH);

    // check connection
    write(mSerialFd, "hello", 5);

    // read arduino response
    size_t rec = 0;

    while(rec < 3)
        rec += read(mSerialFd, &mCommBuffer, 128);

    // null-terminate string
    mCommBuffer[rec] = 0;

    // check whether the arduino responded correctly
    if(string(mCommBuffer, rec) == "SAM"){
        cout << "opening sequence ok" << endl;
        return true;
    }
    cout << "opening sequence faulty" << endl;
    return false;
}

LedConnector::~LedConnector() {
    delete[] mRgbBuffer;
    close(mSerialFd);
}
