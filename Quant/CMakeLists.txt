PROJECT (Quant)

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

# INCLUDE #######################################################
INCLUDE("QuantVersion.txt" ${QT_USE_FILE})
SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}${PROJECT_VERSION_PATCH}")
MESSAGE(STATUS "\nQuant init...")

# git version detecton ################################################ 
include(GetGitRevisionDescription)
git_describe(GIT_REVISION_DESCRIPTION --tags --match v*)
if(GIT_REVISION_DESCRIPTION)
  string(REPLACE "v" "" GIT_REVISION_DESCRIPTION "${GIT_REVISION_DESCRIPTION}")
  string(REGEX REPLACE "\-(.*)" "" GIT_REVISION_DESCRIPTION "${GIT_REVISION_DESCRIPTION}")
  set(PROJECT_VERSION "${GIT_REVISION_DESCRIPTION}")

  # to by bylo, verze je na svete ale nevim jak z toho dostat ty tri udaje... hm

  message(STATUS "Git reports version ${PROJECT_VERSION}")
else()
  message(STATUS "Could not get revision information from Git! Calling this just version ${PROJECT_VERSION}!")
endif()

CONFIGURE_FILE (
  ${CMAKE_CURRENT_SOURCE_DIR}/QuantConfig.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/QuantConfig.h
  )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Jui)

# FIND QT #############################################

FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Gui)

SET(QT_LIBRARIES 
  Qt5::Core
  Qt5::Gui 
  Qt5::Widgets
  )

ADD_SUBDIRECTORY(Jui)  

# SOURCES ##################################################

SET(Quant_hdr 
  Quant.h
  QuantCore.h
  QuantObjects.h
  NodePanel.h
  Node.h
  ControlEnvelope.h
)

SET(Quant_src 
  Quant.cpp
  QuantCore.cpp
  QuantObjects.cpp
  NodePanel.cpp
  Node.cpp
  ControlEnvelope.cpp
)

SET(Quant_res 
  Quant.qrc
  Jui/Jui.qrc
  )

QT5_WRAP_CPP( Quant_moc ${Quant_hdr} )
QT5_ADD_RESOURCES( Quant_rcc ${Quant_res} )
SET(Quant_QT_Sources ${Quant_moc} ${Quant_rcc})

# APPLICATION ICON ##################################################

if(WIN32)
  SET(Quant_ico ${CMAKE_CURRENT_SOURCE_DIR}/Icon/QuantIcon.rc)
  SET(CMAKE_RC_COMPILER_INIT windres)
  ENABLE_LANGUAGE(RC)
  SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(WIN32)

# EXE ##################################################

ADD_EXECUTABLE(Quant ${Quant_src} ${Quant_hdr} ${Quant_QT_Sources} ${Quant_ico})
IF(WIN32) 
  SET_TARGET_PROPERTIES( Quant PROPERTIES WIN32_EXECUTABLE TRUE )
ENDIF()

# LINKER #######################################################

TARGET_INCLUDE_DIRECTORIES(Quant PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

TARGET_LINK_LIBRARIES(Quant
  ${QT_LIBRARIES}
  ScBridge
  Jui
  )

# INSTALL ########################################################

INSTALL(TARGETS Quant DESTINATION ${SC_INSTALL_DIR})




