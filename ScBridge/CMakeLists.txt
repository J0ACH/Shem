PROJECT (ScBridge)

CMAKE_MINIMUM_REQUIRED ( VERSION 3.4 )

MESSAGE(STATUS "\nScBridge init...")
MESSAGE(STATUS "SC_SOURCE_DIR : " ${SC_SOURCE_DIR})
MESSAGE(STATUS "SC_BUILD_DIR : " ${SC_BUILD_DIR})

FIND_PACKAGE(ScLang)



MESSAGE(STATUS "ScLang_FOUND ${SCLANG_LIBRARY}: " ${SCLANG_FOUND})

ADD_LIBRARY(mylang STATIC IMPORTED)
SET_TARGET_PROPERTIES(mylang PROPERTIES IMPORTED_LOCATION "${SCLANG_LIBRARY}")
#SET_TARGET_PROPERTIES(mylang PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SCLANG_INCLUDE_DIR}")

GET_TARGET_PROPERTY(msg mylang LINK_DEPENDS)
MESSAGE(STATUS "msg : " ${msg})

#GET_TARGET_PROPERTY(Lang_INCLUDE_DIRS ScLang INCLUDE_DIRECTORIES)
#MESSAGE(STATUS "ScLang_INCLUDE_DIRS : " ${Lang_INCLUDE_DIRS})

#GET_TARGET_PROPERTY(ScLang_LINK_LIBRARIES sclang IMPORTED_LINK_DEPENDENT_LIBRARIES) 
#MESSAGE(STATUS "ScLang_LINK_LIBRARIES : " ${ScLang_LINK_LIBRARIES})

#GET_TARGET_PROPERTY(ScLang_SOURCES sclang INTERFACE_SOURCES)
#MESSAGE(STATUS "ScLang_SOURCES : " ${ScLang_SOURCES})

SET_DIRECTORY_PROPERTIES(PROPERTIES LINK_DIRECTORIES ${SCLANG_LIBRARY_DIR})

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	
    ${SC_BUILD_DIR}/common
	${SC_SOURCE_DIR}/include/common
	${SC_SOURCE_DIR}/include/lang
	${SC_SOURCE_DIR}/include/plugin_interface
	${SC_SOURCE_DIR}/include/server
	${SC_SOURCE_DIR}/common
	${SC_SOURCE_DIR}/external_libraries/yaml-cpp-0.3.0/include
	${SC_SOURCE_DIR}/external_libraries/threadpool
	${SC_SOURCE_DIR}/external_libraries/boost_sync/include
	${SC_SOURCE_DIR}/lang/LangSource
	${SC_SOURCE_DIR}/lang/LangPrimSource
	${SC_SOURCE_DIR}/external_libraries/hidapi/hidapi
	${SC_SOURCE_DIR}/external_libraries/hidapi/hidapi_parser
	${SC_SOURCE_DIR}/external_libraries/nova-tt
	${SC_SOURCE_DIR}/lang/LangSource/Bison
	${SC_SOURCE_DIR}/lang/../platform/windows/compat_stuff
	${SC_SOURCE_DIR}/lang/../platform/windows/compat_stuff/getopt
	${SC_SOURCE_DIR}/QtCollider
	${SC_SOURCE_DIR}/SCDoc
	${SC_SOURCE_DIR}/external_libraries/boost
####D:\Supercollider\x64\libsndfile\include
	${SC_SOURCE_DIR}/external_libraries/TLSF-2.4.6/src

####" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtCore" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\.\mkspecs\win32-msvc2013" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtGui" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtWidgets" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtNetwork" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtWebKit" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtWebKitWidgets" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtPrintSupport" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtOpenGL" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtSensors" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtQuick" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtQml" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtSql" /I"C:\Qt\Qt5.5\5.5\msvc2013_64\include\QtPositioning" 
	
	${SC_SOURCE_DIR}/external_libraries/portmidi/pm_common
	${SC_SOURCE_DIR}/external_libraries/portmidi/porttime
)

SET(ScBridge_hdr ScBridge.h)
SET(ScBridge_src ScBridge.cpp)

ADD_LIBRARY(ScBridge ${ScBridge_src} ${ScBridge_hdr})

TARGET_LINK_LIBRARIES(ScBridge 
	${SCLANG_LIBRARY}
)

MESSAGE(STATUS "ScBridge include folders:")
GET_TARGET_PROPERTY(BRIDGE_INCLUDE_DIRS ScBridge INCLUDE_DIRECTORIES)
FOREACH(ONE_BRGINCLUDE ${BRIDGE_INCLUDE_DIRS})	
		MESSAGE(STATUS "\t - " ${ONE_BRGINCLUDE})
ENDFOREACH(ONE_BRGINCLUDE)

MESSAGE(STATUS "\nScBridge linked libraies:")
GET_TARGET_PROPERTY(BRIDGE_LINK_LIBRARIES ScBridge LINK_LIBRARIES)
FOREACH(ONE_BRGLINK ${BRIDGE_LINK_LIBRARIES})	
		MESSAGE(STATUS "\t - " ${ONE_BRGLINK})
ENDFOREACH(ONE_BRGLINK)

#INSTALL(TARGETS ScBridge DESTINATION  ${CMAKE_CURRENT_BINARY_DIR})


MESSAGE(STATUS "\nScBridge done...")
