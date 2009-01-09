# ---------------------------------
# Finds Matlab toolkit
# ---------------------------------

IF(WIN32)
	MESSAGE(STATUS "  FindMatlab not yet implemented")
ENDIF(WIN32)

IF(UNIX)
	SET(Matlab_EXECUTABLE "Matlab_EXECUTABLE-NOTFOUND")
	FIND_PROGRAM(Matlab_EXECUTABLE matlab PATHS $ENV{MATLABROOT})
	FIND_PROGRAM(Matlab_EXECUTABLE MATLAB PATHS $ENV{MATLABROOT})
	IF(Matlab_EXECUTABLE)
		GET_FILENAME_COMPONENT(Matlab_ROOT ${Matlab_EXECUTABLE} PATH)
		SET(Matlab_ROOT ${Matlab_ROOT}/..)
		FIND_PATH(Matlab_INCLUDE "mex.h" ${Matlab_ROOT}/extern/include)
		SET(Matlab_LIBRARIES mex mx eng)
		IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
			SET(Matlab_LIB_DIRECTORIES ${Matlab_ROOT}/bin/glnx86)
		ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)
			SET(Matlab_LIB_DIRECTORIES ${Matlab_ROOT}/bin/glnxa64)
		ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
		SET(Matlab_FOUND TRUE)
	ENDIF(Matlab_EXECUTABLE)
ENDIF(UNIX)

IF(Matlab_FOUND)
	MESSAGE(STATUS "  Found Matlab...")

	INCLUDE_DIRECTORIES(${Matlab_INCLUDE})

	FOREACH(Matlab_LIB ${Matlab_LIBRARIES})
		SET(Matlab_LIB1 "Matlab_LIB1-NOTFOUND")
		FIND_LIBRARY(Matlab_LIB1 NAMES ${Matlab_LIB} PATHS ${Matlab_LIB_DIRECTORIES} NO_DEFAULT_PATH)
		FIND_LIBRARY(Matlab_LIB1 NAMES ${Matlab_LIB})
		IF(Matlab_LIB1)
			MESSAGE(STATUS "    [  OK  ] Third party lib ${Matlab_LIB1}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${Matlab_LIB1})
		ELSE(Matlab_LIB1)
			MESSAGE(STATUS "    [FAILED] Third party lib ${Matlab_LIB}")
		ENDIF(Matlab_LIB1)
	ENDFOREACH(Matlab_LIB)

	ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyMatlab)
ELSE(Matlab_FOUND)
	MESSAGE(STATUS "  FAILED to find Matlab...")
ENDIF(Matlab_FOUND)
