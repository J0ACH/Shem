MESSAGE(STATUS "\nFIND SC init...")

FIND_PATH(
  SC_SOURCE_DIR
  NAMES SCVersion.txt
  PATHS 
  "/home/kof/src/supercollider"
  "C:/Supercollider/supercollider"
  ${SC_SRC_DIR}
  NO_DEFAULT_PATH
  )

SET(SC_FOUND FALSE)

IF(SC_FOUND)
  SET(SCBRIDGE_FOUND TRUE)
  message(status "Ok got supercollider source dir, and it is FOUND! : " + ${SC_SOURCE_DIR})

else()
  message(status "Oh no, I coundn't find SUpercollider juicy path!")

ENDIF()


MESSAGE(STATUS "SC FOUND done...\n")
