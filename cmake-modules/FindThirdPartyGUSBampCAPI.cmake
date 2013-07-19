# ---------------------------------
# Finds GUSBampCAPI
# Adds library to target
# Adds include path
# ---------------------------------
IF(WIN32)
	FIND_PATH(PATH_GUSBampCAPI gUSBamp.h PATHS 
		"C:/Program Files/gtec/gUSBampCAPI/API" 
		"C:/Program Files (x86)/gtec/gUSBampCAPI/API" 
		"C:/Program Files/gtec/gUSBampCAPI/API/Win32" 
		"C:/Program Files (x86)/gtec/gUSBampCAPI/API/Win32"
		${OV_CUSTOM_DEPENDENCIES_PATH})
	IF(PATH_GUSBampCAPI)
		MESSAGE(STATUS "  Found gUSBampCAPI...")
		INCLUDE_DIRECTORIES(${PATH_GUSBampCAPI})
		FIND_LIBRARY(LIB_GUSBampCAPI gUSBamp PATHS ${PATH_GUSBampCAPI})
		IF(LIB_GUSBampCAPI)
			MESSAGE(STATUS "    [  OK  ] lib ${LIB_GUSBampCAPI}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_GUSBampCAPI} )
		ELSE(LIB_GUSBampCAPI)
			MESSAGE(STATUS "    [FAILED] lib gUSBamp")
		ENDIF(LIB_GUSBampCAPI)

		# Copy the DLL file at install
		INSTALL(PROGRAMS "${PATH_GUSBampCAPI}/gUSBamp.dll" DESTINATION "bin")

		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyGUSBampCAPI)
	ELSE(PATH_GUSBampCAPI)
		MESSAGE(STATUS "  FAILED to find gUSBampCAPI")
	ENDIF(PATH_GUSBampCAPI)
ENDIF(WIN32)
