# The construction is required because one of our dependencies hard-links against specific boost libs, so we need to tell the 
# linker the path to them. Due to CMake, this needs to be done _before_ creating the target.
#
# Adds boost link dir, this is separated from the Find* scripts as LINK_DIRECTORIES() needs to be called before target definition, 
# whereas TARGET_LINK_LIBRARIES (as used in the Find scripts) requires the target to be already set.

IF(WIN32)
 	INCLUDE("FindThirdPartyBoost")

	IF(PATH_BOOST)
		LINK_DIRECTORIES(${PATH_BOOST}/lib)
	ENDIF(PATH_BOOST)
ENDIF(WIN32)

