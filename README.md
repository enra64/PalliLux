# ShittyAmbilight
ShittyAmbilights primary goal is to create an Ambilight-like experience on computers using an Arduino.  
It is, however, also possible to supply any other RGB data.  

# Basic Usage
## Slave
Just flash the sketch contained in ambilight-arduino to an arduino, but don't forget to adjust the number of LEDs.

## Host
The host code in ambilight-host is a powered by cmake. By default, the [CMakeLists.txt](ambilight-host/CMakeLists.txt) creates an executable file in debug mode, so to compile your version you just need to ```cmake CMakeLists.txt && make```. If you are compiling for linux, and do not want to include X11 or spectrometer (pulseaudio+fftw3) libraries, you can use ```cmake (-DX11=OFF) (-DSPECTROMETER=OFF) CMakeLists.txt``` respectively to disable including and linking them.

### Instantiation (using Builder for Ambilight mode)
```c++
    AmbiConnectorBuilder builder;

    // instantiate appropriate classes
    builder.setScreenshotProvider(shared_ptr<Screenshot>(new XlibScreenshot()));
    builder.setBorderProvider(shared_ptr<BorderProvider>(new SingleScreenBorderProvider(1366, 768)));
    builder.setAmbiRgbLineProvider(shared_ptr<AmbiRgbLineProvider>(new AmbiRgbLineProvider(60, 12)));
    
    // set the port
    builder.setPort("/dev/ttyUSB0");
    
    // get the connector from the instance
    ArduinoConnector connector = builder.build();
```

### Instantiation (other ColorDataProviders)
You may also use ```ArduinoConnector::setRgbLineProvider(shared_ptr<RgbLinProvider> p)``` directly to use other RgbLineProviders, for example ```SpectrometerRgbLineProvider```.


### Running
To run an ```ArduinoConnector```, you may use the following code fragment:
```c++
// establish connection
connector.connect("/dev/ttyUSB0");

// loop: update the screen images and push the data to the arduino
while(1) {
    // update screenshot data
    connector.update();
    
    // send updated data to arduino
    connector.draw();

    // show the fps
    std::cout << "avg fps:" << connector.getCurrentFps() << std::endl;
}
```
Be aware that an ```AmbiConnectorException``` will be thrown if something goes wrong.

# More
[More info about the classes](class_info.md). In [ambilight-gui](ambilight-gui/) there is a gui uising Qt for single and triple screen ambilight.

## SpectrometerRgbLineProvider
This class can create a spectrogram of your current music using the fftw3 library and pulseaudio.

## DataFilters
```DataFilter``` implementations may modify the RgbLineProvider result rgb data in any way, for example to reduce the overall brightness or modify color behaviour. They can be added like this:
```c++
unique_ptr<DataFilter> myFilter = unique_ptr<DataFilter>(new MyFilter()));
connector.addFilter("myFilterId", move(myFilter));
```
#### Existing ```DataFilter``` implementations
|Filter class|effect|
|------------|------|
|BrightnessFilter|Apply a factor to all rgb channels|
|LowPassFilter|Smooth out the color changes by mixing old and new data|
|LinearFactorFilter|Suppress/enhance bright/dark colors|

