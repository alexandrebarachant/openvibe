# ---------------------------------
# Finds third party boost
# Adds library to target
# Adds include path
# ---------------------------------

SET(Boost_ADDITIONAL_VERSIONS 1.34.1)
FIND_PACKAGE(Boost 1.36.0)

IF(Boost_FOUND)
	MESSAGE(STATUS "  Found boost...")
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
#	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

	ADD_DEFINITIONS(-DTARGET_HAS_Boost)
ELSE(Boost_FOUND)
	MESSAGE(STATUS "  FAILED to find boost...")
ENDIF(Boost_FOUND)
