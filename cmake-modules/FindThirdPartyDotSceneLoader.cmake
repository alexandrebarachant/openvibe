# ---------------------------------
# Finds DotSceneLoader toolkit
#
# Sets DotSceneLoader_FOUND
# Sets DotSceneLoader_LIBRARIES
# Sets DotSceneLoader_LIBRARY_DIRS
# Sets DotSceneLoader_LDFLAGS
# Sets DotSceneLoader_LDFLAGS_OTHERS
# Sets DotSceneLoader_INCLUDE_DIRS
# Sets DotSceneLoader_CFLAGS
# Sets DotSceneLoader_CFLAGS_OTHERS
#
# Adds library to target
# Adds include path
# ---------------------------------

IF(WIN32)
	FIND_PATH(PATH_DotSceneLoader include/dotSceneInterface.h PATHS $ENV{OV_DEP_DSI} $ENV{OMK_HOME} $ENV{OMK_BIN})
	IF(PATH_DotSceneLoader)
		SET(DotSceneLoader_FOUND TRUE)
		SET(DotSceneLoader_INCLUDE_DIRS ${PATH_DotSceneLoader}/include)
		IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
			SET(DotSceneLoader_LIBRARIES DotSceneInterface_d)
		ELSE(CMAKE_BUILD_TYPE STREQUAL "Debug")
			SET(DotSceneLoader_LIBRARIES DotSceneInterface)
		ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")
		SET(DotSceneLoader_LIBRARY_DIRS ${PATH_DotSceneLoader}/lib )
	ENDIF(PATH_DotSceneLoader)
ENDIF(WIN32)

IF(UNIX)
	FIND_PATH(PATH_DotSceneLoader include/dotSceneInterface.h PATHS $ENV{OMK_HOME} $ENV{OMK_BIN})
	IF(PATH_DotSceneLoader)
		SET(DotSceneLoader_FOUND TRUE)
		SET(DotSceneLoader_INCLUDE_DIRS ${PATH_DotSceneLoader}/include)
		SET(DotSceneLoader_LIBRARIES DotSceneInterface)
		SET(DotSceneLoader_LIBRARY_DIRS ${PATH_DotSceneLoader}/lib )
	ENDIF(PATH_DotSceneLoader)
ENDIF(UNIX)

IF(DotSceneLoader_FOUND)
	MESSAGE(STATUS "  Found Dot scene loader...")
	INCLUDE_DIRECTORIES(${DotSceneLoader_INCLUDE_DIRS})
	ADD_DEFINITIONS(${DotSceneLoader_CFLAGS})
	ADD_DEFINITIONS(${DotSceneLoader_CFLAGS_OTHERS})
	LINK_DIRECTORIES(${DotSceneLoader_LIBRARY_DIRS})
	FOREACH(DotSceneLoader_LIB ${DotSceneLoader_LIBRARIES})
		SET(DotSceneLoader_LIB1 "DotSceneLoader_LIB1-NOTFOUND")
		FIND_LIBRARY(DotSceneLoader_LIB1 NAMES ${DotSceneLoader_LIB} PATHS ${DotSceneLoader_LIBRARY_DIRS} ${DotSceneLoader_LIBDIR} NO_DEFAULT_PATH)
		FIND_LIBRARY(DotSceneLoader_LIB1 NAMES ${DotSceneLoader_LIB})
		IF(DotSceneLoader_LIB1)
			MESSAGE(STATUS "    [  OK  ] Third party lib ${DotSceneLoader_LIB1}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${DotSceneLoader_LIB1})
		ELSE(DotSceneLoader_LIB1)
			MESSAGE(STATUS "    [FAILED] Third party lib ${DotSceneLoader_LIB}")
		ENDIF(DotSceneLoader_LIB1)
	ENDFOREACH(DotSceneLoader_LIB)

	ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyDotSceneLoader)
ELSE(DotSceneLoader_FOUND)
	MESSAGE(STATUS "  FAILED to find Dot scene loader...")
ENDIF(DotSceneLoader_FOUND)
