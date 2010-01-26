# ---------------------------------
# Finds OgreCEGUIRenderer toolkit
#
# Sets OgreCEGUIRenderer_FOUND
# Sets OgreCEGUIRenderer_LIBRARIES
# Sets OgreCEGUIRenderer_LIBRARY_DIRS
# Sets OgreCEGUIRenderer_LDFLAGS
# Sets OgreCEGUIRenderer_LDFLAGS_OTHERS
# Sets OgreCEGUIRenderer_INCLUDE_DIRS
# Sets OgreCEGUIRenderer_CFLAGS
# Sets OgreCEGUIRenderer_CFLAGS_OTHERS
# ---------------------------------

INCLUDE("FindThirdPartyPkgConfig")
pkg_check_modules(OgreCEGUIRenderer CEGUI-OGRE)

IF(OgreCEGUIRenderer_FOUND)
	MESSAGE(STATUS "  Found Ogre CEGUI Renderer...")
	INCLUDE_DIRECTORIES(${OgreCEGUIRenderer_INCLUDE_DIRS})
	ADD_DEFINITIONS(${OgreCEGUIRenderer_CFLAGS})
	ADD_DEFINITIONS(${OgreCEGUIRenderer_CFLAGS_OTHERS})
	LINK_DIRECTORIES(${OgreCEGUIRenderer_LIBRARY_DIRS})
	FOREACH(OgreCEGUIRenderer_LIB ${OgreCEGUIRenderer_LIBRARIES})
		SET(OgreCEGUIRenderer_LIB1 "OgreCEGUIRenderer_LIB1-NOTFOUND")
		FIND_LIBRARY(OgreCEGUIRenderer_LIB1 NAMES ${OgreCEGUIRenderer_LIB} PATHS ${OgreCEGUIRenderer_LIBRARY_DIRS} ${OgreCEGUIRenderer_LIBDIR} NO_DEFAULT_PATH)
		FIND_LIBRARY(OgreCEGUIRenderer_LIB1 NAMES ${OgreCEGUIRenderer_LIB})
		IF(OgreCEGUIRenderer_LIB1)
			MESSAGE(STATUS "    [  OK  ] Third party lib ${OgreCEGUIRenderer_LIB1}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${OgreCEGUIRenderer_LIB1})
		ELSE(OgreCEGUIRenderer_LIB1)
			MESSAGE(STATUS "    [FAILED] Third party lib ${OgreCEGUIRenderer_LIB}")
		ENDIF(OgreCEGUIRenderer_LIB1)
	ENDFOREACH(OgreCEGUIRenderer_LIB)
ELSE(OgreCEGUIRenderer_FOUND)
	MESSAGE(STATUS "  FAILED to find Ogre CEGUI Renderer...")
ENDIF(OgreCEGUIRenderer_FOUND)
