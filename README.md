# ShittyAmbilight
ShittyAmbilights primary goal is to create an Ambilight-like experience on computers using an Arduino.  
It is, however, also possible to supply any other RGB data.  

# Basic Usage
## Slave
Just flash the sketch contained in ambilight-arduino to an arduino, but don't forget to adjust the number of LEDs.

## Host
The host code in ambilight-host is a powered by cmake. By default, the [CMakeLists.txt](ambilight-host/CMakeLists.txt) creates an executable file in debug mode, so to compile your version you just need to ```cmake CMakeLists.txt && make```. If you are compiling for linux, and do not want to include X11 or spectrometer (pulseaudio+fftw3) libraries, you can use ```cmake (-DX11=OFF) (-DSPECTROMETER=OFF) CMakeLists.txt``` respectively to disable including and linking them.

### Instantiation
```c++
// instantiate a Screenshot implementation, providing the means to make screenshots
shared_ptr<Screenshot> screener = shared_ptr<Screenshot>(new XlibScreenshot());

// instantiate a BorderProvider implementation so the AmbiRgbLineProvider can easily retrieve screen border samples
shared_ptr<BorderProvider> borderProvider = shared_ptr<BorderProvider>(new SingleScreenBorderProvider(1366, 768, screener));

// instantiate an AmbiRgbLineProvider, which will convert the border data to rgb data for the arduino
unique_ptr<RgbLineProvider> rgbProvider = unique_ptr<RgbLineProvider>(new AmbiRgbLineProvider(borderProvider, 60, 12));

// instantiate ArduinoConnector to send our rgbProvider data to the arduino
ArduinoConnector connector(move(rgbProvider));
```


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

