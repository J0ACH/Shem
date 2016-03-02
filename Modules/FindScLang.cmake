########################################################################################
##	SCLANG_FOUND - Found libsclang
##	SCLANG_INCLUDE_DIR - Path to include directory at source folder of Supercollider
##	SCLANG_LIBRARY_DIR - Path to lang library at binary folder of Supercollider
##	SCLANG_LIBRARY - Library libsclang
########################################################################################

MESSAGE(STATUS "\nFindScLang init...")


MESSAGE(STATUS "SC_SOURCE_DIR : " ${SC_SOURCE_DIR})
MESSAGE(STATUS "SC_BUILD_DIR : " ${SC_BUILD_DIR})

FIND_PATH(
  SCLANG_INCLUDE_DIR
  NAMES SC_LanguageClient.h
  PATHS ${SC_SOURCE_DIR}/include/lang
  )

FIND_PATH(
  SCLANG_LIBRARY_DIR
  NAMES libsclang.lib libsclang.a
  PATHS ${SC_BUILD_DIR}/lang
  PATH_SUFFIXES "Release"
)

FIND_LIBRARY(
  SCLANG_LIBRARY
  NAMES libsclang
  PATHS ${SC_BUILD_DIR}/lang
  PATH_SUFFIXES "Release"
)

SET(SCLANG_FOUND FALSE)

IF( SCLANG_INCLUDE_DIR AND SCLANG_LIBRARY)
  SET(SCLANG_FOUND TRUE)
  MESSAGE(STATUS "SCLANG_INCLUDE_DIR : " ${SCLANG_INCLUDE_DIR})
  MESSAGE(STATUS "SCLANG_LIBRARY_DIR : " ${SCLANG_LIBRARY_DIR})
  MESSAGE(STATUS "SCLANG_LIBRARY : " ${SCLANG_LIBRARY})
  
  mark_as_advanced(FORCE SCLANG_INCLUDE_DIR)
  mark_as_advanced(FORCE SCLANG_LIBRARY_DIR)
  #mark_as_advanced(FORCE SCLANG_LIBRARY)
ENDIF()

MESSAGE(STATUS "FindScLang done...\n")
