# ---------------------------------
# Finds Neurosky ThinkGear library
# Adds library to target
# Adds include path
# ---------------------------------
IF(WIN32)
	FIND_PATH(PATH_ThinkGearAPIOld thinkgear.h PATHS "C:/Program Files/MindSet Development Tools/tgcd/win32"  ${OV_CUSTOM_DEPENDENCIES_PATH})
	IF(PATH_ThinkGearAPIOld)
		MESSAGE(STATUS "  Found a ThinkGear API, but the version seems inferior to 2.1.")
	ENDIF(PATH_ThinkGearAPIOld)
	
	FIND_PATH(PATH_ThinkGearAPI thinkgear.h PATHS "C:/Program Files/MindSet Development Tools/ThinkGear Communications Driver/win32" "C:/Program Files (x86)/MindSet Development Tools/ThinkGear Communications Driver/win32" ${OV_CUSTOM_DEPENDENCIES_PATH})
	IF(PATH_ThinkGearAPI)
		MESSAGE(STATUS "  Found ThinkGear API...")
		INCLUDE_DIRECTORIES(${PATH_ThinkGearAPI})
		FIND_LIBRARY(LIB_ThinkGearAPI thinkgear PATHS ${PATH_ThinkGearAPI} )
		IF(LIB_ThinkGearAPI)
			MESSAGE(STATUS "    [  OK  ] lib ${LIB_ThinkGearAPI}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_ThinkGearAPI} )
		ELSE(LIB_ThinkGearAPI)
			MESSAGE(STATUS "    [FAILED] lib thinkgear")
		ENDIF(LIB_ThinkGearAPI)

		# Copy the DLL file at install
		INSTALL(PROGRAMS "${PATH_ThinkGearAPI}/thinkgear.dll" DESTINATION "bin")

		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyThinkGearAPI)
	ELSE(PATH_ThinkGearAPI)
		MESSAGE(STATUS "  FAILED to find a valid ThinkGear API - cmake looked in 'C:/Program Files/MindSet Development Tools/ThinkGear Communications Driver/win32' and 'C:/Program Files (x86)/MindSet Development Tools/ThinkGear Communications Driver/win32'")
	ENDIF(PATH_ThinkGearAPI)
ENDIF(WIN32)

IF(UNIX)
	MESSAGE(STATUS "The ThinkGear API for Neurosky MindSet is only available on windows.")
ENDIF(UNIX)
