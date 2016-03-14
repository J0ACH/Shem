PROJECT (Editor)

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

MESSAGE(STATUS "\nEditor init...")

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

SET(Editor_hdr Editor.h)
SET(Editor_src 
  main.cpp
  Editor.cpp
)
QT5_WRAP_CPP( Editor_moc ${Editor_hdr} )

ADD_EXECUTABLE(Editor ${Editor_hdr} ${Editor_src} ${Editor_moc})
IF(WIN32) 
	set_target_properties( Editor PROPERTIES WIN32_EXECUTABLE TRUE )
ENDIF()

# LINKER #######################################################

TARGET_LINK_LIBRARIES(Editor
	${QT_LIBRARIES}
	ScBridge
)

# INSTALL ########################################################

INSTALL(TARGETS Editor DESTINATION ${SC_INSTALL_DIR})


