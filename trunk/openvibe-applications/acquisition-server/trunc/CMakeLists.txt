PROJECT(OpenViBE-acquisition-server)

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
INCLUDE("FindOpenViBEModuleEBML")
INCLUDE("FindOpenViBEModuleSocket")
INCLUDE("FindOpenViBEModuleSystem")
INCLUDE("FindOpenViBEModuleFS")
INCLUDE("FindOpenViBEPluginsGlobalDefines")
INCLUDE("FindThirdPartyGTK")
INCLUDE("FindThirdPartyBrainmasterCodeMakerAPI")
INCLUDE("FindThirdPartyActiCHampAPI")
INCLUDE("FindThirdPartyGUSBampCAPI")
INCLUDE("FindThirdPartyGMobiLabPlusAPI")
INCLUDE("FindThirdPartyUSBFirstAmpAPI")
INCLUDE("FindThirdPartyThinkGearAPI")
INCLUDE("FindThirdPartyEmotivAPI")
INCLUDE("FindThirdPartyOpenAL")
INCLUDE("FindThirdPartyBoost")
INCLUDE("FindThirdPartyBoost_Thread")	# for linux

# ---------------------------------
# Target macros
# Defines target operating system
# Defines target architecture
# Defines target compilator
# ---------------------------------
IF(WIN32)
	#ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
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

#begin fieldtrip driver:
IF(WIN32)		
	SET(PTHREAD_LIB "PTHREAD_LIB-NOTFOUND")
	SET(PTHREAD_LIB ${OV_WIN32_DEP_DIR}/pthreads/lib/pthreadVCE2.lib)

	IF(PTHREAD_LIB)
		MESSAGE(STATUS "    [  OK  ] Third party lib ${PTHREAD_LIB}")
	ELSE(PTHREAD_LIB)
		MESSAGE(STATUS "    [FAILED] Third party lib ${PTHREAD_LIB}")
	ENDIF(PTHREAD_LIB)
	
	INCLUDE("OvSetWindowsSDKPath")
	SET(WS232_LIB "WS232_LIB-NOTFOUND")
	FIND_LIBRARY(WS232_LIB ws2_32 ${OV_MS_SDK_PATH}/lib)
	
	IF(WS232_LIB)
		MESSAGE(STATUS "    [  OK  ] Third party lib ${WS232_LIB}")
	ELSE(WS232_LIB)
		MESSAGE(STATUS "    [FAILED] Third party lib ${WS232_LIB}")
	ENDIF(WS232_LIB)

	IF(PTHREAD_LIB AND WS232_LIB)
		INCLUDE_DIRECTORIES(${OV_WIN32_DEP_DIR}/pthreads/include)		
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${WS232_LIB} )
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${PTHREAD_LIB})
	ENDIF(PTHREAD_LIB AND WS232_LIB)

ENDIF(WIN32)
#end fieldtrip driver

# ----------------------
# Generate launch script
# ----------------------
OV_INSTALL_LAUNCH_SCRIPT("ov-acquisition-server")

# -----------------------------
# Install files
# -----------------------------
INSTALL(TARGETS ${PROJECT_NAME}-dynamic
	RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

INSTALL(DIRECTORY share/ DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}   PATTERN ".svn" EXCLUDE)

#INSTALL(DIRECTORY src/   DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE PATTERN "doc" EXCLUDE)
