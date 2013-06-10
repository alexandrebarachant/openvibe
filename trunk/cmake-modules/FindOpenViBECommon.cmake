# ---------------------------------
# Finds OpenViBE common include files
# Adds dependency to target
# Adds include path
# ---------------------------------

FIND_PATH(PATH_OPENVIBE_COMMON ov_common_defines.h PATHS ${OV_BASE_DIR}/openvibe-common/src NO_DEFAULT_PATH) 
IF(PATH_OPENVIBE_COMMON)
	MESSAGE(STATUS "  Found openvibe-common...")
	INCLUDE_DIRECTORIES(${PATH_OPENVIBE_COMMON})
	
	ADD_DEPENDENCIES(${PROJECT_NAME} openvibe-common)
	
	ADD_DEFINITIONS(-DTARGET_HAS_OpenViBE_Common)
	ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines)	
ELSE(PATH_OPENVIBE_COMMON)
	MESSAGE(STATUS "  FAILED to find openvibe-common...")
ENDIF(PATH_OPENVIBE_COMMON)


