# ---------------------------------
# Finds FABIEN_LIB
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PATH(PATH_FABIEN_LIB include/Trial.h PATHS /local/yrenard/Open-ViBE/repository/libBCI)
IF(PATH_FABIEN_LIB)
	MESSAGE(STATUS "  Found FABIEN_LIB...")
	INCLUDE_DIRECTORIES(${PATH_FABIEN_LIB}/include)
	FIND_LIBRARY(LIB_FABIEN_LIB OpenViBE PATHS ${PATH_FABIEN_LIB}/lib )
	IF(LIB_FABIEN_LIB)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_FABIEN_LIB}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_FABIEN_LIB} )
	ELSE(LIB_FABIEN_LIB)
		MESSAGE(STATUS "    [FAILED] lib FABIEN_LIB")
	ENDIF(LIB_FABIEN_LIB)

	ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyFabien)
	ADD_DEFINITIONS(-Wno-deprecated)
ELSE(PATH_FABIEN_LIB)
	MESSAGE(STATUS "  FAILED to find FABIEN_LIB")
ENDIF(PATH_FABIEN_LIB)
