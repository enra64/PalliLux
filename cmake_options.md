# Available cmake options

## Options available on all platforms

### MAIN_CPP_EXECUTABLE
Compile ambilight-host/main.cpp together with the library code, and create an executable.

### DYNAMIC_LIB
Switch between creating dynamic or static library files. Only available if MAIN_CPP_EXECUTABLE is not checked.

### DEBUG_MODE
Sets the CMAKE_BUILD_TYPE to Debug.

## Options available on *windows*
### DIRECTX
Include DirectX 9 libraries; the DirectX SDK is required.

## Options available on *linuxy-things*
### X11
Include Xlib and the screenshotproviders based on it. (recommended)

### SPECTROMETER
Include fftw3, the pulseaudio libs, and the spectrometer color data provider (unfinished!).
