PROJECT(OpenViBE-module-Socket)

SET(PROJECT_VERSION_MAJOR x)
SET(PROJECT_VERSION_MINOR x)
SET(PROJECT_VERSION_PATCH x)
SET(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

FILE(GLOB_RECURSE source_files src/*.cpp src/*.h)
FILE(GLOB_RECURSE header_files src/*.h)

INCLUDE("OvAddBoostLinkDir")

ADD_LIBRARY(${PROJECT_NAME}-dynamic SHARED ${source_files})

SET_TARGET_PROPERTIES(${PROJECT_NAME}-dynamic PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	COMPILE_FLAGS "-DSocket_Shared -DSocket_Exports")

INCLUDE("FindThirdPartyBoost")
	
# ---------------------------------
# Finds standard library winsock
# Adds library to target
# Adds include path
# ---------------------------------
IF(WIN32)
	INCLUDE("OvSetWindowsSDKPath")
	FIND_LIBRARY(LIB_STANDARD_MODULE_WINSOCK ws2_32 ${OV_MS_SDK_PATH}/lib)
	IF(LIB_STANDARD_MODULE_WINSOCK)
		MESSAGE(STATUS "  Found ws2_32...")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_STANDARD_MODULE_WINSOCK})
	ELSE(LIB_STANDARD_MODULE_WINSOCK)
		MESSAGE(STATUS "  FAILED to find ws2_32...")
	ENDIF(LIB_STANDARD_MODULE_WINSOCK)
ENDIF(WIN32)

# ---------------------------------
# Test applications
# ---------------------------------
OPTION(BUILD_OPENVIBE_MODULE_Socket_EXAMPLES "Build OpenViBE module Socket examples" OFF)
IF(BUILD_OPENVIBE_MODULE_Socket_EXAMPLES)
	SUBDIRS(test)
ENDIF(BUILD_OPENVIBE_MODULE_Socket_EXAMPLES)

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

# -----------------------------
# Install files
# -----------------------------
INSTALL(TARGETS ${PROJECT_NAME}-dynamic
	RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

INSTALL(DIRECTORY src/   DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE PATTERN "doc" EXCLUDE)
