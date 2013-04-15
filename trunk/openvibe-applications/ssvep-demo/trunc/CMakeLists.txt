PROJECT(openvibe-ssvep-demo)

SET(PROJECT_VERSION_MAJOR ${OV_GLOBAL_VERSION_MAJOR})
SET(PROJECT_VERSION_MINOR ${OV_GLOBAL_VERSION_MINOR})
SET(PROJECT_VERSION_PATCH ${OV_GLOBAL_VERSION_PATCH})
SET(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

INCLUDE("FindThirdPartyVRPN_Check")
IF(NOT PATH_VRPN)
	MESSAGE(STATUS "    --> Not building ${PROJECT_NAME}")
	RETURN()
ENDIF(NOT PATH_VRPN)

FILE(GLOB_RECURSE source_files src/*.cpp src/*.h src/*.inl)
ADD_EXECUTABLE(${PROJECT_NAME} ${source_files})

INCLUDE("FindOpenViBE")
INCLUDE("FindOpenViBECommon")
INCLUDE("FindOpenViBEToolkit")
INCLUDE("FindOpenViBEModuleSystem")
INCLUDE("FindOpenViBEModuleEBML")
INCLUDE("FindOpenViBEModuleFS")
INCLUDE("FindThirdPartyCEGUI") # CEGUI + CEGUIOgreRender included
INCLUDE("FindThirdPartyVRPN")
INCLUDE("FindThirdPartyOgre3D")			# OGRE + OIS included, pulls boost thread
INCLUDE("FindThirdPartyBoost")			# Using Ogre headers on Win32 causes dependency to Boost thread library
INCLUDE("FindThirdPartyBoost_Thread")	# Note that this is a potential issue on Windows, as the dependencies/ogre/boost and dependencies/boost are not the same at the time of writing this.

# ---------------------------------
# Finds standard library pthread
# Adds library to target
# Adds include path
# ---------------------------------
IF(UNIX)
	FIND_LIBRARY(LIB_STANDARD_MODULE_PTHREAD pthread)
	IF(LIB_STANDARD_MODULE_PTHREAD)
		MESSAGE(STATUS "  Found pthread...")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_STANDARD_MODULE_PTHREAD})
	ELSE(LIB_STANDARD_MODULE_PTHREAD)
		MESSAGE(STATUS "  FAILED to find pthread...")
	ENDIF(LIB_STANDARD_MODULE_PTHREAD)
ENDIF(UNIX)

# ---------------------------------
# Target macros
# Defines target operating system
# Defines target architecture
# Defines target compiler
# ---------------------------------
IF(WIN32)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ADD_DEFINITIONS(-DTARGET_OS_Windows)
	ADD_DEFINITIONS(-DTARGET_ARCHITECTURE_i386)
	ADD_DEFINITIONS(-DTARGET_COMPILER_VisualStudio)
ENDIF(WIN32)
IF(UNIX)
	# ADD_DEFINITIONS(-fvisibility=hidden) # This flag should be present... man gcc
	ADD_DEFINITIONS(-g)
	ADD_DEFINITIONS(-fnon-call-exceptions)
	ADD_DEFINITIONS(-DTARGET_OS_Linux)
	ADD_DEFINITIONS(-DTARGET_ARCHITECTURE_i386)
	ADD_DEFINITIONS(-DTARGET_COMPILER_GCC)
ENDIF(UNIX)

# ----------------------
# Generate launch script
# ----------------------
OV_INSTALL_LAUNCH_SCRIPT("openvibe-ssvep-demo")

# -----------------------------
# Install files
# -----------------------------
INSTALL(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
	LIBRARY DESTINATION  DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
	ARCHIVE DESTINATION  DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

INSTALL(DIRECTORY share/ DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}  PATTERN ".svn" EXCLUDE)

#INSTALL(DIRECTORY src/   DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE PATTERN "doc" EXCLUDE)
