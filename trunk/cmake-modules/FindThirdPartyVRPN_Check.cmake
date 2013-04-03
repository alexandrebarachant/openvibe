# ---------------------------------
# Finds VRPN
# Sets PATH_VRPN if found
# ---------------------------------
FIND_PATH(PATH_VRPN include/vrpn_BaseClass.h PATHS ${OV_WIN32_DEP_DIR}/vrpn $ENV{VRPN_ROOT} NO_DEFAULT_PATH)
FIND_PATH(PATH_VRPN include/vrpn_BaseClass.h PATHS ${OV_WIN32_DEP_DIR}/vrpn $ENV{VRPN_ROOT})
IF(PATH_VRPN)
	MESSAGE(STATUS "  Found VRPN...")
ELSE(PATH_VRPN)
	MESSAGE(STATUS "  FAILED to find VRPN")
ENDIF(PATH_VRPN)
