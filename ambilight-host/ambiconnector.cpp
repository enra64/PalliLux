#include "triplescreenborderprovider.h"
#include "ambiconnector.h"

// cout
#include <iostream>

// strerror
#include <string.h>

// smart pointer
#include <memory>

// benchmarking
#include <ctime>

// file access
#include <fcntl.h>

// serial
#include <termios.h>
#include <unistd.h>
#include <sys/poll.h>

using namespace std;

AmbiConnector::AmbiConnector(std::shared_ptr<BorderProvider> borderProvider, unsigned int horizontalLedCount, unsigned int verticalLedCount) {
    mRgbConverter = make_unique<RgbConverter>(borderProvider, horizontalLedCount, verticalLedCount);
    mRgbBuffer = new uint8_t[mRgbConverter->getRequiredBufferLength()];

    // initialise timekeeping
    mLastDraw = clock();
}

void AmbiConnector::writeRgbBufferToText(string path) {
    // open the file
    FILE* debugFile = fopen(path.c_str(), "w+");

    // check the file handle
    if(debugFile == NULL)
        throw invalid_argument("No file could be opened at " + path);

    // print a rgb triple per line
    for(size_t i = 0; i < mRgbConverter->getRequiredBufferLength(); i+=3) {
        // data string
        string line = "R" + to_string(mRgbBuffer[i]) + "G" + to_string(mRgbBuffer[i+1]) + "B" + to_string(mRgbBuffer[i+2]) + "\n";
        // write to file
        fputs(line.c_str(), debugFile);
    }

    // close file
    fclose(debugFile);
}

void AmbiConnector::waitForSerialInput() {
    // create poll struct, watching our serial file descriptor for input events
    struct pollfd pollStruct[1];
    pollStruct[0].fd = mSerialFd;
    pollStruct[0].events = POLLIN ;

    // poll, checking for failure
    if (poll(pollStruct, 1, 1000) < 0)
        throw new AmbiConnectorCommunicationException(strerror(errno));
}

void AmbiConnector::updateFps() {
    mCurrentFps = 1.f / (static_cast<float>(clock() - mLastDraw) / CLOCKS_PER_SEC);
    mLastDraw = clock();
}

void AmbiConnector::update() {
    // refresh screen data, update duration
    mRgbConverter->takeAndParseScreenShot(mRgbBuffer);
}

void AmbiConnector::draw() {
    // notify if the serial connection has been closed or not yet opened
    if(mSerialFd == -1)
        throw new AmbiConnectorCommunicationException("No connection active!");

    // write data buffer
    write(mSerialFd, mRgbBuffer, mRgbConverter->getRequiredBufferLength());

    // wait for arduino acknowledgement
    waitForSerialInput();

    // read input
    read(mSerialFd, mCommBuffer, 128);

    // check the acknowledgement char
    if(mCommBuffer[0] != 'k')
        throw new AmbiConnectorProtocolException("incorrect acknowledgement character received");

    // keep check of how long draw-to-draw takes
    updateFps();
}

bool AmbiConnector::connect(const string& port) {
    // close previous serial connection
    close(mSerialFd);

    // open serial connection
    mSerialFd = open(port.c_str(), O_RDWR | O_NOCTTY);

    // -1 is returned on error
    if(mSerialFd == -1)
        throw new AmbiConnectorCommunicationException("could not open " + port);

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

    // disable: canonical input, echo, visually erase chars, terminal-generated signals
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

    while(rec < 3) {
        waitForSerialInput();
        rec += read(mSerialFd, &mCommBuffer, 128);
    }

    // null-terminate string
    mCommBuffer[rec] = 0;

    // check whether the arduino responded correctly
    if(string(mCommBuffer, rec) == "SAM") {
        cout << "opening sequence ok" << endl;
        return true;
    }
    throw new AmbiConnectorProtocolException("faulty opening sequence");
    return false;
}

AmbiConnector::~AmbiConnector() {
    delete[] mRgbBuffer;
    close(mSerialFd);
}

void AmbiConnector::disconnect(bool blackoutLeds)
{
    if(blackoutLeds){
        memset(mRgbBuffer, 0, mRgbConverter->getRequiredBufferLength());
        draw();
    }
    close(mSerialFd);
    mSerialFd = -1;
}

void AmbiConnector::addFilter(string id, std::unique_ptr<DataFilter> filter)
{
    mRgbConverter->addFilter(id, move(filter));
}

std::unique_ptr<DataFilter> AmbiConnector::removeFilter(string id)
{
    return move(mRgbConverter->removeFilter(id));
}
