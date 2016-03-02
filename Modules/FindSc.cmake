MESSAGE(STATUS "\nFIND SC init...")

########################################################################################
##	SC_SOURCE_FOUND - Found source folder of Supercollider 
##	SC_SOURCE_DIR - Path to binary folder of Supercollider
##
##	SC_BUILD_FOUND - Found binary folder of Supercollider
##	SC_BUILD_DIR - Found binary folder of Supercollider
########################################################################################


## Supercollider_source #######################################

SET(SC_SOURCE_FOUND FALSE)

FIND_PATH(
  SC_SOURCE_DIR
  NAMES SCVersion.txt
  PATHS 
  "/home/kof/src/supercollider"
  "C:/Supercollider/supercollider"
  "D:/Supercollider/supercollider"
  NO_DEFAULT_PATH
)

IF(SC_SOURCE_DIR)
	SET(SC_SOURCE_FOUND TRUE)
	
	message(STATUS "\t{SC_SOURCE_DIR} - Supercollider source dir found: " ${SC_SOURCE_DIR})
	
	mark_as_advanced(FORCE SC_SRC_DIR)
	#mark_as_advanced(FORCE SC_BUILD_DIR)
else()
	SET(SC_SOURCE_DIR "Kdepak mas SC_source?")
	message(STATUS "Oh no, I coundn't find SuperCollider source path!")
ENDIF()

## Supercollider_build #######################################

SET(SC_BUILD_FOUND FALSE)

FIND_PATH(
  SC_BUILD_DIR
  NAMES SuperCollider.sln build_sclang.cfg
  PATHS 
  "C:/Supercollider/Supercollider_findModules"
  "D:/Supercollider/Supercollider_3.7"
  "/home/kof/src/supercollider/build"
  ONLY_CMAKE_FIND_ROOT_PATH
)

IF(SC_BUILD_DIR)
	SET(SC_BUILD_FOUND TRUE)
	
	message(STATUS "\t{SC_BUILD_DIR} - Supercollider source dir found: " ${SC_BUILD_DIR})
		
	#mark_as_advanced(FORCE SC_SRC_DIR)
	#mark_as_advanced(FORCE SC_BUILD_DIR)
else()
SET(SC_BUILD_DIR "Kdepak mas SC_build?")
	message(STATUS "Oh no, I coundn't find SuperCollider source path!")
ENDIF()


MESSAGE(STATUS "SC FOUND done...\n")
