CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
	
PROJECT(OpenVIBE)

# These versions are used by the subprojects by default.
# If you wish to maintain specific version numbers for a subproject, please do so in the projects CMakeLists.txt
SET(OV_GLOBAL_VERSION_MAJOR 0)
SET(OV_GLOBAL_VERSION_MINOR "X")
SET(OV_GLOBAL_VERSION_PATCH "Y")

# Default is to build to dist/. If you wish a custom install, set your own MAKE_INSTALL_PREFIX when you call CMake. Safest to do under a fakeroot.
IF(NOT CMAKE_INSTALL_PREFIX)
	SET(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist")
ENDIF(NOT CMAKE_INSTALL_PREFIX)

MESSAGE(STATUS "Appending compilation flags...")
	
IF(WIN32)
	ADD_DEFINITIONS("-DNOMINMAX")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4355")
	SET(OV_CUSTOM_DEPENDENCIES_PATH "${CMAKE_SOURCE_DIR}/dependencies")	
ELSEIF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	SET(OV_CUSTOM_DEPENDENCIES_PATH "${CMAKE_SOURCE_DIR}/scripts/software")
	# This ENV var is needed to locate our custom IT++ on Linux
	SET(ENV{PKG_CONFIG_PATH} "${OV_CUSTOM_DEPENDENCIES_PATH}/lib/pkgconfig")	
ELSE(WIN32)
	MESSAGE(WARNING "Warning: unknown platform")
ENDIF(WIN32)	

IF(NOT CMAKE_BUILD_TYPE)
	MESSAGE(STATUS "Setting default build type to Release")
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)

# Print the used compilation parameters (for transparency)
GET_DIRECTORY_PROPERTY(TMP_DEFINITIONS COMPILE_DEFINITIONS)
MESSAGE(STATUS "Compilation flags used at source root: ")
MESSAGE(STATUS "  COMPILE_DEFINITIONS = '${TMP_DEFINITIONS}'")
MESSAGE(STATUS "  CMAKE_CXX_FLAGS = '${CMAKE_CXX_FLAGS}'")
MESSAGE(STATUS "  CMAKE_CXX_FLAGS_RELEASE = '${CMAKE_CXX_FLAGS_RELEASE}'")
MESSAGE(STATUS "  CMAKE_CXX_FLAGS_DEBUG = '${CMAKE_CXX_FLAGS_DEBUG}'")

# if no specific branch has been given for an openvibe component, default is to compile its trunk folder
SET(OV_TRUNK "trunc")

# SET(OV_COMPILE_TESTS "true")

# To compile your own non-trunk branches, add variables OV_BRANCH_[DIR OR DIR_DIR] to point to "branches/YOURBRANCH" for each.
# Branch name __SKIPME signifies that this component should not be compiled or installed.
# SET(OV_BRANCH_PLUGINS_PYTHON "__SKIPME")			# skips python module
SET(OV_BRANCH_DOCUMENTATION "__SKIPME")				# skips documentation module
 
# Some recognized branch variables
#SET(OV_BRANCH_OPENVIBE													"branches/mybranch")
#SET(OV_BRANCH_KERNEL													"branches/mybranch")
#SET(OV_BRANCH_MODULES_AUTOMATON										"branches/mybranch")
#SET(OV_BRANCH_MODULES_EBML												"branches/mybranch")
#SET(OV_BRANCH_MODULES_FS												"branches/mybranch")
#SET(OV_BRANCH_MODULES_SOCKET											"branches/mybranch")
#SET(OV_BRANCH_MODULES_STREAM											"branches/mybranch")
#SET(OV_BRANCH_MODULES_SYSTEM											"branches/mybranch")
#SET(OV_BRANCH_MODULES_XML												"branches/mybranch")
#SET(OV_BRANCH_TOOLKIT													"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_ACQUISITION										"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_CLASSIFICATION									"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_CLASSIFICATION-GPL								"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_FEATURE-EXTRACTION								"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_FILE-IO											"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_MATLAB											"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_PYTHON 											"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_SAMPLES											"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_SIGNAL-PROCESSING 								"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_SIGNAL-PROCESSING-GPL 							"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_SIMPLE-VISUALISATION 							"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_STIMULATION 										"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_STREAM-CODECS									"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_STREAMING										"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_TOOLS											"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_TURBOFIELDTRIP									"branches/mybranch")
#SET(OV_BRANCH_PLUGINS_VRPN												"branches/mybranch")
#SET(OV_BRANCH_SCENARIOS												"branches/mybranch")
#SET(OV_BRANCH_APPLICATIONS_ACQUISITION-SERVER							"branches/mybranch")
#SET(OV_BRANCH_APPLICATIONS_DESIGNER									"branches/mybranch")
#SET(OV_BRANCH_APPLICATIONS_EXTERNAL-STIMULATION-CONNECTION-EXAMPLE		"branches/mybranch")
#SET(OV_BRANCH_APPLICATIONS_ID-GENERATOR								"branches/mybranch")
#SET(OV_BRANCH_APPLICATIONS_PLUGIN-INSPECTOR							"branches/mybranch")
#SET(OV_BRANCH_APPLICATIONS_SKELETON-GENERATOR							"branches/mybranch")
#SET(OV_BRANCH_APPLICATIONS_SSVEP-DEMO									"branches/mybranch")
#SET(OV_BRANCH_APPLICATIONS_VR-DEMO										"branches/mybranch")
#SET(OV_BRANCH_APPLICATIONS_VRPN-SIMULATOR								"branches/mybranch")
#SET(OV_BRANCH_DOCUMENTATION											"branches/mybranch")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")

# If you wish you can uncomment the next line and add a file with additional information called OVBranches.cmake to
# the cmake-modules folder. It makes modifying this very CMake file easier.
#INCLUDE("OVCustomSettings")

# a list of all project which will be filled by the directory traversal. This is needed to generate the documentation.
SET_PROPERTY(GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS "") 	

# Used by the various Find* scripts to locate OpenViBE modules
SET(OV_BASE_DIR ${CMAKE_SOURCE_DIR})

# needed for making visual studio projects when this script is called without CMAKE_BUILD_TYPE
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG "TARGET_BUILDTYPE_Debug")
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE "TARGET_BUILDTYPE_Release")
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELWITHDEBINFO "TARGET_BUILDTYPE_Release")

# OpenViBE-specific helper functions that are used by the CMakeLists of the subprojects.
INCLUDE("OvAddSingleProject")
INCLUDE("OvAddProjects")
INCLUDE("OvInstallLaunchScript")

# Setup knowledge of GNU-style install path variables
INCLUDE("GNUInstallDirs")
	
# Add directories that OpenViBE will use to look for its components runtime, unless overridden by environment variables in the launch scripts.
# These defines should only be used in "ov_directories.h".
ADD_DEFINITIONS(-DOV_CMAKE_PATH_ROOT="${CMAKE_INSTALL_PREFIX}")
ADD_DEFINITIONS(-DOV_CMAKE_PATH_BIN="${CMAKE_INSTALL_FULL_BINDIR}")	
ADD_DEFINITIONS(-DOV_CMAKE_PATH_DATA="${CMAKE_INSTALL_FULL_DATADIR}")
ADD_DEFINITIONS(-DOV_CMAKE_PATH_LIB="${CMAKE_INSTALL_FULL_LIBDIR}")
	
# Traverse these directories and build their components
ADD_SUBDIRECTORY("openvibe/")
ADD_SUBDIRECTORY("openvibe-kernel-omk/")
ADD_SUBDIRECTORY("openvibe-modules/")
ADD_SUBDIRECTORY("openvibe-toolkit/")
ADD_SUBDIRECTORY("openvibe-plugins/")
ADD_SUBDIRECTORY("openvibe-scenarios/")
ADD_SUBDIRECTORY("openvibe-applications/")
ADD_SUBDIRECTORY("openvibe-externals/")
ADD_SUBDIRECTORY("openvibe-documentation/")	# needs to be the last since it uses the list of collected projects

# make a copy of the dependency script, this is done so we can use the same launch scripts both in win install and win devel situations.
IF(WIN32)
	INSTALL(PROGRAMS ${CMAKE_SOURCE_DIR}/scripts/win32-dependencies.cmd DESTINATION ${CMAKE_INSTALL_FULL_BINDIR} RENAME "openvibe-set-env.cmd")
ENDIF(WIN32)

# ----------------------
# Generate 'external application launcher' script
# ----------------------
IF(WIN32)
	SET(OV_EXT_CMD_FILE "openvibe-external-application-launcher.cmd")
ENDIF(WIN32)
IF(UNIX)
	SET(OV_EXT_CMD_FILE "openvibe-external-application-launcher")
ENDIF(UNIX)

CONFIGURE_FILE("${CMAKE_MODULE_PATH}/launchers/${OV_EXT_CMD_FILE}-base" "${CMAKE_CURRENT_BINARY_DIR}/${OV_EXT_CMD_FILE}")
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${OV_EXT_CMD_FILE} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

# create the openvibe-ogre-plugins.cfg file for Ogre on UNIX
IF(UNIX)
	execute_process(COMMAND pkg-config --variable=plugindir OGRE OUTPUT_VARIABLE OGRE3D_PLUGIN_PATH)
	CONFIGURE_FILE("${CMAKE_MODULE_PATH}/launchers/openvibe-ogre-plugins.cfg-base" "${CMAKE_CURRENT_BINARY_DIR}/openvibe-ogre-plugins.cfg")
	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/openvibe-ogre-plugins.cfg DESTINATION ${CMAKE_INSTALL_FULL_DATADIR})
ENDIF(UNIX)



