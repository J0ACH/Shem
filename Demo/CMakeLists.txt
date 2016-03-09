PROJECT (Demo)

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

MESSAGE(STATUS "\nDemo init...")

# FIND QT #############################################

FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Gui)

SET(QT_LIBRARIES 
  Qt5::Core
  Qt5::Gui 
  Qt5::Widgets
  )

# SOURCES ##################################################

SET(Demo_hdr Demo.h)
SET(Demo_src Demo.cpp)

ADD_EXECUTABLE(Demo ${Demo_hdr} ${Demo_src})
IF(WIN32) 
	set_target_properties( Demo PROPERTIES WIN32_EXECUTABLE TRUE )
ENDIF()

# LINKER #######################################################

TARGET_LINK_LIBRARIES(Demo
	${QT_LIBRARIES}
	ScBridge
)

# INSTALL ########################################################

INSTALL(TARGETS Demo DESTINATION ${SC_INSTALL_DIR})


