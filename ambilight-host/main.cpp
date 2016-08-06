#include <iostream>
#include <algorithm>
#include <string>

#include <assert.h>

using namespace std;

#include "ambiconnector.h"
#include "rgbconverter.h"


#include "singlescreenborderprovider.h"
#include "xlibscreenshot.h"

using namespace std;

int main() {
    // create a unique pointer to an instance of the desired screenshot class
    shared_ptr<Screenshot> screener = shared_ptr<Screenshot>(new XlibScreenshot());

    // create a shared pointer to the desired borderProvider
    shared_ptr<BorderProvider> provider =
            shared_ptr<BorderProvider>(new SingleScreenBorderProvider(1366, 768, screener));

    // supply our ledconnector with the count of horizontal/vertical leds on each border
    AmbiConnector connector(provider, 60, 12);

    // add the filters we want
    //connector.addFilter("brightnessFilter", unique_ptr<DataFilter>(new LowPassFilter(connector.getRequiredBufferLength())));

    // try sending to the arduino
    try {
        // establish connection
        connector.connect("/dev/ttyUSB0");

        // loop: update the screen images and push the data to the arduino
        //for(int i = 0; i < 100; i++) {
        while(1) {
            connector.update();
            connector.draw();

            //cout << "avg draw fps:" << connector.getCurrentFps() << endl;
        }
    } catch(AmbiConnectorException* e) {
        cout << "Ambiconnector experienced an exception: " << e->what() << endl;
    }

    return 0;
}
