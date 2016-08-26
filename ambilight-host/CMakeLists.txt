project(host)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# get qt creator to show these headers
SET(MYHEADERS
    "include/arduinoconnector.h"
    "include/customexceptions.h"
    "include/letterboxometer.h"
    "include/borderproviders/singlescreenborderprovider.h"
    "include/borderproviders/triplescreenborderprovider.h"
    "include/datafilters/brightnessfilter.h"
    "include/datafilters/linearfactorfilter.h"
    "include/datafilters/lowpassfilter.h"
    "include/rgblineprovider/ambirgblineprovider.h"
    "include/rgblineprovider/spectrometerrgblineprovider.h"
    "include/screenshot/vlcscreenshot.h"
    "include/screenshot/xlibscreenshot.h"
    "include/interfaces/borderprovider.h"
    "include/interfaces/datafilter.h"
    "include/interfaces/rgblineprovider.h"
    "include/interfaces/screenshot.h")

# relevant source files
SET(MYSOURCES
    "src/arduinoconnector.cpp"
    "src/letterboxometer.cpp"
    "src/borderproviders/singlescreenborderprovider.cpp"
    "src/borderproviders/triplescreenborderprovider.cpp"
    "src/datafilters/brightnessfilter.cpp"
    "src/datafilters/linearfactorfilter.cpp"
    "src/datafilters/lowpassfilter.cpp"
    "src/rgblineprovider/ambirgblineprovider.cpp"
    "src/rgblineprovider/spectrometerrgblineprovider.cpp"
    "src/screenshot/vlcscreenshot.cpp"
    "src/screenshot/xlibscreenshot.cpp")

#include subdirectories
include_directories(
    "include"
    "include/interfaces"
    "include/borderproviders"
    "include/screenshot"
    "include/datafilters"
    "include/rgblineprovider")

#debug mode?
set(CMAKE_BUILD_TYPE Debug)

# create a library or an executable?
#add_executable(${PROJECT_NAME} "main.cpp" ${MYSOURCES} ${MYHEADERS})
add_library(${PROJECT_NAME} ${MYSOURCES} ${MYHEADERS})

# library stuff that needs to be included etc
# imageMagick
find_package(ImageMagick COMPONENTS Magick++ MagickCore REQUIRED)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})
target_compile_options(${PROJECT_NAME} PUBLIC "-DMAGICKCORE_HDRI_ENABLE=1" "-DMAGICKCORE_QUANTUM_DEPTH=16")



# X11 - needed for xlibborderprovider
find_package(X11 REQUIRED)
INCLUDE_DIRECTORIES ( ${X11_INCLUDE_DIR} )
TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${X11_LIBRARIES} )



# libraries used only for spectrometer functionality:
#fftw
find_path (FFTW_INCLUDE_DIR fftw3.h)
find_library (FFTW_LIBRARIES NAMES fftw3)
INCLUDE_DIRECTORIES ( ${FFTW_INCLUDE_DIR} )
TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${FFTW_LIBRARIES} )

#pulseaudio
TARGET_LINK_LIBRARIES (${PROJECT_NAME} "pulse")
TARGET_LINK_LIBRARIES (${PROJECT_NAME} "pulse-simple")
