CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
	
PROJECT(OpenVIBE)

# These versions are used by the subprojects by default.
# If you wish to maintain specific version numbers for a subproject, please do so in the projects CMakeLists.txt
SET(OV_GLOBAL_VERSION_MAJOR 0)
SET(OV_GLOBAL_VERSION_MINOR "X")
SET(OV_GLOBAL_VERSION_PATCH "Y")

# Default is to build to dist/. If you wish a custom install, set your own MAKE_INSTALL_PREFIX when you call CMake. Safest to do under a fakeroot.
#IF(NOT CMAKE_INSTALL_PREFIX)
	SET(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist")
#ENDIF(NOT CMAKE_INSTALL_PREFIX)

MESSAGE(STATUS "Appending compilation flags...")
	
SET(OV_CUSTOM_DEPENDENCIES_PATH "${CMAKE_SOURCE_DIR}/dependencies")	


IF(WIN32)
	ADD_DEFINITIONS("-DNOMINMAX -DBOOST_ALL_NO_LIB")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4355")
	SET(OV_WIN32_BOOST_VERSION "1_47")
ELSEIF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
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
## SET(OV_TRUNK "trunc")

# SET(OV_COMPILE_TESTS "true")

# By setting SKIP[_FOLDER]* you can skip a subtree (example: SKIP_A_B_C skips folder a/b/c and all its subfolders if any)
SET(SKIP_DOCUMENTATION "1")
#SET(SKIP_CONTRIB_PLUGINS_PROCESSING_PYTHON "1")

#SET(SKIP_MODULES_AUTOMATON "1")
#SET(SKIP_MODULES_EBML "1")
#SET(SKIP_MODULES_FS "1")
#SET(SKIP_MODULES_SOCKET "1")
#SET(SKIP_MODULES_STREAM "1")
#SET(SKIP_MODULES_SYSTEM "1")
#SET(SKIP_MODULES_XML "1")
#SET(SKIP_APPLICATIONS_PLATFORM_ACQUISITION-SERVER "1")
#SET(SKIP_APPLICATIONS_PLATFORM_DESIGNER "1")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")

# If you wish you can uncomment the next line and add a file with additional information called OVBranches.cmake to
# the cmake-modules folder. It makes modifying this very CMake file easier.
#INCLUDE("OVCustomSettings")

# a list of all project which will be filled by the directory traversal. This is needed to generate the documentation.
SET_PROPERTY(GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS "") 	
SET_PROPERTY(GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS_BUILD_DIR "")

# Used by the various Find* scripts to locate OpenViBE modules
SET(OV_BASE_DIR ${CMAKE_SOURCE_DIR})

# needed for making visual studio projects when this script is called without CMAKE_BUILD_TYPE
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG "TARGET_BUILDTYPE_Debug")
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE "TARGET_BUILDTYPE_Release")
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELWITHDEBINFO "TARGET_BUILDTYPE_Release")

# OpenViBE-specific helper functions that are used by the CMakeLists of the subprojects.
#INCLUDE("OvAddSingleProject")
INCLUDE("OvAddProjects")
INCLUDE("OvInstallLaunchScript")
INCLUDE("OvLinkBoostLib")

# Setup knowledge of GNU-style install path variables
INCLUDE("GNUInstallDirs")
SET(CMAKE_INSTALL_FULL_DOCDIR "${CMAKE_INSTALL_FULL_BINDIR}/../doc/html")
	
# Add directories that OpenViBE will use to look for its components runtime, unless overridden by environment variables in the launch scripts.
# These defines should only be used in "openvibe/ov_directories.h".
ADD_DEFINITIONS(-DOV_CMAKE_PATH_ROOT="${CMAKE_INSTALL_PREFIX}")
ADD_DEFINITIONS(-DOV_CMAKE_PATH_BIN="${CMAKE_INSTALL_FULL_BINDIR}")	
ADD_DEFINITIONS(-DOV_CMAKE_PATH_DATA="${CMAKE_INSTALL_FULL_DATADIR}/openvibe")
ADD_DEFINITIONS(-DOV_CMAKE_PATH_LIB="${CMAKE_INSTALL_FULL_LIBDIR}")
	
# Traverse these directories and build their components
ADD_SUBDIRECTORY("common/")
ADD_SUBDIRECTORY("openvibe/")
ADD_SUBDIRECTORY("kernel/")
ADD_SUBDIRECTORY("modules/")
ADD_SUBDIRECTORY("toolkit/")
ADD_SUBDIRECTORY("plugins/")
ADD_SUBDIRECTORY("applications/")
ADD_SUBDIRECTORY("externals/")
ADD_SUBDIRECTORY("contrib/")
ADD_SUBDIRECTORY("documentation/") # needs to be the last since it uses the list of collected projects

#add dependency to plugin inspector to the documentation
IF(openvibe-documentation)
	ADD_DEPENDENCIES(openvibe-documentation openvibe-plugin-inspector)
ENDIF(openvibe-documentation)
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
	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/openvibe-ogre-plugins.cfg DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/openvibe)
ENDIF(UNIX)

