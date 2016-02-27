MESSAGE(STATUS "\nFindSC init...")
MESSAGE(STATUS "SC_SRC_DIR : " ${SC_SRC_DIR})

FIND_PATH(
    LANG_INCLUDE_DIR
    NAMES SC_LanguageClient.h
    PATHS ${SC_SRC_DIR}/include/lang
)
MESSAGE(STATUS "LANG_INCLUDE_DIR : " ${LANG_INCLUDE_DIR})

FIND_LIBRARY(
    LANG_LINK_DIR
    NAMES sclang libsclang.a libsclang.dll libsclang.dynlib
    HINTS ${SC_BUILD_DIR}/lang
    PATHS ${SC_BUILD_DIR}/lang
    PATH_SUFFIXES Release
)

MESSAGE(STATUS "LANG_LINK_DIR : " ${LANG_LINK_DIR})

MESSAGE(STATUS "FindSC done...")
