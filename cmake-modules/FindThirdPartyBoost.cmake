# ---------------------------------
# Finds third party boost
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PACKAGE(Boost)
IF(Boost_FOUND)
	MESSAGE(STATUS "  Found boost...")
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

	ADD_DEFINITIONS(-DTARGET_HAS_Boost)
ELSE(Boost_FOUND)
	MESSAGE(STATUS "  FAILED to find boost...")
ENDIF(Boost_FOUND)
