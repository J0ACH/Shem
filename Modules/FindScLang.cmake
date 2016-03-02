########################################################################################
##	SC_LANG_FOUND - Found libsclang
##	SC_LANG_INCLUDE_DIR - Path to include directory at source folder of Supercollider
##	SC_LANG_LIBRARY_DIR - Path to lang library at binary folder of Supercollider
##	SC_LANG_LIBRARY - Library libsclang
########################################################################################

MESSAGE(STATUS "\nFindScLang init...")


MESSAGE(STATUS "SC_SOURCE_DIR : " ${SC_SOURCE_DIR})
MESSAGE(STATUS "SC_BUILD_DIR : " ${SC_BUILD_DIR})

SET(SC_LANG_FOUND FALSE)

FIND_PATH(
  SC_LANG_INCLUDE_DIR
  NAMES SC_LanguageClient.h
  PATHS ${SC_SOURCE_DIR}/include/lang
  )

FIND_PATH(
  SC_LANG_LIBRARY_DIR
  NAMES libsclang.lib libsclang.a
  PATHS ${SC_BUILD_DIR}/lang
  PATH_SUFFIXES Release
 )

FIND_LIBRARY(
  SC_LANG_LIBRARY
  NAMES libsclang sclang
  PATHS ${SC_BUILD_DIR}/lang
  PATH_SUFFIXES Release
)

IF( SC_LANG_INCLUDE_DIR AND SC_LANG_LIBRARY)
  SET(SC_LANG_FOUND TRUE)
  MESSAGE(STATUS "SC_LANG_INCLUDE_DIR : " ${SC_LANG_INCLUDE_DIR})
  MESSAGE(STATUS "SC_LANG_LIBRARY_DIR : " ${SC_LANG_LIBRARY_DIR})
  MESSAGE(STATUS "SC_LANG_LIBRARY : " ${SC_LANG_LIBRARY})
  
  mark_as_advanced(FORCE SC_LANG_INCLUDE_DIR)
  mark_as_advanced(FORCE SC_LANG_LIBRARY_DIR)
  #mark_as_advanced(FORCE SC_LANG_LIBRARY)
ENDIF()

MESSAGE(STATUS "FindScLang done...\n")
