FILE(GLOB_RECURSE source_files *.cpp)

ADD_LIBRARY(${PROJECT_NAME}-static STATIC ${source_files})
ADD_LIBRARY(${PROJECT_NAME}-dynamic SHARED ${source_files})

ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-toolkit-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-toolkit-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-toolkit-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-toolkit-static)

SET_TARGET_PROPERTIES(${PROJECT_NAME}-dynamic PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	COMPILE_FLAGS "-DOVK_Exports -DOVK_Shared")

INCLUDE("FindOpenViBE.cmake")
INCLUDE("FindOpenViBEModuleEBML.cmake")
INCLUDE("FindOpenViBEModuleFS.cmake")
INCLUDE("FindOpenViBEModuleSocket.cmake")
INCLUDE("FindOpenViBEModuleSystem.cmake")
INCLUDE("FindOpenViBEModuleXML.cmake")

# ---------------------------------
# Finds standard library dl
# Adds library to target
# Adds include path
# ---------------------------------
IF(UNIX)
	FIND_LIBRARY(LIB_STANDARD_MODULE_DL dl)
	IF(LIB_STANDARD_MODULE_DL)
		MESSAGE(STATUS "  Found dl...")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_STANDARD_MODULE_DL})
	ELSE(LIB_STANDARD_MODULE_PTHREAD)
		MESSAGE(STATUS "  FAILED to find dl...")
	ENDIF(LIB_STANDARD_MODULE_DL)
ENDIF(UNIX)

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
# Finds thirdparty OpenMASK
# Adds library to target
# Adds include path
# ---------------------------------
# OMK_FOUND    = system has OpenMASK lib
# OMK_INCFLAGS = Cflags used on gcc
# OMK_INC      = where to find headers 
# OMK_LIBS     = libs used by OpenMASK
# OMK_LIBFLAGS = ldflags used on gcc
# OMK_LIBDIR   = full path(S) to the libraries, ie OpenSG path + ...
INCLUDE("FindOpenMASK.cmake")
IF(OMK_FOUND)
	MESSAGE(STATUS "  Found OpenMASK...")
	INCLUDE_DIRECTORIES(${OMK_INC})
	ADD_DEFINITIONS(${OMK_INCFLAGS})
	LINK_DIRECTORIES(${OMK_LIBDIR})
	FOREACH(OMK_LIB ${OMK_LIBS})
		SET(OMK_LIB1 "") # pthread OpenMASK UserTypes pvm3 gpvm3 XmlLoader xml2
		STRING(REGEX MATCH "${OMK_LIB} " OMK_LIB1 "m glut GL GLU OSGBase OSGSystem OSGWindowGLUT X11 Xmu Xext Xi Xxf86vm 3DVis 3DInteractionBasicObjectsForInteraction 3DInteractionFob 3DInteractionMotionStar 3DInteractionGlove5DT 3DInteractionInteractionTriggers 3DInteractionInteractors 3DInteractionMouse 3DInteractionStandardInteractionAwareness 3DInteractionInteractive3DUtilities OMKSound SimpleLoader ")
		IF(OMK_LIB1)
		ELSE(OMK_LIB1)
			SET(OMK_LIB2 "OMK_LIB2-NOTFOUND")
			FIND_LIBRARY(OMK_LIB2 ${OMK_LIB} PATHS ${OMK_LIBDIR})
			IF(OMK_LIB2)
				MESSAGE(STATUS "    [  OK  ] Third party lib ${OMK_LIB2}")
				TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${OMK_LIB2})
			ELSE(OMK_LIB2)
				MESSAGE(STATUS "    [FAILED] Third party lib ${OMK_LIB}")
			ENDIF(OMK_LIB2)
		ENDIF(OMK_LIB1)
	ENDFOREACH(OMK_LIB)
ELSE(OMK_FOUND)
	MESSAGE(STATUS "  FAILED to find OpenMASK...")
ENDIF(OMK_FOUND)

# ---------------------------------
# Finds thirdparty OpenSG
# Adds library to target
# Adds include path
# ---------------------------------
# OPENSG_LIBRARIES        = full path to the libraries
# CMAKE_OPENSG_CXX_FLAGS  = Unix compiler flags for OPENSG, essentially "`osg-config --cxxflags`"
# OPENSG_INCLUDE_DIR      = where to find headers 
# OPENSG_LINK_DIRECTORIES = link directories, useful for rpath on Unix
# OPENSG_EXE_LINKER_FLAGS = rpath on Unix

# INCLUDE("FindOpenSG.cmake")
# IF(OPENSG_FOUND)
# 	MESSAGE(STATUS "  Found OpenSG")
# 	INCLUDE_DIRECTORIES(${OPENSG_INCLUDE_DIR})
# 	ADD_DEFINITIONS(${CMAKE_OPENSG_CXX_FLAGS})
# 	LINK_DIRECTORIES(${OPENSG_LINK_DIRECTORIES})
# 	FOREACH(OSG_LIB ${OPENSG_LIBRARIES})
# 		SET(OSG_LIB2 "OSG_LIB2-NOTFOUND")
# 		FIND_LIBRARY(OSG_LIB2 ${OSG_LIB} PATHS ${OPENSG_LINK_DIRECTORIES})
# 		IF(OSG_LIB2)
# 			MESSAGE(STATUS "    [  OK  ] Third party lib ${OSG_LIB2}")
# 			TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${OSG_LIB2})
# 		ELSE(OSG_LIB2)
# 			MESSAGE(STATUS "    [FAILED] Third party lib ${OSG_LIB}")
# 		ENDIF(OSG_LIB2)
# 	ENDFOREACH(OSG_LIB)
# ENDIF(OPENSG_FOUND)
