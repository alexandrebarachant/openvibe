# ---------------------------------
# Finds third party boost
# Adds library to target
# Adds include path
# ---------------------------------


IF(UNIX)
	FIND_LIBRARY(LIB_Boost_System NAMES "boost_system-mt" PATHS ${OV_CUSTOM_DEPENDENCIES_PATH}/lib NO_DEFAULT_PATH)
	FIND_LIBRARY(LIB_Boost_System NAMES "boost_system-mt" PATHS ${OV_CUSTOM_DEPENDENCIES_PATH}/lib)
	IF(LIB_Boost_System)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_Boost_System}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_Boost_System} )
	ELSE(LIB_Boost_System)
		MESSAGE(STATUS "    [FAILED] lib boost_system-mt")
	ENDIF(LIB_Boost_System)
ENDIF(UNIX)

