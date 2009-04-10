# - This module looks for AStyle
# AStyle is a source code formatting tool see http://astyle.sourceforge.net
# This code sets the following variables:
#  ASTYLE_EXECUTABLE     = The path to the astyle command.
#  ASTYLE_FOUND		 = Whether the astyle tool has been found

MESSAGE(STATUS "Looking for astyle...")

FIND_PROGRAM(ASTYLE_EXECUTABLE
  NAMES astyle
#  PATHS "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\doxygen_is1;Inno Setup: App Path]/bin"
#  /Applications/Doxygen.app/Contents/Resources
#  /Applications/Doxygen.app/Contents/MacOS
  DOC "AStyle Source Code Formatting tool (http://astyle.sourceforge.net)"
)

IF (ASTYLE_EXECUTABLE)
  SET (ASTYLE_FOUND "YES")
  MESSAGE(STATUS "Looking for astyle... - found ${ASTYLE_EXECUTABLE}")
ELSE (ASTYLE_EXECUTABLE)
  IF (ASTYLE_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Looking for astyle... - NOT found")
  ELSE (ASTYLE_FIND_REQUIRED)
    MESSAGE(STATUS "Looking for astyle... - NOT found")
  ENDIF (ASTYLE_FIND_REQUIRED)
ENDIF (ASTYLE_EXECUTABLE)

MARK_AS_ADVANCED(
  ASTYLE_FOUND
  ASTYLE_EXECUTABLE
  )

