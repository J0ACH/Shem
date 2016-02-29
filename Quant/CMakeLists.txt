MESSAGE(STATUS "\nQuant init...")
MESSAGE(STATUS "SC_SRC_DIR : " ${SC_SRC_DIR})


FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE(ScBridge)

if(SCBRIDGE_FOUND)
  MESSAGE(STATUS "OK got SCBRIDGE!")
endif()

SET(QT_LIBRARIES 
  Qt5::Core
  Qt5::Gui 
  Qt5::Widgets
  )

INCLUDE_DIRECTORIES(
  ${SCBRIDGE_INCLUDE_DIR}
  )

SET(Quant_hdr Quant.h)

SET(Quant_src Quant.cpp)

ADD_EXECUTABLE(Quant WIN32 ${Quant_hdr} ${Quant_src})

TARGET_LINK_LIBRARIES(Quant
  ${QT_LIBRARIES}
    ${SCBRIDGE_LIBRARY}
  )

INSTALL(TARGETS Quant DESTINATION ${CMAKE_BINARY_DIR}/Install)

if(WIN32)	
  SET(QT_DLL_DIR ${Qt5Core_DIR}/../../../bin)

  SET(QT_LIBRARIES_RELEASE
    ${QT_DLL_DIR}/Qt5Core.dll
    ${QT_DLL_DIR}/Qt5Gui.dll
    ${QT_DLL_DIR}/Qt5Widgets.dll
    ${QT_DLL_DIR}/icuin54.dll
    ${QT_DLL_DIR}/icuuc54.dll
    ${QT_DLL_DIR}/icudt54.dll
    )

  IF(CMAKE_C_FLAGS_RELEASE)
    FILE(COPY ${QT_LIBRARIES_RELEASE} DESTINATION ${CMAKE_BINARY_DIR}/Install)
  ENDIF()
ENDIF()


MESSAGE(STATUS "\nScBridge done...")
