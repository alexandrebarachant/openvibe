 #
 # The gMobilab driver was contributed
 # by Lucie Daubigney from Supelec Metz
 #

# ---------------------------------
# Finds GTecMobiLabPlus+
# Adds library to target
# Adds include path
# ---------------------------------

IF(WIN32)
	MESSAGE(STATUS "  Skipping MobiLabPlus, not supported on Windows")
ENDIF(WIN32)

IF(UNIX)
	FIND_LIBRARY(gMOBIlabplus_LIBRARY NAMES gMOBIlabplus PATHS /usr/lib /usr/local/lib)
	IF(gMOBIlabplus_LIBRARY)
		MESSAGE(STATUS "  Found GMobiLabPlusAPI...")
		MESSAGE(STATUS "    [  OK  ] lib ${gMOBIlabplus_LIBRARY}")
		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyGMobiLabPlusAPI)
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${gMOBIlabplus_LIBRARY} )
	ELSE(gMOBIlabplus_LIBRARY)
		MESSAGE(STATUS "  FAILED to find GMobiLabPlusAPI...")
	ENDIF(gMOBIlabplus_LIBRARY)
ENDIF(UNIX)
