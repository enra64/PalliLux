#include "arduinoconnector.h"

// cout
#include <iostream>

// assertions
#include <assert.h>

// strerror
#include <string.h>

// smart pointer
#include <memory>

// benchmarking
#include <ctime>

// file access
#include <fcntl.h>
#include <unistd.h>// access()

// serial
#include <termios.h>
#include <unistd.h>
#include <sys/poll.h>

using namespace std;

ArduinoConnector::ArduinoConnector(std::shared_ptr<RgbLineProvider> rgbProvider) : ArduinoConnector(rgbProvider, ""){

}

ArduinoConnector::ArduinoConnector(std::shared_ptr<RgbLineProvider> rgbProvider, const std::string port) : mTtyDevice(port)
{
    // save rgb line provider
    mRgbLineProvider = rgbProvider;

    // reserve buffer space
    mRgbBuffer = new uint8_t[mRgbLineProvider->getRequiredBufferLength()];

    // initialise timekeeping
    mLastDraw = clock();
}

void ArduinoConnector::writeRgbBufferToText(string path) {
    // open the file
    FILE* debugFile = fopen(path.c_str(), "w+");

    // check the file handle
    if(debugFile == NULL)
        throw invalid_argument("No file could be opened at " + path);

    // print a rgb triple per line
    for(size_t i = 0; i < mRgbLineProvider->getRequiredBufferLength(); i+=3) {
        // data string
        string line = "R" + to_string(mRgbBuffer[i]) + "G" + to_string(mRgbBuffer[i+1]) + "B" + to_string(mRgbBuffer[i+2]) + "\n";
        // write to file
        fputs(line.c_str(), debugFile);
    }

    // close file
    fclose(debugFile);
}

void ArduinoConnector::waitForSerialInput() {
    // create poll struct, watching our serial file descriptor for input events
    struct pollfd pollStruct[1];
    pollStruct[0].fd = mSerialFd;
    pollStruct[0].events = POLLIN ;

    // poll, checking for failure
    if (poll(pollStruct, 1, 1000) < 0)
        throw AmbiConnectorCommunicationException(strerror(errno));
}

void ArduinoConnector::updateFps() {
    mCurrentFps = 1.f / (static_cast<float>(clock() - mLastDraw) / CLOCKS_PER_SEC);
    mLastDraw = clock();
}

void ArduinoConnector::update() {
    // refresh screen data, update duration
    mRgbLineProvider->getData(mRgbBuffer);
}

void ArduinoConnector::draw() {
    // notify if the serial connection has been closed or not yet opened
    if(mSerialFd == -1)
        throw AmbiConnectorCommunicationException("No connection active!");

    // write data buffer
    write(mSerialFd, mRgbBuffer, mRgbLineProvider->getRequiredBufferLength());

    // wait for arduino acknowledgement
    waitForSerialInput();

    // read input
    read(mSerialFd, mCommBuffer, 128);

    // check the acknowledgement char
    if(mCommBuffer[0] != 'k')
        throw AmbiConnectorProtocolException("incorrect acknowledgement character received");

    // keep check of how long draw-to-draw takes
    updateFps();

    // wait if custom target fps were set
    if(mTargetFps > 0){
        // only wait if our framerate is too high
        if(mCurrentFps > mTargetFps){
            // calculate microsecond waiting time
            float waitTime = 1000000 / (mCurrentFps - mTargetFps);
            // wait
            usleep(waitTime);
        }
    }
}

void ArduinoConnector::connect(const string& port) {
    mTtyDevice = port;
    connect();
}

void ArduinoConnector::connect() {
    // check that a tty device has been set
    assert(mTtyDevice.length() > 0);

    // close previous serial connection
    close(mSerialFd);

    // check whether the tty device exists
    if(access(mTtyDevice.c_str(), F_OK) < 0)
        throw AmbiConnectorCommunicationException(mTtyDevice + " does not exist");

    // open serial connection
    mSerialFd = open(mTtyDevice.c_str(), O_RDWR | O_NOCTTY);

    // exception on error
    if(mSerialFd == -1)
        throw AmbiConnectorCommunicationException("could not open " + mTtyDevice);

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
        // success: no exception!
        return;
    }
    throw AmbiConnectorProtocolException("faulty opening sequence");
}

ArduinoConnector::~ArduinoConnector() {
    delete[] mRgbBuffer;
    close(mSerialFd);
}

void ArduinoConnector::disconnect(bool blackoutLeds)
{
    if(blackoutLeds){
        memset(mRgbBuffer, 0, mRgbLineProvider->getRequiredBufferLength());
        draw();
    }
    close(mSerialFd);
    mSerialFd = -1;
}

void ArduinoConnector::addFilter(string id, std::unique_ptr<DataFilter> filter)
{
    mRgbLineProvider->addFilter(id, move(filter));
}

std::unique_ptr<DataFilter> ArduinoConnector::removeFilter(string id)
{
    return move(mRgbLineProvider->removeFilter(id));
}

std::unique_ptr<DataFilter> &ArduinoConnector::getFilter(std::__cxx11::string id)
{
    return mRgbLineProvider->getFilter(id);
}
