PROJECT (Shem)

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

MESSAGE(STATUS "\nShem init...")

SET(SC_SRC_DIR "D:/Supercollider/supercollider" CACHE PATH "Supercollider source path")
SET(SC_BUILD_DIR "D:/Supercollider/Supercollider_findModules" CACHE PATH "Supercollider build path")

SET(CMAKE_MODULE_PATH 
	${CMAKE_CURRENT_SOURCE_DIR}/Modules 
	${SC_SRC_DIR}/cmake_modules/
)


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

### Subdirectories ########################


#ADD_SUBDIRECTORY(Modules)
ADD_SUBDIRECTORY(ScBridge)


MESSAGE(STATUS "\nShem done...")
