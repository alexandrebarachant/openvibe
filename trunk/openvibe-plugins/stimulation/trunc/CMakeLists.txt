PROJECT(OpenViBE-plugins-stimulation)

SET(PROJECT_VERSION_MAJOR x)
SET(PROJECT_VERSION_MINOR x)
SET(PROJECT_VERSION_PATCH x)
SET(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# The following is required because one of our dependencies hard-links against specific boost libs on Win, so we need to tell the linker the path to them. 
# Due to CMake, this needs to be done _before_ creating the target.
INCLUDE("OvAddBoostLinkDir")

FILE(GLOB_RECURSE source_files src/*.cpp src/*.h)
ADD_LIBRARY(${PROJECT_NAME}-dynamic SHARED ${source_files})
SET_TARGET_PROPERTIES(${PROJECT_NAME}-dynamic PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	COMPILE_FLAGS "-DOVP_Exports -DOVP_Shared")
	
INCLUDE("FindOpenViBE")
INCLUDE("FindOpenViBEToolkit")
INCLUDE("FindOpenViBEModuleEBML")
INCLUDE("FindOpenViBEModuleAutomaton")
INCLUDE("FindOpenViBEPluginsGlobalDefines")
INCLUDE("FindThirdPartyGTK")
INCLUDE("FindThirdPartyLua")
INCLUDE("FindThirdPartyOpenAL")
INCLUDE("FindThirdPartyBoost")
INCLUDE("FindThirdPartyBoost_Thread")

# ---------------------------------
# Finds standard library winmm
# Adds library to target
# Adds include path
# ---------------------------------
IF(WIN32)
	FIND_LIBRARY(LIB_STANDARD_MODULE_WINMM winmm)
	IF(LIB_STANDARD_MODULE_WINMM)
		MESSAGE(STATUS "  Found winmm...")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_STANDARD_MODULE_WINMM})
	ELSE(LIB_STANDARD_MODULE_WINMM)
		MESSAGE(STATUS "  FAILED to find winmm...")
	ENDIF(LIB_STANDARD_MODULE_WINMM)
ENDIF(WIN32)

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

INSTALL(DIRECTORY share/ DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}   PATTERN ".svn" EXCLUDE)

#INSTALL(DIRECTORY src/   DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE PATTERN "doc" EXCLUDE)
