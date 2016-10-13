project(pallilux)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# get qt creator to show these headers
SET(BASE_HEADERS
    "cimg/CImg.h"
    "cimg/load_custom_channel_order.h"
    "cimg/twodimensionalhistogram.h"
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
    option(DYNAMIC_LIB "compile to a dynamic library instead of a static one" OFF)
    if(DYNAMIC_LIB)
        add_library(${PROJECT_NAME} SHARED ${BASE_SOURCES} ${BASE_HEADERS})
    else(DYNAMIC_LIB)
        add_library(${PROJECT_NAME} ${BASE_SOURCES} ${BASE_HEADERS})
    endif(DYNAMIC_LIB)
endif(MAIN_CPP_EXECUTABLE)


######################################################################################################################################################
######################################################################################################################################################
# debug mode?
######################################################################################################################################################
######################################################################################################################################################
option(DEBUG_MODE "use cmake debug build type" OFF)

if(DEBUG_MODE)
	set(CMAKE_BUILD_TYPE Debug)
endif(DEBUG_MODE)



######################################################################################################################################################
# direct x screenshot support
# Thanks to the people of h3dapi for the find-direct-x-snippet:
# https://www.h3dapi.org:8090/HAPI/trunk/build/modules/FindDirectX.cmake
######################################################################################################################################################
if(WIN32)
	option(DIRECTX "include direct x screenshot provider" ON)

	if(DIRECTX)
		# Look for the header file.
		FIND_PATH( DirectX_INCLUDE_DIR 
					NAMES 
						d3d9.h
					PATHS 
					   "$ENV{DXSDK_DIR}Include"
				    DOC "Path in which d3dx9.h and d3d9.h are located." )
		
		# Look for the library.
		FIND_LIBRARY( DirectX_d3d9_LIBRARY 
					NAMES d3d9.lib
					PATHS "$ENV{DXSDK_DIR}Lib\\x86"
					DOC "Path to d3d9.lib" )
					
		FIND_LIBRARY( DirectX_d3dx9_LIBRARY 
					NAMES d3dx9.lib
					PATHS "$ENV{DXSDK_DIR}Lib\\x86"
					DOC "Path to d3dx9.lib" )

		# Copy the results to the output variables.
		IF(DirectX_INCLUDE_DIR AND DirectX_d3d9_LIBRARY AND DirectX_d3dx9_LIBRARY )
			SET(DirectX_FOUND 1)
			SET(DirectX_LIBRARIES ${DirectX_d3d9_LIBRARY} ${DirectX_d3dx9_LIBRARY} )
			SET(DirectX_INCLUDE_DIR ${DirectX_INCLUDE_DIR})
		ELSE(DirectX_INCLUDE_DIR AND DirectX_d3d9_LIBRARY AND DirectX_d3dx9_LIBRARY )		  
			# report that this just wont work
			MESSAGE(FATAL_ERROR "DirectX was not found!")
		ENDIF(DirectX_INCLUDE_DIR AND DirectX_d3d9_LIBRARY AND DirectX_d3dx9_LIBRARY )
		
		target_sources(${PROJECT_NAME} 
			PUBLIC 
				"src/screenshotproviders/d3dscreenshotprovider.cpp"
				"include/screenshotproviders/d3dscreenshotprovider.h")
	
		INCLUDE_DIRECTORIES ( ${DirectX_INCLUDE_DIR} )
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${DirectX_d3d9_LIBRARY} ${DirectX_d3dx9_LIBRARY})
	
	endif(DIRECTX)
endif(WIN32)


######################################################################################################################################################
# LINUX ONLY LIBRARIES
######################################################################################################################################################
if(UNIX)

	######################################################################################################################################################
	# OPTIONS	
	######################################################################################################################################################
	option(X11 "find and include x11 lib" ON)
	


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
    # find pulseaudio libs
    find_library(PULSEAUDIO_LIBRARY
                NAMES pulse
                DOC "The PulseAudio library")
                
    find_library(PULSEAUDIO_SIMPLE_LIBRARY
                NAMES pulse-simple
                DOC "The PulseAudio library")
                
    # find fftw lib
    find_path (FFTW_INCLUDE_DIR fftw3.h)
    find_library (FFTW_LIBRARIES NAMES fftw3)
    
    # if all required libs are available, give option to enable spectrometer system
    if(FFTW_LIBRARIES AND FFTW_INCLUDE_DIR AND PULSEAUDIO_LIBRARY AND PULSEAUDIO_SIMPLE_LIBRARY)
        option(SPECTROMETER "find and include fftw3 and pulse libs" OFF)
    endif(FFTW_LIBRARIES AND FFTW_INCLUDE_DIR AND PULSEAUDIO_LIBRARY AND PULSEAUDIO_SIMPLE_LIBRARY)
                
    # if spectrometer option enabled
    if(SPECTROMETER)
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${PULSEAUDIO_LIBRARY} ${PULSEAUDIO_LIBRARY})
        INCLUDE_DIRECTORIES ( ${FFTW_INCLUDE_DIR} )
        TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${FFTW_LIBRARIES} )
        
        # add spectrometer files to target
        target_sources(${PROJECT_NAME} PUBLIC "include/colordataproviders/spectrometercolordataprovider.h" "src/colordataproviders/spectrometercolordataprovider.cpp")
    endif(SPECTROMETER)

endif(UNIX)
