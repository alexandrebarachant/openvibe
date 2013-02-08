PROJECT(OpenViBE-ssvep-demo)

SET(PROJECT_VERSION_MAJOR x)
SET(PROJECT_VERSION_MINOR x)
SET(PROJECT_VERSION_PATCH x)
SET(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# The following is required because one of our dependencies hard-links against specific boost libs on Win, so we need to tell the linker the path to them. 
# Due to CMake, this needs to be done _before_ creating the target.
INCLUDE("OvAddBoostLinkDir")

FILE(GLOB_RECURSE source_files src/*.cpp src/*.h)
ADD_EXECUTABLE(${PROJECT_NAME}-dynamic ${source_files})

INCLUDE("FindOpenViBE")
INCLUDE("FindOpenViBEToolkit")
INCLUDE("FindOpenViBEModuleSystem")
INCLUDE("FindOpenViBEModuleEBML")
INCLUDE("FindOpenViBEModuleFS")
INCLUDE("FindThirdPartyCEGUI") # CEGUI + CEGUIOgreRender included
INCLUDE("FindThirdPartyVRPN")
INCLUDE("FindThirdPartyOgre3D") # OGRE + OIS included
INCLUDE("FindThirdPartyBoost")
INCLUDE("FindThirdPartyBoost_Thread")

# ---------------------------------
# Finds standard library pthread
# Adds library to target
# Adds include path
# ---------------------------------
IF(UNIX)
	FIND_LIBRARY(LIB_STANDARD_MODULE_PTHREAD pthread)
	IF(LIB_STANDARD_MODULE_PTHREAD)
		MESSAGE(STATUS "  Found pthread...")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_STANDARD_MODULE_PTHREAD})
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
OV_INSTALL_LAUNCH_SCRIPT("ov-ssvep-demo-training" "training")
OV_INSTALL_LAUNCH_SCRIPT("ov-ssvep-demo-shooter" "shooter")

# -----------------------------
# Install files
# -----------------------------
INSTALL(TARGETS ${PROJECT_NAME}-dynamic
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

INSTALL(DIRECTORY bin/   DESTINATION bin     PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY doc/   DESTINATION doc     PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY etc/   DESTINATION etc     PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY lib/   DESTINATION lib     PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY share/ DESTINATION share   PATTERN ".svn" EXCLUDE)

#INSTALL(DIRECTORY src/   DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE PATTERN "doc" EXCLUDE)
