#include <iostream>
#include <algorithm>
#include <string>

#include <assert.h>

using namespace std;

#include "arduinoconnector.h"

#include "spectrometerrgblineprovider.h"
#include "ambirgblineprovider.h"

#include "customexceptions.h"

#include "singlescreenborderprovider.h"
#include "triplescreenborderprovider.h"
#include "xlibscreenshot.h"

using namespace std;

std::shared_ptr<RgbLineProvider> createAmbilightRgbProvider(){
    // instantiate the desired screenshot class
    shared_ptr<Screenshot> screener = shared_ptr<Screenshot>(new XlibScreenshot());

    // instantiate the desired borderProvider with the screener. it will use the Screenshot instance
    // to get screenshots from the system
    shared_ptr<BorderProvider> borderProvider = shared_ptr<BorderProvider>(new SingleScreenBorderProvider(1920, 1080, screener, 1024, 0));
    //shared_ptr<BorderProvider> borderProvider = shared_ptr<BorderProvider>(new TripleScreenBorderProvider(screener));

    // instantiate and return an AmbiRgbLineProvider, the RGB data source. It will use the
    // BorderProvider to get images of the borders and convert them to RGB arrays
    return shared_ptr<RgbLineProvider>(new AmbiRgbLineProvider(borderProvider, 60, 18));
}

int main() {
    // audio spectrum rgb provider
    //unique_ptr<RgbLineProvider> rgbProvider = unique_ptr<RgbLineProvider>(new SpectrometerRgbLineProvider(60, 18));

    // ambilight rgb provider
    shared_ptr<RgbLineProvider> rgbProvider = createAmbilightRgbProvider();

    // supply our AmbiConnector with its chosen RgbLineProvider
    ArduinoConnector connector(rgbProvider);

    // add the filters we want
    //connector.addFilter("lowPassFilter", unique_ptr<DataFilter>(new LowPassFilter(connector.getRequiredBufferLength())));

    // try sending to the arduino
    try {
        // establish connection
        connector.connect("/dev/ttyUSB0");

        // loop: update the screen images and push the data to the arduino
        //for(int i = 0; i < 100; i++) {
        while(1) {
            connector.update();
            connector.draw();

            cout << "avg draw fps:" << connector.getCurrentFps() << endl;
        }
    } catch(AmbiConnectorException e) {
        cout << "Ambiconnector experienced an exception: " << e.what() << endl;
    }

    return 0;
}
