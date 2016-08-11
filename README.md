# ShittyAmbilight
ShittyAmbilights primary goal is to create an Ambilight-like experience on computers using an Arduino.  
It is, however, also possible to supply any other RGB data.  

# Usage
## Slave
Just flash the sketch contained in ambilight-arduino to an arduino, but don't forget to adjust the number of LEDs.

## Host
To skip the explanation and go straight to the examples, [click here](#examples).

### ArduinoConnector
An ```ArduinoConnector``` instance will communicate with the slave Arduino. To do so, it needs a class instance implementing the ```RgbLineProvider``` interface, which will supply the RGB data for the arduino. If used as an Ambilight, there are some other classes that need explanation:

### AmbiRgbLineProvider
```AmbiRgbLineProvider``` implements the ```RgbLineProvider``` interface. It pulls captures of the screen borders from a ```BorderProvider``` implementation, and converts them to RGB data. 

### BorderProvider
A ```BorderProvider``` instance uses an implementation of ```Screenshot``` to capture the screen borders. The height does not matter, as the images will be scaled 
to be a single pixel high anyway, but beware of the orientation, because ```AmbiRgbLineProvider``` will rotate the borders.

### Screenshot
A ```Screenshot``` implementation allows to capture screen areas with a specified size and offset. The ```XlibScreenshot``` uses the X server to do so, so it needs an environment where it is available. A windows ```Screenshot``` implementation is currently not available.

# Examples
## Instantiation
### Basic:
```c++
// instantiate any RgbLineProvider implemenation
unique_ptr<RgbLineProvider> rgbProvider = unique_ptr<RgbLineProvider>(new SpectrometerRgbLineProvider());

// supply our ArduinoConnector with the RgbLineProvider
ArduinoConnector connector(move(rgbProvider));
```

### Ambilight-mode
The existing implementations for BorderProviders need a screenshot implementation. If you want to modify behaviour, you can swap out the implementations of 
```Screenshot``` or ```BorderProvider```.
```c++
// instantiate any Screenshot implementation
shared_ptr<Screenshot> screener = shared_ptr<Screenshot>(new XlibScreenshot());

// instantiate any BorderProvider implementation. It does not have to use a screenshot class!
shared_ptr<BorderProvider> borderProvider = shared_ptr<BorderProvider>(new SingleScreenBorderProvider(1366, 768, screener));

// instantiate an AmbiRgbLineProvider
unique_ptr<RgbLineProvider> rgbProvider = unique_ptr<RgbLineProvider>(new AmbiRgbLineProvider(borderProvider, 60, 12));

// supply our AmbiConnector with its chosen RgbLineProvider
ArduinoConnector connector(move(rgbProvider));
```

## Running
To run an ```ArduinoConnector```, you may use the following code fragment:
```c++
// try sending to the arduino
try {
    // establish connection
    connector.connect("/dev/ttyUSB0");

    // loop: update the screen images and push the data to the arduino
    while(1) {
        // update screenshot data
        connector.update();
        
        // send updated data to arduino
        connector.draw();

        //std::cout << "avg fps:" << connector.getCurrentFps() << std::endl;
    }
} catch(AmbiConnectorException e) {
    cout << "Ambiconnector experienced an exception: " << e.what() << endl;
}
```

# DataFilter
```DataFilter``` implementations may modify the incoming RGB data in any way, for example to reduce the overall brightness, or modify color behaviour etc... They can be added like this:
```c++
arduinoConnector.addFilter("lowPassFilter", unique_ptr<DataFilter>(new LowPassFilter(connector.getRequiredBufferLength())));
```
