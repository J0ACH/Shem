message(STATUS "Jui init...")

#OPTION(JUI_CANVAN_SYSTEMFRAME "Canvan system frame" OFF)
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

SET(Jui_hdr
	Canvan.h
	Button.h
	Console.h
	Panel.h
	Edges.h
	EdgeControler.h
	CodeEditor.h
	Graph.h
	ControlBox.h
)

SET(Jui_src
	Canvan.cpp
	Button.cpp
	Console.cpp
	Panel.cpp
	Edges.cpp
	EdgeControler.cpp
	CodeEditor.cpp
	Graph.cpp
	ControlBox.cpp
)

QT5_WRAP_CPP( Jui_moc ${Jui_hdr} )

ADD_LIBRARY(Jui STATIC ${Jui_src} ${Jui_hdr} ${Jui_moc})

TARGET_INCLUDE_DIRECTORIES(Jui PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
TARGET_LINK_LIBRARIES(Jui ${QT_JUI_LIBRARIES})







