# ---------------------------------
# Adds VRPN library to the target project
# Adds include path
# Requires FindThirdPartyVRPN_Check.cmake to have been run successfully before
# ---------------------------------

IF(PATH_VRPN)
	# Find all library paths
	
	IF(WIN32)
		FIND_LIBRARY(LIB_VRPN vrpn PATHS ${PATH_VRPN}/lib NO_DEFAULT_PATH)
		FIND_LIBRARY(LIB_VRPN vrpn PATHS ${PATH_VRPN}/lib)
		IF(LIB_VRPN)
			MESSAGE(STATUS "    [  OK  ] lib ${LIB_VRPN}")
		ELSE(LIB_VRPN)
			MESSAGE(STATUS "    [FAILED] lib vrpn")
			RETURN()
		ENDIF(LIB_VRPN)
	ENDIF(WIN32)

	IF(UNIX)
		FIND_LIBRARY(LIB_VRPNSERVER vrpnserver PATHS ${PATH_VRPN}/lib NO_DEFAULT_PATH)
		FIND_LIBRARY(LIB_VRPNSERVER vrpnserver PATHS ${PATH_VRPN}/lib)
		IF(LIB_VRPNSERVER)
			MESSAGE(STATUS "    [  OK  ] lib ${LIB_VRPNSERVER}")
		ELSE(LIB_VRPNSERVER)
			MESSAGE(STATUS "    [FAILED] lib vrpnserver")
			RETURN()
		ENDIF(LIB_VRPNSERVER)
	ENDIF(UNIX)

	FIND_LIBRARY(LIB_QUAT quat PATHS ${PATH_VRPN}/lib NO_DEFAULT_PATH)
	FIND_LIBRARY(LIB_QUAT quat PATHS ${PATH_VRPN}/lib)
	IF(LIB_QUAT)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_QUAT}")
	ELSE(LIB_QUAT)
		MESSAGE(STATUS "    [FAILED] lib quat")
		RETURN()
	ENDIF(LIB_QUAT)

	# If we reach here, everything has been found, add
	INCLUDE_DIRECTORIES(${PATH_VRPN}/include)
	
	IF(WIN32)
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_VRPN})	
	ENDIF(WIN32)
	IF(UNIX)
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_VRPNSERVER})	
	ENDIF(UNIX)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_QUAT})
	
	ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyVRPN)
ELSE(PATH_VRPN)
	MESSAGE(AUTHOR_WARNING "Should not run FindThirdPartyVRPN without FindThirdPartyVRPN_Check passing successfully first")
ENDIF(PATH_VRPN)
