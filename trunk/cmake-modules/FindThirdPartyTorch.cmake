# ---------------------------------
# Finds TORCH
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PATH(PATH_TORCH include/general.h PATHS ${OV_CUSTOM_DEPENDENCIES_PATH})
IF(PATH_TORCH)
	MESSAGE(STATUS "  Found Torch...")
	INCLUDE_DIRECTORIES(${PATH_TORCH}/include)	
	FIND_LIBRARY(LIB_TORCH torch PATHS ${PATH_TORCH}/lib )
	IF(LIB_TORCH)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_TORCH}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_TORCH} )
	ELSE(LIB_TORCH)
		MESSAGE(STATUS "    [FAILED] lib TORCH")
	ENDIF(LIB_TORCH)

	ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyTorch)
	ADD_DEFINITIONS(-Wno-deprecated)
ELSE(PATH_TORCH)
	MESSAGE(STATUS "  FAILED to find Torch")
ENDIF(PATH_TORCH)

