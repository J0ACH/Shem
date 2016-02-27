MESSAGE(STATUS "\nFindSC init...")
MESSAGE(STATUS "SC_SRC_DIR : " ${SC_SRC_DIR})

FIND_PATH(
    TEST_INCLUDE_DIR
    NAMES ScBridge.h
    #HINTS ${CMAKE_SOURCE_DIR}
    PATHS ${CMAKE_SOURCE_DIR}/ScBridge
    
)
MESSAGE(STATUS "TEST_INCLUDE_DIR : " ${TEST_INCLUDE_DIR})

FIND_PATH(
    LANG_INCLUDE_DIR
    NAMES SC_LanguageClient.hpp
    PATHS ${SC_SRC_DIR}/lang/LangSource
)
MESSAGE(STATUS "LANG_INCLUDE_DIR : " ${LANG_INCLUDE_DIR})

FIND_LIBRARY(
    LANG_LINK_DIR
    NAMES sclang
    HINTS ${SC_BUILD_DIR}/lang/Release/
    PATHS ${SC_BUILD_DIR}/lang/Release/
)

MESSAGE(STATUS "LANG_LINK_DIR : " ${LANG_LINK_DIR})

MESSAGE(STATUS "FindSC done...")