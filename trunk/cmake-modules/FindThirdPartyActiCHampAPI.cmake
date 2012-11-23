# ---------------------------------
# Finds VAmp actiCHamp library
# Adds library to target
# Adds include path
# ---------------------------------
IF(WIN32)
	FIND_PATH(PATH_ActiCHampAPI actiCHamp.h PATHS "C:/Program Files/actichamp" "C:/Program Files (x86)/actichamp" $ENV{OpenViBE_dependencies})
	IF(PATH_ActiCHampAPI)
		MESSAGE(STATUS "  Found actiCHamp API...")
		INCLUDE_DIRECTORIES(${PATH_ActiCHampAPI})

		FIND_LIBRARY(LIB_ActiCHampAPI actiCHamp_x86 PATHS ${PATH_ActiCHampAPI} )
		IF(LIB_ActiCHampAPI)
			MESSAGE(STATUS "    [  OK  ] lib ${LIB_ActiCHampAPI}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_ActiCHampAPI} )
		ELSE(LIB_ActiCHampAPI)
			MESSAGE(STATUS "    [FAILED] lib actiCHamp")
		ENDIF(LIB_ActiCHampAPI)

		FIND_FILE(FIRMWARE_ActiCHampAPI ActiChamp.bit PATHS ${PATH_ActiCHampAPI} )
		IF(FIRMWARE_ActiCHampAPI)
			MESSAGE(STATUS "    [  OK  ] firmware ${FIRMWARE_ActiCHampAPI}")
		ELSE(FIRMWARE_ActiCHampAPI)
			MESSAGE(STATUS "    [FAILED] firmware actiCHamp")
		ENDIF(FIRMWARE_ActiCHampAPI)

		# Copying the DLL file at postbuild
		ADD_CUSTOM_COMMAND(
				TARGET ${PROJECT_NAME}-dynamic
				POST_BUILD
				COMMAND ${CMAKE_COMMAND}
				ARGS -E copy "${PATH_ActiCHampAPI}/actiCHamp_x86.dll" "${PROJECT_SOURCE_DIR}/bin"
				COMMENT "      --->   Copying dll file ${PATH_ActiCHampAPI}/actiCHamp_x86.dll for the BrainProducts actiCHamp driver."
			VERBATIM)

		# Copying the firmware file at postbuild
		ADD_CUSTOM_COMMAND(
				TARGET ${PROJECT_NAME}-dynamic
				POST_BUILD
				COMMAND ${CMAKE_COMMAND}
				ARGS -E copy "${FIRMWARE_ActiCHampAPI}" "${PROJECT_SOURCE_DIR}/bin"
				COMMENT "      --->   Copying firmware file ${FIRMWARE_ActiCHampAPI} for the BrainProducts actiCHamp driver."
			VERBATIM)

		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyActiCHampAPI)
	ELSE(PATH_ActiCHampAPI)
		MESSAGE(STATUS "  FAILED to find actiCHamp API - cmake looked in 'C:/Program Files/actichamp' and 'C:/Program Files (x86)/actichamp'")
	ENDIF(PATH_ActiCHampAPI)
ENDIF(WIN32)
