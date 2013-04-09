# ---------------------------------
# Finds VRPN
# Sets PATH_VRPN if found
# ---------------------------------
# The first ${..}/vrpn path is for Windows, the second ${..}/ for Linux
FIND_PATH(PATH_VRPN include/vrpn_BaseClass.h PATHS ${OV_CUSTOM_DEPENDENCIES_PATH}/vrpn ${OV_CUSTOM_DEPENDENCIES_PATH} NO_DEFAULT_PATH)
FIND_PATH(PATH_VRPN include/vrpn_BaseClass.h PATHS ${OV_CUSTOM_DEPENDENCIES_PATH}/vrpn ${OV_CUSTOM_DEPENDENCIES_PATH})
IF(PATH_VRPN)
	MESSAGE(STATUS "  Found VRPN...")
ELSE(PATH_VRPN)
	MESSAGE(STATUS "  FAILED to find VRPN")
ENDIF(PATH_VRPN)
