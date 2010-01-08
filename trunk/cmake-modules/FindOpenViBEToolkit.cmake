# ---------------------------------
# Finds OpenViBE-toolkit
# Adds library to target
# Adds include path
# ---------------------------------
OPTION(DYNAMIC_LINK_OPENVIBE_TOOLKIT "Dynamically link OpenViBE-toolkit" ON)

IF(DYNAMIC_LINK_OPENVIBE_TOOLKIT)
	ADD_DEFINITIONS(-DOVTK_Shared)
ENDIF(DYNAMIC_LINK_OPENVIBE_TOOLKIT)

IF(DYNAMIC_LINK_OPENVIBE_TOOLKIT)
	SET(OPENVIBE_TOOLKIT_LINKING dynamic)
ELSE(DYNAMIC_LINK_OPENVIBE_TOOLKIT)
	SET(OPENVIBE_TOOLKIT_LINKING static)
ENDIF(DYNAMIC_LINK_OPENVIBE_TOOLKIT)

FIND_PATH(PATH_OPENVIBE_TOOLKIT include/openvibe-toolkit/ovtk_all.h PATHS $ENV{OpenViBE_toolkit})
IF(PATH_OPENVIBE_TOOLKIT)
	MESSAGE(STATUS "  Found OpenViBE-toolkit...")
	INCLUDE_DIRECTORIES(${PATH_OPENVIBE_TOOLKIT}/include)
	FIND_LIBRARY(LIB_OPENVIBE_TOOLKIT OpenViBE-toolkit-${OPENVIBE_LINKING} PATH ${PATH_OPENVIBE_TOOLKIT}/lib)
	IF(LIB_OPENVIBE_TOOLKIT)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_OPENVIBE_TOOLKIT}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_OPENVIBE_TOOLKIT})
	ELSE(LIB_OPENVIBE_TOOLKIT)
		MESSAGE(STATUS "    [FAILED] lib OpenViBE-toolkit-${OPENVIBE_TOOLKIT_LINKING}")
	ENDIF(LIB_OPENVIBE_TOOLKIT)

	ADD_DEFINITIONS(-DTARGET_HAS_OpenViBEToolkit)
ELSE(PATH_OPENVIBE_TOOLKIT)
	MESSAGE(STATUS "  FAILED to find OpenViBE-toolkit...")
ENDIF(PATH_OPENVIBE_TOOLKIT)

