project(host)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

# get qt creator to show these files
SET(MYHEADERS
    "include/customexceptions.h"
    "include/implementations"
    "include/implementations/interfaces"
    "include/implementations/interfaces/borderprovider.h"
    "include/implementations/interfaces/rgblineprovider.h"
    "include/implementations/interfaces/datafilter.h"
    "include/implementations/interfaces/screenshot.h"
    "include/implementations/borderproviders"
    "include/implementations/borderproviders/singlescreenborderprovider.h"
    "include/implementations/borderproviders/triplescreenborderprovider.h"
    "include/implementations/screenshot"
    "include/implementations/screenshot/vlcscreenshot.h"
    "include/implementations/screenshot/xlibscreenshot.h"
    "include/implementations/rgblineprovider"
    "include/implementations/rgblineprovider/spectrometerrgblineprovider.h"
    "include/implementations/rgblineprovider/ambirgblineprovider.h"
    "include/arduinoconnector.h")

# relevant source files
SET(MYSOURCES
    "src/arduinoconnector.cpp"
    "src/borderproviders/singlescreenborderprovider.cpp"
    "src/borderproviders/triplescreenborderprovider.cpp"
    "src/rgblineprovider/ambirgblineprovider.cpp"
    "src/rgblineprovider/spectrometerrgblineprovider.cpp"
    "src/screenshot/vlcscreenshot.cpp"
    "src/screenshot/xlibscreenshot.cpp")

#include subdirectories
include_directories("include" "include/implementations"
    "include/implementations/interfaces"
    "include/implementations/borderproviders"
    "include/implementations/screenshot"
    "include/implementations/rgblineprovider")

add_executable(${PROJECT_NAME} ${SRC_LIST} ${MYSOURCES} ${MYHEADERS})

# LIB


# imageMagick
find_package(ImageMagick COMPONENTS Magick++ MagickCore convert composite REQUIRED)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})
target_compile_options(${PROJECT_NAME} PUBLIC "-DMAGICKCORE_HDRI_ENABLE=0" "-DMAGICKCORE_QUANTUM_DEPTH=16")

#X11
find_package(X11 REQUIRED)
INCLUDE_DIRECTORIES ( ${X11_INCLUDE_DIR} )
TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${X11_LIBRARIES} )

# spectrometer:
#fftw
find_path (FFTW_INCLUDE_DIR fftw3.h)
find_library (FFTW_LIBRARIES NAMES fftw3)
INCLUDE_DIRECTORIES ( ${FFTW_INCLUDE_DIR} )
TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${FFTW_LIBRARIES} )
#pulseaudio
#INCLUDE_DIRECTORIES (  )
TARGET_LINK_LIBRARIES (${PROJECT_NAME} "pulse")
TARGET_LINK_LIBRARIES (${PROJECT_NAME} "pulse-simple")
