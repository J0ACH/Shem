MESSAGE(STATUS "\nFIND SC init...")

SET(SC_FOUND FALSE)

FIND_PATH(
  SC_SOURCE_DIR
  NAMES SCVersion.txt
  PATHS 
  "/home/kof/src/supercollider"
  "C:/Supercollider/supercollider"
  ${SC_SRC_DIR}
  NO_DEFAULT_PATH
  )

IF(SC_SOURCE_DIR)
  SET(SC_FOUND TRUE)
  message(status "Ok got supercollider source dir, and it is FOUND! : " + ${SC_SOURCE_DIR})

else()
  message(status "Oh no, I coundn't find SuperCollider source path!")
  mark_as_advanced(CLEAR SC_SRC_DIR)
  mark_as_advanced(CLEAR SC_BUILD_DIR)
ENDIF()


MESSAGE(STATUS "SC FOUND done...\n")
