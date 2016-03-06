########################################################################################
##	BOOST_FOUND - Found libsclang
##	BOOST_LIBRARY - Library libsclang
########################################################################################

MESSAGE(STATUS "\nFindBOOST init...")


#MESSAGE(STATUS "SC_SOURCE_DIR : " ${SC_SOURCE_DIR})
#MESSAGE(STATUS "SC_BUILD_DIR : " ${SC_BUILD_DIR})

FIND_LIBRARY(
  BOOST_LIBRARY
  NAMES boost_system
  PATHS ${SC_BUILD_DIR}/external_libraries
  PATH_SUFFIXES "Release"
)

SET(BOOST_FOUND FALSE)

IF(BOOST_INCLUDE_DIR AND BOOST_LIBRARY)
  SET(BOOST_FOUND TRUE)
  MESSAGE(STATUS "BOOST_LIBRARY : " ${BOOST_LIBRARY})
  
  #mark_as_advanced(FORCE BOOST_LIBRARY)
ENDIF()

MESSAGE(STATUS "FindBOOST done...\n")
