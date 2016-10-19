#include <iostream>
#include <string>

#include <assert.h>

#include "arduinoconnector.h"
#include "ambicolordataprovider.h"
#include "customexceptions.h"
#include "singlescreenborderprovider.h"
#include "ambiconnectorbuilder.h"

using namespace std;

// linux
//#include "xlibscreenshotprovider.h"
//string mDefaultTtyDevice("/dev/ttyUSB0");

// windows: GDI
#include "ddapiscreenshotprovider.h"
string mDefaultTtyDevice("COM5");

// linux or windows: file provider
//#include "filescreenshotprovider.h"
//string mDefaultTtyDevice("COM3");

shared_ptr<ScreenshotProvider> getScreenshot()
{
	// linux
	//return static_pointer_cast<ScreenshotProvider>(make_shared<XlibScreenshotProvider>());
	// windows: DD API
	return static_pointer_cast<ScreenshotProvider>(make_shared<DdApiScreenshotProvider>());
	// file screenshot provider, cross platform
	//return static_pointer_cast<ScreenshotProvider>(make_shared<FileScreenshotProvider>());
}

int main() {
	// get builder for connector
	AmbiConnectorBuilder builder;

	builder
		.setAmbiColorDataProvider(new AmbiColorDataProvider(60, 18, 60, 18))
		.setBorderProvider(new SingleScreenBorderProvider(1920, 1080, 0, 0))
		.setScreenshotProvider(getScreenshot());

	/*
	// Example for using a non-Ambi ColorDataProvider
	shared_ptr<ColorDataProvider> colorDataProvider = make_shared<SpectrometerColorDataProvider>(60, 18);
	ArduinoConnector connector;
	connector.setColorDataProvider(colorDataProvider);
	*/

	shared_ptr<ArduinoConnector> connector = builder.build();

	// try sending to the arduino
	//try {
	// establish connection
	connector->connect(mDefaultTtyDevice);

	// loop: update the screen images and push the data to the arduino
	//for(int i = 0; i < 100; i++) {
	while (1) {
		connector->update();
		connector->draw();

		cout << "avg draw fps:" << connector->getCurrentFps() << endl;
	}
	/*} catch(ArduinoConnectorException e) {
	cout << "Ambiconnector experienced an exception: " << e.what() << endl;
	}
	catch (SerialException e) {
	cout << "Ambiconnector experienced a serial communication exception: " << e.what() << endl;
	}*/

	return 0;
}
