PROJECT (Shem)

### Project setup ########################

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

### Path setup ########################

MESSAGE(STATUS "\nShem init...")

SET(SC_SRC_DIR "/home/kof/src/supercollider" CACHE PATH "Supercollider source path")
SET(SC_BUILD_DIR "/home/kof/src/supercollider/build" CACHE PATH "Supercollider build path")


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

SET(CMAKE_MODULE_PATH 
        ${CMAKE_MODULE_PATH}	
        ${CMAKE_SOURCE_DIR}/Modules 
	${SC_SRC_DIR}/cmake_modules/
)


FILE(GLOB_RECURSE SHEM_MODULES ${CMAKE_SOURCE_DIR}/Modules/*.cmake )

find_package(Sc)

MESSAGE(STATUS "Modules found:")
FOREACH(ONEMODULE ${SHEM_MODULES})
	MESSAGE(STATUS "\t - " ${ONEMODULE})
    #add_test(NAME ${TEST}  COMMAND $<TARGET_FILE:${TEST}>)
    #set_tests_properties( ${TEST} PROPERTIES TIMEOUT 1) 
    #set_property(TARGET ${TEST} PROPERTY FOLDER "Modules")
	#source_group("Modules" FILES ${TEST})
ENDFOREACH(ONEMODULE)

### Subdirectories ########################


ADD_SUBDIRECTORY(ScBridge)
ADD_SUBDIRECTORY(Quant)



MESSAGE(STATUS "\nShem done...")
