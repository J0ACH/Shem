PROJECT (Quant)

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

# INCLUDE #######################################################
INCLUDE("QuantVersion.txt" ${QT_USE_FILE})
SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}${PROJECT_VERSION_PATCH}")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Jui)

MESSAGE(STATUS "\nQuant [${PROJECT_VERSION}] init...")

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

SET(Quant_hdr Quant.h)
SET(Quant_src Quant.cpp)

ADD_EXECUTABLE(Quant ${Quant_hdr} ${Quant_src})
IF(WIN32) 
	SET_TARGET_PROPERTIES( Quant PROPERTIES WIN32_EXECUTABLE TRUE )
ENDIF()

# LINKER #######################################################

TARGET_LINK_LIBRARIES(Quant
	${QT_LIBRARIES}
	ScBridge
	Jui
)

# INSTALL ########################################################

INSTALL(TARGETS Quant DESTINATION ${SC_INSTALL_DIR})


