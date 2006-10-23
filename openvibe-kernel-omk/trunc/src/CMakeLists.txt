FILE(GLOB_RECURSE source_files *.cpp)

ADD_LIBRARY(${PROJECT_NAME}-static ${source_files})

ADD_DEFINITIONS(-DOVK_Exports)
ADD_DEFINITIONS(-DOVK_Shared)

ADD_LIBRARY(${PROJECT_NAME}-dynamic SHARED ${source_files})

SET_TARGET_PROPERTIES(${PROJECT_NAME}-dynamic PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR})

# ---------------------------------
# Finds module EBML
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PATH(PATH_OPENVIBE_MODULES_EBML include/ebml/defines.h PATHS $ENV{OpenViBE_modules}/ebml $ENV{OpenViBE_module_ebml})
IF(PATH_OPENVIBE_MODULES_EBML)
	MESSAGE(STATUS "  Found OpenViBE module ebml...")
	INCLUDE_DIRECTORIES(${PATH_OPENVIBE_MODULES_EBML}/include)
	FIND_LIBRARY(LIB_OPENVIBE_MODULE_EBML OpenViBE-module-EBML-${MODULE_LINKING} PATHS ${PATH_OPENVIBE_MODULES_EBML}/lib)
	IF(LIB_OPENVIBE_MODULE_EBML)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_OPENVIBE_MODULE_EBML}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_OPENVIBE_MODULE_EBML})
	ELSE(LIB_OPENVIBE_MODULE_EBML)
		MESSAGE(STATUS "    [FAILED] lib OpenViBE-module-EBML-${MODULE_LINKING}")
	ENDIF(LIB_OPENVIBE_MODULE_EBML)
ELSE(PATH_OPENVIBE_MODULES_EBML)
	MESSAGE(STATUS "  FAILED to find OpenViBE module ebml...")
ENDIF(PATH_OPENVIBE_MODULES_EBML)

# ---------------------------------
# Finds module FS
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PATH(PATH_OPENVIBE_MODULES_FS include/fs/defines.h PATHS $ENV{OpenViBE_modules}/fs $ENV{OpenViBE_module_fs})
IF(PATH_OPENVIBE_MODULES_FS)
	MESSAGE(STATUS "  Found OpenViBE module fs...")
	INCLUDE_DIRECTORIES(${PATH_OPENVIBE_MODULES_FS}/include)
	FIND_LIBRARY(LIB_OPENVIBE_MODULE_FS OpenViBE-module-FS-${MODULE_LINKING} PATHS ${PATH_OPENVIBE_MODULES_FS}/lib)
	IF(LIB_OPENVIBE_MODULE_FS)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_OPENVIBE_MODULE_FS}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_OPENVIBE_MODULE_FS})
	ELSE(LIB_OPENVIBE_MODULE_FS)
		MESSAGE(STATUS "    [FAILED] lib OpenViBE-module-FS-${MODULE_LINKING}")
	ENDIF(LIB_OPENVIBE_MODULE_FS)
ELSE(PATH_OPENVIBE_MODULES_FS)
	MESSAGE(STATUS "  FAILED to find OpenViBE module fs...")
ENDIF(PATH_OPENVIBE_MODULES_FS)

# ---------------------------------
# Finds module Socket
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PATH(PATH_OPENVIBE_MODULES_SOCKET include/socket/defines.h PATHS $ENV{OpenViBE_modules}/socket $ENV{OpenViBE_module_socket})
IF(PATH_OPENVIBE_MODULES_SOCKET)
	MESSAGE(STATUS "  Found OpenViBE module socket...")
	INCLUDE_DIRECTORIES(${PATH_OPENVIBE_MODULES_SOCKET}/include)
	FIND_LIBRARY(LIB_OPENVIBE_MODULE_SOCKET OpenViBE-module-Socket-${MODULE_LINKING} PATHS ${PATH_OPENVIBE_MODULES_SOCKET}/lib)
	IF(LIB_OPENVIBE_MODULE_SOCKET)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_OPENVIBE_MODULE_SOCKET}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_OPENVIBE_MODULE_SOCKET})
	ELSE(LIB_OPENVIBE_MODULE_SOCKET)
		MESSAGE(STATUS "    [FAILED] lib OpenViBE-module-Socket-${MODULE_LINKING}")
	ENDIF(LIB_OPENVIBE_MODULE_SOCKET)
ELSE(PATH_OPENVIBE_MODULES_SOCKET)
	MESSAGE(STATUS "  FAILED to find OpenViBE module socket...")
ENDIF(PATH_OPENVIBE_MODULES_SOCKET)

# ---------------------------------
# Finds module System
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PATH(PATH_OPENVIBE_MODULES_SYSTEM include/system/defines.h PATHS $ENV{OpenViBE_modules}/system $ENV{OpenViBE_module_system})
IF(PATH_OPENVIBE_MODULES_SYSTEM)
	MESSAGE(STATUS "  Found OpenViBE module system...")
	INCLUDE_DIRECTORIES(${PATH_OPENVIBE_MODULES_SYSTEM}/include)
	FIND_LIBRARY(LIB_OPENVIBE_MODULE_SYSTEM OpenViBE-module-System-${MODULE_LINKING} PATHS ${PATH_OPENVIBE_MODULES_SYSTEM}/lib)
	IF(LIB_OPENVIBE_MODULE_SYSTEM)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_OPENVIBE_MODULE_SYSTEM}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_OPENVIBE_MODULE_SYSTEM})
	ELSE(LIB_OPENVIBE_MODULE_SYSTEM)
		MESSAGE(STATUS "    [FAILED] lib OpenViBE-module-System-${MODULE_LINKING}")
	ENDIF(LIB_OPENVIBE_MODULE_SYSTEM)
ELSE(PATH_OPENVIBE_MODULES_SYSTEM)
	MESSAGE(STATUS "  FAILED to find OpenViBE module system...")
ENDIF(PATH_OPENVIBE_MODULES_SYSTEM)

# ---------------------------------
# Finds module XML
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PATH(PATH_OPENVIBE_MODULES_XML include/xml/defines.h PATHS $ENV{OpenViBE_modules}/xml $ENV{OpenViBE_module_xml})
IF(PATH_OPENVIBE_MODULES_XML)
	MESSAGE(STATUS "  Found OpenViBE module xml...")
	INCLUDE_DIRECTORIES(${PATH_OPENVIBE_MODULES_XML}/include)
	FIND_LIBRARY(LIB_OPENVIBE_MODULE_XML OpenViBE-module-XML-${MODULE_LINKING} PATHS ${PATH_OPENVIBE_MODULES_XML}/lib)
	IF(LIB_OPENVIBE_MODULE_XML)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_OPENVIBE_MODULE_XML}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_OPENVIBE_MODULE_XML})
	ELSE(LIB_OPENVIBE_MODULE_XML)
		MESSAGE(STATUS "    [FAILED] lib OpenViBE-module-XML-${MODULE_LINKING}")
	ENDIF(LIB_OPENVIBE_MODULE_XML)
ELSE(PATH_OPENVIBE_MODULES_XML)
	MESSAGE(STATUS "  FAILED to find OpenViBE module xml...")
ENDIF(PATH_OPENVIBE_MODULES_XML)

# ---------------------------------
# Finds OpenViBE
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PATH(PATH_OPENVIBE include/openvibe/ov_all.h PATHS $ENV{OpenViBE})
IF(PATH_OPENVIBE)
	MESSAGE(STATUS "  Found OpenViBE...")
	INCLUDE_DIRECTORIES(${PATH_OPENVIBE}/include)
	FIND_LIBRARY(LIB_OPENVIBE OpenViBE-${OPENVIBE_LINKING} PATH ${PATH_OPENVIBE}/lib)
	IF(LIB_OPENVIBE)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_OPENVIBE}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_OPENVIBE})
	ELSE(LIB_OPENVIBE)
		MESSAGE(STATUS "    [FAILED] lib OpenViBE-${OPENVIBE_LINKING}")
	ENDIF(LIB_OPENVIBE)
ELSE(PATH_OPENVIBE)
	MESSAGE(STATUS "  FAILED to find OpenViBE...")
ENDIF(PATH_OPENVIBE)

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
