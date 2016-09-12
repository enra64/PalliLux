#include <iostream>
#include <string>

#include <assert.h>

#include "arduinoconnector.h"
#include "ambicolordataprovider.h"
#include "customexceptions.h"
#include "singlescreenborderprovider.h"
#include "ambiconnectorbuilder.h"

using namespace std;

#ifdef __linux__
	#include "xlibscreenshotprovider.h"
	string mDefaultTtyDevice("/dev/ttyUSB0");
#elif _WIN32
	#include "winscreenshotprovider.h"
	string mDefaultTtyDevice("COM3");
#else
#error Platform not recognized
#endif

shared_ptr<ScreenshotProvider> getScreenshot()
{
	#ifdef __linux__
	return static_pointer_cast<ScreenshotProvider>(make_shared<XlibScreenshotProvider>());
	#elif _WIN32
	return static_pointer_cast<ScreenshotProvider>(make_shared<WinScreenshotProvider>());
	#else
	#error Platform not recognized
	#endif
}

int main() {
	// get builder for connector
	AmbiConnectorBuilder builder;

	builder
		.setAmbiColorDataProvider(new AmbiColorDataProvider(60, 18))
		.setBorderProvider(new SingleScreenBorderProvider(1920, 1080, 1024, 0))
		.setScreenshotProvider(getScreenshot());

	/*
	// Example for using a non-Ambi ColorDataProvider
	shared_ptr<ColorDataProvider> colorDataProvider = make_shared<SpectrometerColorDataProvider>(60, 18);
	ArduinoConnector connector;
	connector.setColorDataProvider(colorDataProvider);
	*/

	shared_ptr<ArduinoConnector> connector = builder.build();

    // try sending to the arduino
    try {
        // establish connection
        connector->connect(mDefaultTtyDevice);

        // loop: update the screen images and push the data to the arduino
        //for(int i = 0; i < 100; i++) {
        while(1) {
            connector->update();
            connector->draw();

            cout << "avg draw fps:" << connector->getCurrentFps() << endl;
        }
    } catch(ArduinoConnectorException e) {
        cout << "Ambiconnector experienced an exception: " << e.what() << endl;
	}/*
	catch (SerialException e) {
		cout << "Ambiconnector experienced a serial communication exception: " << e.what() << endl;
	}*/

    return 0;
}
