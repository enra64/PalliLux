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
#include <fstream>

// cross platform sleep
#include <chrono>
#include <thread>

// choose appropriate serial header
#ifdef __linux__
#include "linuxserial.h"
#elif _WIN32
#include "windowsserial.h"
#else
#error Platform not recognized
#endif

#include "customexceptions.h"


using namespace std;

ArduinoConnector::ArduinoConnector() {
    // initialise timekeeping
    mLastDraw = clock();

    // get a serial connection
#ifdef __linux__
    mSerial = new LinuxSerial();
#elif _WIN32
    mSerial = new WindowsSerial();
#else
#error Platform not recognized
#endif
}

ArduinoConnector::ArduinoConnector(const string& port) : ArduinoConnector() {
    mPort = port;
}


void ArduinoConnector::writeRgbBufferToText(string path) const {
    // open the file
    ofstream file;
    file.open(path.c_str(), std::fstream::trunc);

    // check the file handle
    if (!file.good())
        throw invalid_argument("bad file write for " + path);

    // print a rgb triple per line
    for (size_t i = 0; i < mColorDataProvider->getRequiredBufferLength(); i += 3) {
        // data string
        string line = "R" + to_string(mRgbBuffer[i]) + "G" + to_string(mRgbBuffer[i + 1]) + "B" + to_string(mRgbBuffer[i + 2]) + "\n";
        // write to file
        file << line;
    }

    // close file
    file.close();
}

void ArduinoConnector::updateFps() {
    // the fps value is calculated as 1 / the timestamp diff between draws
    mCurrentFps = 1.f / (static_cast<float>(clock() - mLastDraw) / CLOCKS_PER_SEC);

    // refresh "last draw" timestamp
    mLastDraw = clock();
}

void ArduinoConnector::update() const {
    // refresh screen data, update duration
    mColorDataProvider->getData(mRgbBuffer);
}

void ArduinoConnector::draw() {
    // notify if the serial connection has been closed or not yet opened
    if (!mSerial->good())
        throw ArduinoConnectorCommunicationException("Connection is bad!");

    // write data buffer
    mSerial->send(mRgbBuffer, mColorDataProvider->getRequiredBufferLength());

    // wait for arduino acknowledgement
    mSerial->waitForData();

    // read input
    mSerial->receive(mCommBuffer, 128);

    // check the acknowledgement char
    if (mCommBuffer[0] != 'k')
        throw ArduinoConnectorProtocolException("incorrect acknowledgement character received");

    // reset ACK char
    mCommBuffer[0] = 0;

    // keep check of how long draw-to-draw takes
    updateFps();

    // wait if custom target fps were set
    if (mTargetFps > 0) {
        // only wait if our framerate is too high
        if (mCurrentFps > mTargetFps) {
            // calculate millisecond waiting time
            uint64_t waitTime = static_cast<uint64_t>(1000.f / (mCurrentFps - mTargetFps));
            // wait
            this_thread::sleep_for(chrono::milliseconds(waitTime));
        }
    }
}

void ArduinoConnector::connect(const string& port) {
    // save port name and connect
    mPort = port;
    connect();
}

void ArduinoConnector::connect() {
    // check that a tty device has been set
    assert(mPort.length() > 0);

    mSerial->open(mPort);

    // check connection
    mSerial->send("hello", 6);

    // read arduino response until we have accumulated 3 magic characters ("SAM")
    size_t receiveCount = 0;
    while(receiveCount < 3){
        mSerial->waitForData();
        receiveCount += mSerial->receive(mCommBuffer, 128);
    }

    // null-terminate string
    mCommBuffer[receiveCount] = 0;

    // check whether the arduino responded correctly ("SAM")
    if (string(mCommBuffer, receiveCount) == "SAM")
        return;

    // failure; the arduino response is not what we expected ("SAM")
    throw ArduinoConnectorProtocolException("faulty opening sequence");
}

ArduinoConnector::~ArduinoConnector() {
    // gracefully shut down serial connection
    mSerial->close();

    // free space
    delete[] mRgbBuffer;
    delete mSerial;
}

void ArduinoConnector::setColorDataProvider(std::shared_ptr<ColorDataProvider> colorDataProvider) {
    // save the provider
    mColorDataProvider = colorDataProvider;

    // delete any old rgb buffer space
    delete[] mRgbBuffer;

    // allocate new buffer space
    mRgbBuffer = new uint8_t[mColorDataProvider->getRequiredBufferLength()];
}

void ArduinoConnector::disconnect(bool blackoutLeds) {
    // if blackoutLeds is true, we want to set all leds black as our last action
    if (blackoutLeds) {
        memset(mRgbBuffer, 0, mColorDataProvider->getRequiredBufferLength());
        draw();
    }

    // after that, close the serial connection
    mSerial->close();
}

void ArduinoConnector::addFilter(string id, unique_ptr<DataFilter> filter) const {
    mColorDataProvider->addFilter(id, move(filter));
}

std::unique_ptr<DataFilter> ArduinoConnector::removeFilter(string id) const {
    return move(mColorDataProvider->removeFilter(id));
}

std::unique_ptr<DataFilter>& ArduinoConnector::getFilter(std::string id) const {
    return mColorDataProvider->getFilter(id);
}
