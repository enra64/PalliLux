# ShittyAmbilight
ShittyAmbilights primary aim is to create an Ambilight-like experience on computers using an Arduino for interfacing with individually adressable RGB leds. It is, however, also possible to supply other data, like a live spectrometer of your music.
The ambilight-host folder contains the host-side code, the ambilight-arduino contains the slave-side code. An Arduino must be flashed with the slave side code, adjusted to match the correct number of leds.

## Host-Code usage
To skip all the explanation of my lunatic interface and go straight to the examples, [click here](#Examples).

#### Creating an ArduinoConnector
An ```ArduinoConnector```-instance will communicate with the slave Arduino. To do so, it needs a class instance implementing the ```RgbLineProvider``` interface, which will supply it with the rgb data for the arduino.


## AmbiConnector 


## BorderProvider 
A ```BorderProvider``` is responsible for delivering screen captures of each border. The resolution only matters for the achieved fps. 

## ScreenShot 
A class extending ```ScreenShot``` provides an interface for screen capture with a specific offset and size.

# Examples
## Running
To run any ArduinoConnector combination, use the following code fragment:
```c++
// try sending to the arduino
try {
    // establish connection
    connector.connect("/dev/ttyUSB0");

    // loop: update the screen images and push the data to the arduino
    while(1) {
        connector.update();
        connector.draw();

        //std::cout << "avg fps:" << connector.getCurrentFps() << std::endl;
    }
} catch(AmbiConnectorException e) {
    cout << "Ambiconnector experienced an exception: " << e.what() << endl;
}
```
## Instantiation
### Single screen, Ambilight-mode
```c++
// create a screenshot instance
shared_ptr<Screenshot> screener = shared_ptr<Screenshot>(new XlibScreenshot());

// instantiate the desired borderProvider with screener. it will use the Screenshot instance to get screenshots from the system
shared_ptr<BorderProvider> borderProvider = shared_ptr<BorderProvider>(new SingleScreenBorderProvider(1366, 768, screener));

// instantiate an AmbiRgbLineProvider, the RGB data source. It will use the BorderProvider to get images of the borders and convert them to RGB arrays
unique_ptr<RgbLineProvider> rgbProvider = move(unique_ptr<RgbLineProvider>(new AmbiRgbLineProvider(borderProvider, 60, 12)));

// supply our AmbiConnector with its chosen RgbLineProvider
ArduinoConnector connector(move(rgbProvider));
```

### Three screens with different heights, Ambilight-mode
```c++
// create a screenshot instance
shared_ptr<Screenshot> screener = shared_ptr<Screenshot>(new XlibScreenshot());

// instantiate the desired borderProvider with screener. it will use the Screenshot instance to get screenshots from the system
shared_ptr<BorderProvider> borderProvider = shared_ptr<BorderProvider>(new TripleScreenBorderProvider(screener));

// instantiate an AmbiRgbLineProvider, the RGB data source. It will use the BorderProvider to get images of the borders and convert them to RGB arrays
unique_ptr<RgbLineProvider> rgbProvider = move(unique_ptr<RgbLineProvider>(new AmbiRgbLineProvider(borderProvider, 60, 12)));

// supply our AmbiConnector with its chosen RgbLineProvider
ArduinoConnector connector(move(rgbProvider));
```

### Spectrometer-mode
```c++
// instantiate an SpectrometerRgbLineProvider, the RGB data source.
unique_ptr<RgbLineProvider> rgbProvider = move(unique_ptr<RgbLineProvider>(new SpectrometerRgbLineProvider()));

// supply our AmbiConnector with its chosen RgbLineProvider
ArduinoConnector connector(move(rgbProvider));
```
