PROJECT(openvibe-vr-demo)

SET(PROJECT_VERSION_MAJOR ${OV_GLOBAL_VERSION_MAJOR})
SET(PROJECT_VERSION_MINOR ${OV_GLOBAL_VERSION_MINOR})
SET(PROJECT_VERSION_PATCH ${OV_GLOBAL_VERSION_PATCH})
SET(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

INCLUDE("FindThirdPartyVRPN_Check")
IF(NOT PATH_VRPN)
	MESSAGE(STATUS "    --> Not building ${PROJECT_NAME}")
	RETURN()
ENDIF(NOT PATH_VRPN)
 
# The following is required because one of our dependencies hard-links against specific boost libs on Win, so we need to tell the linker the path to them. 
# Due to CMake, this needs to be done _before_ creating the target.
INCLUDE("OvAddBoostLinkDir")

FILE(GLOB_RECURSE source_files src/*.cpp src/*.h src/*.inl)
ADD_EXECUTABLE(${PROJECT_NAME} ${source_files})

INCLUDE("FindOpenViBE")
INCLUDE("FindOpenViBEModuleSystem")
INCLUDE("FindOpenViBEModuleFS")
INCLUDE("FindThirdPartyCEGUI") # CEGUI + CEGUIOgreRender included
INCLUDE("FindThirdPartyVRPN")
INCLUDE("FindThirdPartyOgre3D") # OGRE + OIS included
INCLUDE("FindThirdPartyBoost")
INCLUDE("FindThirdPartyBoost_Thread") # needed on linux to add lib

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
# Defines target compilator
# ---------------------------------
IF(WIN32)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ADD_DEFINITIONS(-DTARGET_OS_Windows)
	ADD_DEFINITIONS(-DTARGET_ARCHITECTURE_i386)
	ADD_DEFINITIONS(-DTARGET_COMPILATOR_VisualStudio)
ENDIF(WIN32)
IF(UNIX)
	# ADD_DEFINITIONS(-fvisibility=hidden) # This flag should be present... man gcc
	ADD_DEFINITIONS(-g)
	ADD_DEFINITIONS(-fnon-call-exceptions)
	ADD_DEFINITIONS(-DTARGET_OS_Linux)
	ADD_DEFINITIONS(-DTARGET_ARCHITECTURE_i386)
	ADD_DEFINITIONS(-DTARGET_COMPILATOR_GCC)
ENDIF(UNIX)

# ----------------------
# Generate launch script
# ----------------------
IF(WIN32)
	OV_INSTALL_LAUNCH_SCRIPT("openvibe-vr-demo-handball" "handball")
	OV_INSTALL_LAUNCH_SCRIPT("openvibe-vr-demo-tie-fighter" "tie-fighter")
ENDIF(WIN32)
IF(UNIX)
	OV_INSTALL_LAUNCH_SCRIPT("openvibe-vr-demo-handball" "handball")
	OV_INSTALL_LAUNCH_SCRIPT("openvibe-vr-demo-tie-fighter" "tie-fighter")
ENDIF(UNIX)

# -----------------------------
# Install files
# -----------------------------
INSTALL(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
	LIBRARY DESTINATION  DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
	ARCHIVE DESTINATION  DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

INSTALL(DIRECTORY share/ DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}   PATTERN ".svn" EXCLUDE)

#INSTALL(DIRECTORY src/   DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE PATTERN "doc" EXCLUDE)
