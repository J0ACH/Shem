PROJECT (ScBridge)

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

MESSAGE(STATUS "\nScBridge init...")

FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE(Qt5Network)

SET(QT_LIBRARIES 
	Qt5::Core
	Qt5::Gui 
	Qt5::Widgets
	Qt5::Network
)

SET(ScBridge_hdr 
  ScBridge.h
  Customize.h
  UDPServer.h
  Data.h
)
SET(ScBridge_src 
  ScBridge.cpp
  Customize.cpp
  UDPServer.cpp
  Data.cpp
)

QT5_WRAP_CPP( ScBridge_moc ${ScBridge_hdr} )

ADD_LIBRARY(ScBridge STATIC ${ScBridge_src} ${ScBridge_hdr} ${ScBridge_moc})

TARGET_LINK_LIBRARIES(ScBridge ${QT_LIBRARIES})




