# ---------------------------------
# Finds Emotiv SDK
# Adds library to target
# Adds include path
# ---------------------------------
IF(WIN32)
# edkErrorCode.h EmoStateDLL.h
	FIND_PATH(PATH_EmotivAPI edk.h  PATHS "C:/Program Files/Emotiv/Emotiv Development Kit_v1.0.0.3-PREMIUM" "C:/Program Files/Emotiv Development Kit_v1.0.0.3-PREMIUM" "C:/Program Files/Emotiv/Emotiv Development Kit_v1.0.0.3-PREMIUM/doc/examples/include" "C:/Program Files/Emotiv Development Kit_v1.0.0.3-PREMIUM/doc/examples/include" "C:/Program Files (x86)/Emotiv/Emotiv Development Kit_v1.0.0.3-PREMIUM" "C:/Program Files (x86)/Emotiv Development Kit_v1.0.0.3-PREMIUM" "C:/Program Files (x86)/Emotiv/Emotiv Development Kit_v1.0.0.3-PREMIUM/doc/examples/include" "C:/Program Files (x86)/Emotiv Development Kit_v1.0.0.3-PREMIUM/doc/examples/include" $ENV{OpenViBE_dependencies})
	IF(PATH_EmotivAPI)
		MESSAGE(STATUS "  Found Emotiv API...")
		INCLUDE_DIRECTORIES(${PATH_EmotivAPI})
		FIND_LIBRARY(LIB_EmotivAPI edk PATHS "${PATH_EmotivAPI}/../lib")
		IF(LIB_EmotivAPI)
			MESSAGE(STATUS "    [  OK  ] lib ${LIB_EmotivAPI}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_EmotivAPI} )
		ELSE(LIB_EmotivAPI)
			MESSAGE(STATUS "    [FAILED] lib Emotiv edk.lib")
		ENDIF(LIB_EmotivAPI)

		# Copying the DLL files at postbuild
		ADD_CUSTOM_COMMAND(
				TARGET ${PROJECT_NAME}-dynamic
				POST_BUILD
				COMMAND ${CMAKE_COMMAND}
				ARGS -E copy "${PATH_EmotivAPI}/../bin/edk.dll" "${PROJECT_SOURCE_DIR}/bin"
				COMMENT "      --->   Copying dll file ${PATH_EmotivAPI}/../bin/edk.dll for the Emotiv EPOC driver."
			VERBATIM)
		ADD_CUSTOM_COMMAND(
				TARGET ${PROJECT_NAME}-dynamic
				POST_BUILD
				COMMAND ${CMAKE_COMMAND}
				ARGS -E copy "${PATH_EmotivAPI}/../bin/edk_utils.dll" "${PROJECT_SOURCE_DIR}/bin"
				COMMENT "      --->   Copying dll file ${PATH_EmotivAPI}/../bin/edk_utils.dll for the Emotiv EPOC driver."
			VERBATIM)
		
		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyEmotivAPI)
	ELSE(PATH_EmotivAPI)
		MESSAGE(STATUS "  FAILED to find Emotiv API - cmake looked in 'C:/Program Files/Emotiv/Emotiv Development Kit_v1.0.0.3-PREMIUM' 'C:/Program Files/Emotiv Development Kit_v1.0.0.3-PREMIUM' 'C:/Program Files/Emotiv/Emotiv Development Kit_v1.0.0.3-PREMIUM/doc/examples/include' 'C:/Program Files/Emotiv Development Kit_v1.0.0.3-PREMIUM/doc/examples/include' and '$ENV{OpenViBE_dependencies}'")
	ENDIF(PATH_EmotivAPI)
ENDIF(WIN32)
