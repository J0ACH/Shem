message(STATUS "Jui init...")

OPTION(JUI_CANVAN_SYSTEMFRAME "Canvan system frame" OFF)
CONFIGURE_FILE (
  ${CMAKE_CURRENT_SOURCE_DIR}/JuiConfig.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/JuiConfig.h
)

FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Gui)

SET(QT_JUI_LIBRARIES 
	Qt5::Core
	Qt5::Gui 
	Qt5::Widgets
)

SET(Jui_src
	Canvan.cpp
	Button.cpp
	Console.cpp
)

SET(Jui_hdr
	Canvan.h
	Button.h
	Console.h
)

SET(Jui_res
	
)

ADD_LIBRARY(Jui STATIC
	${Jui_src}
	${Jui_hdr}
	${Jui_res}	
)

TARGET_INCLUDE_DIRECTORIES(Jui PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
TARGET_LINK_LIBRARIES(Jui ${QT_JUI_LIBRARIES})







