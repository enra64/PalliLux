// system includes
#include <iostream>
#include <string>
#include <assert.h>

// pallilux includes
#include "arduinoconnector.h"
#include "ambicolordataprovider.h"
#include "customexceptions.h"
#include "singlescreenborderprovider.h"
#include "ambiconnectorbuilder.h"


/*
 *
 * Include a ScreenshotProvider appropriate for this platform 
 *
 */
#define USE_DX 1

#ifdef __linux__
	#include "xlibscreenshotprovider.h"
	string mDefaultTtyDevice("/dev/ttyUSB0");
	
	// on windows, currently a ScreenshotProvider using GDI, and one using DirectX9 are available
#elif _WIN32 && !USE_DX
    // use the GDI ScreenshotProvider
	#include "winscreenshotprovider.h"
	string mDefaultTtyDevice("COM3");
#elif _WIN32 && USE_DX
    // use the DirectX ScreenshotProvider
	#include "d3dscreenshotprovider.h"
	string mDefaultTtyDevice("COM3");
#else
#error Platform not recognized
#endif

/**
 * @brief A helper function returning a ScreenshotProvider appropriate for the current platform
 */
shared_ptr<ScreenshotProvider> getScreenshotProvider()
{
	#ifdef __linux__
	return static_pointer_cast<ScreenshotProvider>(make_shared<XlibScreenshotProvider>());
	#elif _WIN32 && !USE_DX
	return static_pointer_cast<ScreenshotProvider>(make_shared<WinScreenshotProvider>());
	#elif _WIN32 && USE_DX
	return static_pointer_cast<ScreenshotProvider>(make_shared<D3DScreenshotProvider>());
	#else
	#error Platform not recognized
	#endif
}

using namespace std;

int main() {
    /*
	// Example for using a non-Ambi ColorDataProvider
	shared_ptr<ColorDataProvider> colorDataProvider = make_shared<SpectrometerColorDataProvider>(60, 18);
	ArduinoConnector connector;
	connector.setColorDataProvider(colorDataProvider);
	*/
	
	// Get an AmbiConnectorBuilder to help creating an ArduinoConnector in Ambilight-mode
	AmbiConnectorBuilder builder;

    /*
     *
     * The following three classes must be set to create an ArduinoConnector
     *
     */
    
    // led count: 60 bottom, 18 right, 60 top, 18 left
	builder.setAmbiColorDataProvider(new AmbiColorDataProvider(60, 18, 60, 18));
		 
    // screen resolution and offset
    builder.setBorderProvider(new SingleScreenBorderProvider(1920, 1080, 0, 0)); 
		
	// set an appropriate ScreenshotProvider supplied by our helper function
    builder.setScreenshotProvider(getScreenshotProvider());



    /*
     *
     * After we created the connector, we can connect to the Arduino and
     * begin retrieving the screen data and push it onto the leds
     *
     */
     
     // retrieve the ArduinoConnector
	shared_ptr<ArduinoConnector> connector = builder.build();

    try {
        // establish connection to the set port
        connector->connect(mDefaultTtyDevice);

        // update the screen images and push the data to the arduino
        while(1) {
            connector->update();
            connector->draw();

            cout << "avg draw fps:" << connector->getCurrentFps() << endl;
        }
    } catch(ArduinoConnectorException e) {
        cout << "ArduinoConnectorException: " << e.what() << endl;
	}
	catch (SerialException e) {
		cout << "SerialException: " << e.what() << endl;
	}

    return 0;
}
