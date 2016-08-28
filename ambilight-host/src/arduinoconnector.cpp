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
#include <fstream>

// serial
#ifdef __linux__
    #include "linuxserial.h"
#elif _WIN32
    #include "windowsserial.h"
#else
    #error Platform not recognized
#endif



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

    // get a serial connection
    #ifdef __linux__
        mSerial = new LinuxSerial();
    #elif _WIN32
        mSerial = new WindowsSerial();
    #else
        #error Platform not recognized
    #endif
}

void ArduinoConnector::writeRgbBufferToText(string path) {
    // open the file
    ofstream file;
    file.open(path.c_str(), std::fstream::trunc);

    // check the file handle
    if(!file.good())
        throw invalid_argument("bad file write for " + path);

    // print a rgb triple per line
    for(size_t i = 0; i < mRgbLineProvider->getRequiredBufferLength(); i+=3) {
        // data string
        string line = "R" + to_string(mRgbBuffer[i]) + "G" + to_string(mRgbBuffer[i+1]) + "B" + to_string(mRgbBuffer[i+2]) + "\n";
        // write to file
        file << line;
    }

    // close file
    file.close();
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
    if(!mSerial->good(mTtyDevice))
        throw AmbiConnectorCommunicationException("Connection is bad!");

    // write data buffer
    mSerial->send(mRgbBuffer, mRgbLineProvider->getRequiredBufferLength());

    // wait for arduino acknowledgement
    mSerial->waitForData();

    // read input
    mSerial->receive(mCommBuffer, 128);

    // check the acknowledgement char
    if(mCommBuffer[0] != 'k')
        throw AmbiConnectorProtocolException("incorrect acknowledgement character received");

    // reset ACK char
    mCommBuffer[0] = 0;

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

    mSerial->open(mTtyDevice);

    // check connection
    mSerial->send("hello", 5);

    // read arduino response
    size_t rec = 0;

    while(rec < 3) {
        mSerial->waitForData();
        rec += mSerial->receive(mCommBuffer, 128);
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
    mSerial->close();
    delete mSerial;
}

void ArduinoConnector::disconnect(bool blackoutLeds)
{
    if(blackoutLeds){
        memset(mRgbBuffer, 0, mRgbLineProvider->getRequiredBufferLength());
        draw();
    }
    mSerial->close();
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
