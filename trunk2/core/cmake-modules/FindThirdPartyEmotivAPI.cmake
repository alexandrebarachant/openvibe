# ---------------------------------
# Finds Emotiv SDK
# Adds library to target
# Adds include path
# ---------------------------------
IF(WIN32)
# edkErrorCode.h EmoStateDLL.h
	#FIND_PATH(PATH_EmotivAPI edk.h  PATHS "C:/Program Files/Emotiv Development Kit_v1.0.0.3-PREMIUM"  "C:/Program Files/Emotiv Development Kit_v1.0.0.3-PREMIUM/doc/examples/include" "C:/Program Files (x86)/Emotiv Development Kit_v1.0.0.3-PREMIUM" "C:/Program Files (x86)/Emotiv Development Kit_v1.0.0.3-PREMIUM/doc/examples/include" ${OV_CUSTOM_DEPENDENCIES_PATH})
	FILE(GLOB PATH_Candidates 
		"C:/Program Files/Emotiv Development Kit*" 
		"C:/Program Files/Emotiv Research Edition*"
		"C:/Program Files (x86)/Emotiv Development Kit*" 
		"C:/Program Files (x86)/Emotiv Research Edition*"
	)
	FOREACH(Candidate_folder ${PATH_Candidates}) 
		# MESSAGE(STATUS "Found path ${PATH_Candidate}")
		LIST(APPEND PATH_Candidates ${Candidate_folder}/doc/examples/include)
	ENDFOREACH(Candidate_folder ${PATH_Candidates})
	# MESSAGE(STATUS "Emotiv paths found ${PATH_Candidates}")
	
	FIND_PATH(PATH_EmotivAPI edk.h  PATHS ${PATH_Candidates} ${OV_CUSTOM_DEPENDENCIES_PATH})
	IF(PATH_EmotivAPI)
		MESSAGE(STATUS "  Found Emotiv API...")
		INCLUDE_DIRECTORIES(${PATH_EmotivAPI})
		FIND_LIBRARY(LIB_EmotivAPI edk PATHS "${PATH_EmotivAPI}/../lib")
		IF(LIB_EmotivAPI)
			MESSAGE(STATUS "    [  OK  ] lib ${LIB_EmotivAPI}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_EmotivAPI} )
			# To delay load EDK.dll and redistribute binary
			TARGET_LINK_LIBRARIES(${PROJECT_NAME} Delayimp.lib )
			SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/DELAYLOAD:edk.dll")
		ELSE(LIB_EmotivAPI)
			MESSAGE(STATUS "    [FAILED] lib Emotiv edk.lib")
		ENDIF(LIB_EmotivAPI)

		# Copying of DLL files is not needed anymore because of delay load library
		
		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyEmotivAPI)
	ELSE(PATH_EmotivAPI)
		MESSAGE(STATUS "  FAILED to find Emotiv API")
	ENDIF(PATH_EmotivAPI)
ENDIF(WIN32)
