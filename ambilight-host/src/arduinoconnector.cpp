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
	mTtyDevice = port;
}


void ArduinoConnector::writeRgbBufferToText(string path) const
{
	// open the file
	ofstream file;
	file.open(path.c_str(), std::fstream::trunc);

	// check the file handle
	if (!file.good())
		throw invalid_argument("bad file write for " + path);

	// print a rgb triple per line
	for (size_t i = 0; i < mRgbLineProvider->getRequiredBufferLength(); i += 3)
	{
		// data string
		string line = "R" + to_string(mRgbBuffer[i]) + "G" + to_string(mRgbBuffer[i + 1]) + "B" + to_string(mRgbBuffer[i + 2]) + "\n";
		// write to file
		file << line;
	}

	// close file
	file.close();
}

void ArduinoConnector::updateFps()
{
	mCurrentFps = 1.f / (static_cast<float>(clock() - mLastDraw) / CLOCKS_PER_SEC);
	mLastDraw = clock();
}

void ArduinoConnector::update() const
{
	// refresh screen data, update duration
	mRgbLineProvider->getData(mRgbBuffer);
}

void ArduinoConnector::draw()
{
	// notify if the serial connection has been closed or not yet opened
	if (!mSerial->good(mTtyDevice))
		throw AmbiConnectorCommunicationException("Connection is bad!");

	// write data buffer
	mSerial->send(mRgbBuffer, mRgbLineProvider->getRequiredBufferLength());

	// wait for arduino acknowledgement
	mSerial->waitForData();

	// read input
	mSerial->receive(mCommBuffer, 128);

	// check the acknowledgement char
	if (mCommBuffer[0] != 'k')
		throw AmbiConnectorProtocolException("incorrect acknowledgement character received");

	// reset ACK char
	mCommBuffer[0] = 0;

	// keep check of how long draw-to-draw takes
	updateFps();

	// wait if custom target fps were set
	if (mTargetFps > 0)
	{
		// only wait if our framerate is too high
		if (mCurrentFps > mTargetFps)
		{
			// calculate millisecond waiting time
			uint64_t waitTime = static_cast<uint64_t>(1000.f / (mCurrentFps - mTargetFps));
			// wait
			this_thread::sleep_for(chrono::milliseconds(waitTime));
		}
	}
}

void ArduinoConnector::connect(const string& port)
{
	mTtyDevice = port;
	connect();
}

void ArduinoConnector::connect()
{
	// check that a tty device has been set
	assert(mTtyDevice.length() > 0);

	mSerial->open(mTtyDevice);

	// check connection
	mSerial->send("hello", 5);

	// read arduino response
	size_t rec = 0;

	while (rec < 3)
	{
		mSerial->waitForData();
		rec += mSerial->receive(mCommBuffer, 128);
	}

	// null-terminate string
	mCommBuffer[rec] = 0;

	// check whether the arduino responded correctly
	if (string(mCommBuffer, rec) == "SAM")
	{
		cout << "opening sequence ok" << endl;
		// success: no exception!
		return;
	}
	throw AmbiConnectorProtocolException("faulty opening sequence");
}

ArduinoConnector::~ArduinoConnector()
{
	delete[] mRgbBuffer;
	mSerial->close();
	delete mSerial;
}

void ArduinoConnector::setRgbLineProvider(std::shared_ptr<ColorDataProvider> lineProvider) {
	mRgbLineProvider = lineProvider;

	delete[] mRgbBuffer;
	mRgbBuffer = new uint8_t[mRgbLineProvider->getRequiredBufferLength()];
}

void ArduinoConnector::disconnect(bool blackoutLeds)
{
	if (blackoutLeds)
	{
		memset(mRgbBuffer, 0, mRgbLineProvider->getRequiredBufferLength());
		draw();
	}
	mSerial->close();
}

void ArduinoConnector::addFilter(string id, unique_ptr<DataFilter> filter) const
{
	mRgbLineProvider->addFilter(id, move(filter));
}

std::unique_ptr<DataFilter> ArduinoConnector::removeFilter(string id) const
{
	return move(mRgbLineProvider->removeFilter(id));
}

std::unique_ptr<DataFilter>& ArduinoConnector::getFilter(std::string id) const
{
	return mRgbLineProvider->getFilter(id);
}
