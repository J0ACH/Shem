########################################################################################
##	SC_FOUND - Found Supercollider
##  SC_INSTALL_DIR - Install folder of Supercollider with sclang application
########################################################################################

SET(SC_FOUND FALSE)

FIND_PATH(
  SC_INSTALL_DIR
  NAMES "sclang.exe" "sclang"
  PATHS 
  "C:/Supercollider/Supercollider_findModules"
  "D:/Supercollider/Supercollider_3.7"
  "/home/kof/src/supercollider/build"
  PATH_SUFFIXES "Install/Supercollider"
)

IF(SC_INSTALL_DIR)
	SET(SC_FOUND TRUE)
  MESSAGE(STATUS "\nSupercollider install dir found {SC_INSTALL_DIR} - " ${SC_INSTALL_DIR})
	MARK_AS_ADVANCED(FORCE SC_INSTALL_DIR)
ELSE()
	MESSAGE(WARNING "Oh no, I coundn't find SuperCollider path with sclang app!")
ENDIF()


