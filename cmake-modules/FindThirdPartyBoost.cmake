# ---------------------------------
# Finds third party boost
# Adds library to target
# Adds include path
# ---------------------------------
IF(WIN32)
	FIND_PACKAGE(Boost)
ENDIF(WIN32)

IF(UNIX)
	FIND_PATH(Boost_INCLUDE_DIRS "boost/config.hpp" PATHS "$ENV{OpenViBE_dependencies}/include" "$ENV{BOOST_ROOT}")
	IF(Boost_INCLUDE_DIRS)
		SET(Boost_FOUND TRUE)
	ELSE(Boost_INCLUDE_DIRS)
		SET(Boost_FOUND FALSE)
	ENDIF(Boost_INCLUDE_DIRS)
ENDIF(UNIX)

IF(Boost_FOUND)
	MESSAGE(STATUS "  Found boost...")
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
#	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

	ADD_DEFINITIONS(-DTARGET_HAS_Boost)
ELSE(Boost_FOUND)
	MESSAGE(STATUS "  FAILED to find boost...")
ENDIF(Boost_FOUND)
