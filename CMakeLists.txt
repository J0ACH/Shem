PROJECT (Shem)

### Project setup ########################

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

#SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

SET(CMAKE_AUTOMOC ON)

### Path setup ########################

MESSAGE(STATUS "\nShem init...")

### Compiler setup ########################

IF(CMAKE_GENERATOR MATCHES "Visual Studio 12 2013")
    SET(CMAKE_CONFIGURATION_TYPES Debug Release)
    SET(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
        "Reset the configurations to what we need/support"
        FORCE
    )
ENDIF()

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/Install" CACHE STRING "Cmake install path" FORCE)
ENDIF()

### Modules setup ########################

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Modules)

FIND_PACKAGE(Sc) # looking for SC

#LIST(APPEND CMAKE_MODULE_PATH 
        #${CMAKE_MODULE_PATH}	
        #${SC_SOURCE_DIR}/cmake_modules
#)

MESSAGE(STATUS "Shem can use this modules:")
FOREACH(ONE_CMAKE_MODULEPATH ${CMAKE_MODULE_PATH})	
	FILE(GLOB_RECURSE ONE_PATH_MODULES ${ONE_CMAKE_MODULEPATH}/*.cmake )
	FOREACH(SHEM_MODULES ${ONE_PATH_MODULES})
		MESSAGE(STATUS "\t - " ${SHEM_MODULES})
	ENDFOREACH(SHEM_MODULES)
ENDFOREACH(ONE_CMAKE_MODULEPATH)

### Subdirectories ########################

#ADD_SUBDIRECTORY(sc-ide)
ADD_SUBDIRECTORY(ScBridge)
ADD_SUBDIRECTORY(Quant)

MESSAGE(STATUS "\nShem done...")
