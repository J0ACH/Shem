########################################################################################
##	YAML_FOUND - Found libsclang
##	YAML_INCLUDE_DIR - Path to include directory at source folder of Supercollider
##	YAML_LIBRARY_DIR - Path to lang library at binary folder of Supercollider
##	YAML_LIBRARY - Library libsclang
########################################################################################

MESSAGE(STATUS "\nFindYAML init...")


#MESSAGE(STATUS "SC_SOURCE_DIR : " ${SC_SOURCE_DIR})
#MESSAGE(STATUS "SC_BUILD_DIR : " ${SC_BUILD_DIR})

FIND_PATH(
  YAML_INCLUDE_DIR
  NAMES yaml.h
  PATHS ${SC_SOURCE_DIR}/external_libraries/yaml-cpp-0.3.0/include/yaml-cpp
)

FIND_LIBRARY(
  YAML_LIBRARY
  NAMES YAML
  PATHS ${SC_BUILD_DIR}/external_libraries
  PATH_SUFFIXES "Release"
)

SET(YAML_FOUND FALSE)

IF(YAML_INCLUDE_DIR AND YAML_LIBRARY)
  SET(YAML_FOUND TRUE)
  MESSAGE(STATUS "YAML_INCLUDE_DIR : " ${YAML_INCLUDE_DIR})
  MESSAGE(STATUS "YAML_LIBRARY_DIR : " ${YAML_LIBRARY_DIR})
  MESSAGE(STATUS "YAML_LIBRARY : " ${YAML_LIBRARY})
  
  #mark_as_advanced(FORCE YAML_INCLUDE_DIR)
  #mark_as_advanced(FORCE YAML_LIBRARY_DIR)
  #mark_as_advanced(FORCE YAML_LIBRARY)
ENDIF()

MESSAGE(STATUS "FindYAML done...\n")
