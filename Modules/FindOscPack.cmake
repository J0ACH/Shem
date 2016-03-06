########################################################################################
##	OSCPACK_FOUND - Found libsclang
##	OSCPACK_INCLUDE_DIR - Path to include directory at source folder of Supercollider
##	OSCPACK_LIBRARY_DIR - Path to lang library at binary folder of Supercollider
##	OSCPACK_LIBRARY - Library libsclang
########################################################################################

MESSAGE(STATUS "\nFindOscPack init...")


#MESSAGE(STATUS "SC_SOURCE_DIR : " ${SC_SOURCE_DIR})
#MESSAGE(STATUS "SC_BUILD_DIR : " ${SC_BUILD_DIR})

FIND_PATH(
  OSCPACK_INCLUDE_DIR
  NAMES OscReceivedElements.h
  PATHS ${SC_SOURCE_DIR}/external_libraries/oscpack_1_1_0/osc
)

FIND_PATH(
  OSCPACK_SRC_DIR
  NAMES make.MinGW32.bat
  PATHS ${SC_SOURCE_DIR}/external_libraries/oscpack_1_1_0
)

FIND_LIBRARY(
  OSCPACK_LIBRARY
  NAMES oscpack
  PATHS ${SC_BUILD_DIR}/external_libraries
  PATH_SUFFIXES "Release"
)

SET(OSCPACK_FOUND FALSE)

IF(OSCPACK_INCLUDE_DIR AND OSCPACK_LIBRARY)
  SET(OSCPACK_FOUND TRUE)
  MESSAGE(STATUS "OSCPACK_INCLUDE_DIR : " ${OSCPACK_INCLUDE_DIR})
  MESSAGE(STATUS "OSCPACK_LIBRARY_DIR : " ${OSCPACK_LIBRARY_DIR})
  MESSAGE(STATUS "OSCPACK_LIBRARY : " ${OSCPACK_LIBRARY})
  
  #mark_as_advanced(FORCE OSCPACK_INCLUDE_DIR)
  #mark_as_advanced(FORCE OSCPACK_LIBRARY_DIR)
  #mark_as_advanced(FORCE OSCPACK_LIBRARY)
ENDIF()

MESSAGE(STATUS "FindOscPack done...\n")
