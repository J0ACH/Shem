PROJECT (Quant)

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

MESSAGE(STATUS "\nQuant init...")

MESSAGE(STATUS "ScBridge include folders:")
GET_TARGET_PROPERTY(BRIDGE_INCLUDE_DIRS ScBridge SOURCES)
FOREACH(ONE_BRGINCLUDE ${BRIDGE_INCLUDE_DIRS})	
		MESSAGE(STATUS "\t - " ${ONE_BRGINCLUDE})
ENDFOREACH(ONE_BRGINCLUDE)

MESSAGE(STATUS "\nScBridge linked libraies:")
GET_TARGET_PROPERTY(BRIDGE_LINK_LIBRARIES ScBridge LINK_LIBRARIES)
FOREACH(ONE_BRGLINK ${BRIDGE_LINK_LIBRARIES})	
		MESSAGE(STATUS "\t - " ${ONE_BRGLINK})
ENDFOREACH(ONE_BRGLINK)

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

if(UNIX)
 ADD_EXECUTABLE(Quant ${Quant_hdr} ${Quant_src})
else()
ADD_EXECUTABLE(Quant ${Quant_hdr} ${Quant_src}) 
#ADD_EXECUTABLE(Quant WIN32 ${Quant_hdr} ${Quant_src})
endif()

#ADD_DEPENDENCIES(Quant ScBridge)

# LINKER #######################################################

TARGET_INCLUDE_DIRECTORIES(Quant PUBLIC ${CMAKE_SOURCE_DIR}/ScBridge)

TARGET_LINK_LIBRARIES(Quant
	${QT_LIBRARIES}
	ScBridge
)

# INSTALL ########################################################

INSTALL(TARGETS Quant DESTINATION  ${SC_BUILD_DIR}/Install/Supercollider)

MESSAGE(STATUS "\nScBridge done...")
