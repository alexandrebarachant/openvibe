# ---------------------------------
# Finds third party boost
# Adds library to target
# Adds include path
# ---------------------------------

IF(UNIX)
	FIND_LIBRARY(LIB_Boost_Thread NAMES "boost_thread-mt" PATHS ${OV_CUSTOM_DEPENDENCIES_PATH}/lib NO_DEFAULT_PATH)
	FIND_LIBRARY(LIB_Boost_Thread NAMES "boost_thread-mt" PATHS ${OV_CUSTOM_DEPENDENCIES_PATH}/lib)
	IF(LIB_Boost_Thread)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_Boost_Thread}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_Boost_Thread} )
	ELSE(LIB_Boost_Thread)
		MESSAGE(STATUS "    [FAILED] lib boost_thread-mt")
	ENDIF(LIB_Boost_Thread)

	# For Fedora
	FIND_LIBRARY(LIB_STANDARD_MODULE_PTHREAD pthread)
	IF(LIB_STANDARD_MODULE_PTHREAD)
		MESSAGE(STATUS "  Found pthread...")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_STANDARD_MODULE_PTHREAD})
	ELSE(LIB_STANDARD_MODULE_PTHREAD)
		MESSAGE(STATUS "  FAILED to find pthread...")
	ENDIF(LIB_STANDARD_MODULE_PTHREAD)
ENDIF(UNIX)
