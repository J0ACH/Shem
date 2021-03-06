PROJECT (Shem)

### Project setup ########################

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

#SET(CMAKE_AUTOMOC ON)
#SET(CMAKE_AUTORCC ON)

MESSAGE(STATUS "\nShem init...")

### Compiler setup ########################

IF(CMAKE_GENERATOR MATCHES "Visual Studio 12 2013")
    SET(CMAKE_CONFIGURATION_TYPES Debug Release)
    SET(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
        "Reset the configurations to what we need/support"
        FORCE
    )
ENDIF()

if(CMAKE_COMPILER_IS_GNUCXX)
      add_definitions(-std=c++11)
    endif()

### Modules setup ########################

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Modules)

MESSAGE(STATUS "Found Shem modules:")
FOREACH(ONE_CMAKE_MODULEPATH ${CMAKE_MODULE_PATH})	
	FILE(GLOB_RECURSE ONE_PATH_MODULES ${ONE_CMAKE_MODULEPATH}/*.cmake )
	FOREACH(SHEM_MODULES ${ONE_PATH_MODULES})
		MESSAGE(STATUS "\t - " ${SHEM_MODULES})
	ENDFOREACH(SHEM_MODULES)
ENDFOREACH(ONE_CMAKE_MODULEPATH)

FIND_PACKAGE(SC REQUIRED) # looking for Supercollider ( SC_FOUND || SC_INSTALL_DIR )

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	SET(CMAKE_INSTALL_PREFIX ${SC_INSTALL_DIR} CACHE PATH "Cmake install path" FORCE)
ENDIF()

### ScBridge ########################

ADD_SUBDIRECTORY(ScBridge)
SET(SCBRIDGE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ScBridge)
INCLUDE_DIRECTORIES(${SCBRIDGE_INCLUDE_DIR}) # setup ScBridge dir like include dir for all rest subdirectories

MESSAGE(STATUS "ScBridge include folders {SCBRIDGE_INCLUDE_DIR}: " ${SCBRIDGE_INCLUDE_DIR})
MESSAGE(STATUS "ScBridge linked libraies {SCBRIDGE_LINK_LIBRARIES}: ")
GET_TARGET_PROPERTY(SCBRIDGE_LINK_LIBRARIES ScBridge LINK_LIBRARIES)
FOREACH(ONE_BRGLINK ${SCBRIDGE_LINK_LIBRARIES})	
		MESSAGE(STATUS "\t - " ${ONE_BRGLINK})
ENDFOREACH(ONE_BRGLINK)

### Project semaphore ########################

OPTION(Build_Demo "Build basic example of using ScBridge" OFF)
IF(Build_Demo)
	MESSAGE(STATUS "Building the Demo")
	ADD_SUBDIRECTORY(Demo)
ENDIF()


OPTION(Build_Quant "Build project Quant" ON)
IF(Build_Quant)
	MESSAGE(STATUS "Building the Quant IDE")
	ADD_SUBDIRECTORY(Quant)
ENDIF()


MESSAGE(STATUS "\nShem done...")
