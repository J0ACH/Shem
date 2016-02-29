MESSAGE(STATUS "\nScBridge init...")
MESSAGE(STATUS "SC_SRC_DIR : " ${SC_SRC_DIR})

#FIND_PACKAGE(SC)

INCLUDE_DIRECTORIES(
    ${SC_SRC_DIR}/include/lang
    ${SC_SRC_DIR}/include/common
  )
#LINK_DIRECTORIES()

SET(ScBridge_hdr ScBridge.h)
SET(ScBridge_src ScBridge.cpp)

ADD_LIBRARY(ScBridge ${ScBridge_src} ${ScBridge_hdr})

#TARGET_LINK_LIBRARIES(ScBridge libsclang)

#INSTALL(TARGETS ScBridge DESTINATION  ${CMAKE_CURRENT_BINARY_DIR})


MESSAGE(STATUS "\nScBridge done...")
