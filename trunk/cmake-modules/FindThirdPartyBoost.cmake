# ---------------------------------
# Finds third party boost
# Adds library to target
# Adds include path
# ---------------------------------

IF(UNIX)
	SET(Boost_ADDITIONAL_VERSIONS 1.34.1)
	FIND_PACKAGE(Boost 1.36.0)
	FIND_PACKAGE(Boost 1.36.0)

	IF(Boost_FOUND)
		MESSAGE(STATUS "  Found boost...")
		INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
#		LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

		ADD_DEFINITIONS(-DTARGET_HAS_Boost)
	ELSE(Boost_FOUND)
		MESSAGE(STATUS "  FAILED to find boost...")
	ENDIF(Boost_FOUND)
ENDIF(UNIX)

IF(WIN32)
	FIND_PATH(PATH_BOOST "include/boost/config/auto_link.hpp" PATHS $ENV{OV_DEP_BOOST} NO_DEFAULT_PATH)
	FIND_PATH(PATH_BOOST "include/boost/config/auto_link.hpp" PATHS $ENV{OV_DEP_BOOST})

	IF(PATH_BOOST)
		MESSAGE(STATUS "  Found boost...")
		INCLUDE_DIRECTORIES(${PATH_BOOST}/include)

		ADD_DEFINITIONS(-DTARGET_HAS_Boost)
	ELSE(PATH_BOOST)
		MESSAGE(STATUS "  FAILED to find boost...")
	ENDIF(PATH_BOOST)
ENDIF(WIN32)
