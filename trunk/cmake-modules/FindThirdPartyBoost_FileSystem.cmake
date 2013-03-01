# ---------------------------------
# Finds third party boost
# Adds library to target
# Adds include path
# ---------------------------------

IF(UNIX)
	FIND_LIBRARY(LIB_Boost_Filesystem NAMES "boost_filesystem-mt" PATHS $ENV{OpenViBE_dependencies}/lib NO_DEFAULT_PATH)
	FIND_LIBRARY(LIB_Boost_Filesystem NAMES "boost_filesystem-mt" PATHS $ENV{OpenViBE_dependencies}/lib)
	IF(LIB_Boost_Filesystem)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_Boost_Filesystem}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_Boost_Filesystem} )
	ELSE(LIB_Boost_Filesystem)
		MESSAGE(STATUS "    [FAILED] lib boost_Filesystem-mt")
	ENDIF(LIB_Boost_Filesystem)

	# For Fedora
	FIND_LIBRARY(LIB_STANDARD_MODULE_PTHREAD pthread)
	IF(LIB_STANDARD_MODULE_PTHREAD)
		MESSAGE(STATUS "  Found pthread...")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_STANDARD_MODULE_PTHREAD})
	ELSE(LIB_STANDARD_MODULE_PTHREAD)
		MESSAGE(STATUS "  FAILED to find pthread...")
	ENDIF(LIB_STANDARD_MODULE_PTHREAD)
ENDIF(UNIX)
