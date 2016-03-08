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

SET(ScBridge_hdr ScBridge.h)
SET(ScBridge_src ScBridge.cpp)

ADD_LIBRARY(ScBridge STATIC ${ScBridge_src} ${ScBridge_hdr})

TARGET_LINK_LIBRARIES(ScBridge ${QT_LIBRARIES})




