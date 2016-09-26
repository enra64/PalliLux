project(host)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

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
	"cimg/CImg.h"
    "include/arduinoconnector.h"
    "include/ambiconnectorbuilder.h"
    "include/customexceptions.h"
	"include/geometry.h"
    "include/letterboxometer.h"
    "include/borderproviders/singlescreenborderprovider.h"
    "include/borderproviders/triplescreenborderprovider.h"
    "include/datafilters/brightnessfilter.h"
    "include/datafilters/linearfactorfilter.h"
    "include/datafilters/lowpassfilter.h"
    "include/datafilters/pulsingfilter.h"
    "include/colordataproviders/ambicolordataprovider.h"
    "include/colordataproviders/constantcolordataprovider.h"
    "include/interfaces/borderprovider.h"
    "include/interfaces/datafilter.h"
    "include/interfaces/colordataprovider.h"
    "include/interfaces/screenshotprovider.h"
    "include/interfaces/serial.h")


# relevant source files
SET(BASE_SOURCES
    "src/arduinoconnector.cpp"
    "src/ambiconnectorbuilder.cpp"
    "src/letterboxometer.cpp"
    "src/borderproviders/singlescreenborderprovider.cpp"
    "src/borderproviders/triplescreenborderprovider.cpp"
    "src/datafilters/brightnessfilter.cpp"
    "src/datafilters/linearfactorfilter.cpp"
    "src/datafilters/lowpassfilter.cpp"
    "src/datafilters/pulsingfilter.cpp"
    "src/colordataproviders/ambicolordataprovider.cpp"
    "src/colordataproviders/constantcolordataprovider.cpp")


#include subdirectories
include_directories(
	"cimg"
    "include"
    "include/serial"
    "include/interfaces"
    "include/borderproviders"
    "include/screenshotproviders"
    "include/datafilters"
    "include/colordataproviders")
    


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
    "include/screenshotproviders/winscreenshotprovider.h")
    
SET(WIN_ONLY_SOURCES
    "src/serial/windowsserial.cpp"
    "src/screenshotproviders/winscreenshotprovider.cpp")
    
if(WIN32)
    SET(BASE_HEADERS ${BASE_HEADERS} ${WIN_ONLY_HEADERS})
    SET(BASE_SOURCES ${BASE_SOURCES} ${WIN_ONLY_SOURCES})
endif(WIN32)



######################################################################################################################################################
######################################################################################################################################################
# LIBRARY OR EXECUTABLE ?
######################################################################################################################################################
######################################################################################################################################################
option(MAIN_CPP_EXECUTABLE "compile the main.cpp, and create an executable file" OFF)

if(MAIN_CPP_EXECUTABLE)
	add_executable(${PROJECT_NAME} "main.cpp" ${BASE_SOURCES} ${BASE_HEADERS})
else(MAIN_CPP_EXECUTABLE)
	add_library(${PROJECT_NAME} ${BASE_SOURCES} ${BASE_HEADERS})
endif(MAIN_CPP_EXECUTABLE)


######################################################################################################################################################
######################################################################################################################################################
# debug mode?
######################################################################################################################################################
######################################################################################################################################################
option(DEBUG_MODE "use cmake debug build type" ON)

if(DEBUG_MODE)
	set(CMAKE_BUILD_TYPE Debug)
endif(DEBUG_MODE)


######################################################################################################################################################
# OPTIONS
######################################################################################################################################################
option(X11 "find and include x11 lib" ON)
option(SPECTROMETER "find and include fftw3 and pulse libs" ON)


######################################################################################################################################################
# LINUX ONLY LIBRARIES
######################################################################################################################################################
if(UNIX)


######################################################################################################################################################
# X11 OPTION - needed for xlibscreenshot - LINUX ONLY
######################################################################################################################################################
    if(X11)
        target_sources(${PROJECT_NAME} PUBLIC "src/screenshotproviders/xlibscreenshotprovider.cpp" "include/screenshotproviders/xlibscreenshotprovider.h")
        find_package(X11 REQUIRED)
        INCLUDE_DIRECTORIES ( ${X11_INCLUDE_DIR} )
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${X11_LIBRARIES})
    endif(X11)


######################################################################################################################################################
# SPECTROMETER OPTION - LINUX ONLY
######################################################################################################################################################
    if(SPECTROMETER)
        # add spectrometer files to target
        target_sources(${PROJECT_NAME} PUBLIC "include/colordataproviders/spectrometercolordataprovider.h" "src/colordataproviders/spectrometercolordataprovider.cpp")
        
        #  pulseaudio libs
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} "pulse")
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} "pulse-simple")
        
        # fftw lib
        find_path (FFTW_INCLUDE_DIR fftw3.h)
        find_library (FFTW_LIBRARIES NAMES fftw3)
        INCLUDE_DIRECTORIES ( ${FFTW_INCLUDE_DIR} )
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${FFTW_LIBRARIES} )
    endif(SPECTROMETER)

endif(UNIX)
