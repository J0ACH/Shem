PROJECT (Quant)

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

MESSAGE(STATUS "\nQuant init...")

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

SET(Quant_hdr Quant.h)
SET(Quant_src Quant.cpp)

ADD_EXECUTABLE(Quant ${Quant_hdr} ${Quant_src})
IF(WIN32) 
	set_target_properties( Quant PROPERTIES WIN32_EXECUTABLE TRUE )
ENDIF()

# LINKER #######################################################

TARGET_LINK_LIBRARIES(Quant
	${QT_LIBRARIES}
	ScBridge
)

# INSTALL ########################################################

INSTALL(TARGETS Quant DESTINATION ${SC_INSTALL_DIR})


