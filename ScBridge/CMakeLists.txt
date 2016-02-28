MESSAGE(STATUS "\nScBridge init...")
MESSAGE(STATUS "SC_SRC_DIR : " ${SC_SRC_DIR})

SET (CMAKE_PREFIX_PATH ${SC_SRC_DIR})
MESSAGE(STATUS "CMAKE_PREFIX_PATH : " ${CMAKE_PREFIX_PATH})

FIND_PACKAGE(SC)

IF(SC_FOUND)
	MESSAGE(STATUS "SCmodule : " ${SC_FOUND})
        include_directories(
          ${SC_INCLUDE_DIR}
          ${SC_INCLUDE_DIRS})
ELSE()
	MESSAGE(STATUS "SCmodule - nenalezen")
ENDIF()


SET(ScBridge_src ScBridge.cpp)

ADD_LIBRARY(ScBridge STATIC ${ScBridge_src})


IF(SC_FOUND)
    MESSAGE(STATUS "Linking Scalng..")
    TARGET_LINK_LIBRARIES(ScBridge ${SC_LIBRARY})
ENDIF()

MESSAGE(STATUS "\nScBridge done...")
