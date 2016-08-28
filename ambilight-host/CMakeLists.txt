project(host)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

######################################################################################################################################################
######################################################################################################################################################
######################################################################################################################################################
#
#
#
# Available options are X11 and SPECTROMETER, both on linux only, both enabled by default
# use "cmake CMakeLists.txt (-DX11=OFF) (-DSPECTROMETER=OFF)" to disable them
#
#
#
######################################################################################################################################################
######################################################################################################################################################
######################################################################################################################################################


# get qt creator to show these headers
SET(BASE_HEADERS
    "include/arduinoconnector.h"
    "include/customexceptions.h"
    "include/letterboxometer.h"
    "include/borderproviders/singlescreenborderprovider.h"
    "include/borderproviders/triplescreenborderprovider.h"
    "include/datafilters/brightnessfilter.h"
    "include/datafilters/linearfactorfilter.h"
    "include/datafilters/lowpassfilter.h"
    "include/rgblineprovider/ambirgblineprovider.h"
    "include/screenshot/vlcscreenshot.h"
    "include/interfaces/borderprovider.h"
    "include/interfaces/datafilter.h"
    "include/interfaces/rgblineprovider.h"
    "include/interfaces/screenshot.h"
    "include/interfaces/serial.h")


# relevant source files
SET(BASE_SOURCES
    "src/arduinoconnector.cpp"
    "src/letterboxometer.cpp"
    "src/borderproviders/singlescreenborderprovider.cpp"
    "src/borderproviders/triplescreenborderprovider.cpp"
    "src/datafilters/brightnessfilter.cpp"
    "src/datafilters/linearfactorfilter.cpp"
    "src/datafilters/lowpassfilter.cpp"
    "src/rgblineprovider/ambirgblineprovider.cpp"
    "src/screenshot/vlcscreenshot.cpp")


#include subdirectories
include_directories(
    "include"
    "include/serial"
    "include/interfaces"
    "include/borderproviders"
    "include/screenshot"
    "include/datafilters"
    "include/rgblineprovider")
    


######################################################################################################################################################
# LINUX
######################################################################################################################################################
SET(LINUX_ONLY_HEADERS
    "include/serial/linuxserial.h")
    
SET(LINUX_ONLY_SOURCES
    "src/serial/linuxserial.cpp")
    
if(UNIX)
    SET(BASE_HEADERS ${BASE_HEADERS} ${LINUX_ONLY_HEADERS})
    SET(BASE_SOURCES ${BASE_SOURCES} ${LINUX_ONLY_SOURCES})
endif(UNIX)
    
    
    
    
######################################################################################################################################################
# WIN
######################################################################################################################################################
SET(WIN_ONLY_HEADERS
    "include/serial/windowsserial.h"
    "include/screenshot/winscreenshot.h")
    
SET(WIN_ONLY_SOURCES
    "src/serial/windowsserial.cpp"
    "src/screenshot/winscreenshot.cpp")
    
if(WIN32)
    SET(BASE_HEADERS ${BASE_HEADERS} ${WIN_ONLY_HEADERS})
    SET(BASE_SOURCES ${BASE_SOURCES} ${WIN_ONLY_SOURCES})
endif(WIN32)



######################################################################################################################################################
######################################################################################################################################################
# LIBRARY OR EXECUTABLE ?
######################################################################################################################################################
######################################################################################################################################################
add_executable(${PROJECT_NAME} "main.cpp" ${BASE_SOURCES} ${BASE_HEADERS})
#add_library(${PROJECT_NAME} ${BASE_SOURCES} ${BASE_HEADERS})


######################################################################################################################################################
######################################################################################################################################################
# debug mode?
######################################################################################################################################################
######################################################################################################################################################
set(CMAKE_BUILD_TYPE Debug)



######################################################################################################################################################
# OPTIONS
######################################################################################################################################################
option(X11 "find and include x11 lib" ON)
option(SPECTROMETER "find and include fftw3 and pulse libs" ON)


######################################################################################################################################################
# IMAGEMAGICK - REQUIRED
######################################################################################################################################################
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})
target_compile_options(${PROJECT_NAME} PUBLIC "-DMAGICKCORE_HDRI_ENABLE=1" "-DMAGICKCORE_QUANTUM_DEPTH=16")

######################################################################################################################################################
# LINUX ONLY LIBRARIES
######################################################################################################################################################
if(UNIX)


######################################################################################################################################################
# X11 OPTION - needed for xlibscreenshot - LINUX ONLY
######################################################################################################################################################
    if(X11)
        target_sources(${PROJECT_NAME} PUBLIC "src/screenshot/xlibscreenshot.cpp" "include/screenshot/xlibscreenshot.h")
        find_package(X11 REQUIRED)
        INCLUDE_DIRECTORIES ( ${X11_INCLUDE_DIR} )
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${X11_LIBRARIES})
    endif(X11)


######################################################################################################################################################
# SPECTROMETER OPTION - LINUX ONLY
######################################################################################################################################################
    if(SPECTROMETER)
        # add spectrometer files to target
        target_sources(${PROJECT_NAME} PUBLIC "include/rgblineprovider/spectrometerrgblineprovider.h" "src/rgblineprovider/spectrometerrgblineprovider.cpp")
        
        # pulseaudio libs
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} "pulse")
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} "pulse-simple")
        
        # fftw lib
        find_path (FFTW_INCLUDE_DIR fftw3.h)
        find_library (FFTW_LIBRARIES NAMES fftw3)
        INCLUDE_DIRECTORIES ( ${FFTW_INCLUDE_DIR} )
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${FFTW_LIBRARIES} )
    endif(SPECTROMETER)

endif(UNIX)
