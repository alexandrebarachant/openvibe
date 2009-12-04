# ---------------------------------
# Finds VAmp FirstAmp library
# Adds library to target
# Adds include path
# ---------------------------------
IF(WIN32)
	FIND_PATH(PATH_USBFirstAmpAPI FirstAmp.h PATHS "C:/Program Files/FaSDK" $ENV{OpenViBE_dependencies})
	IF(PATH_USBFirstAmpAPI)
		MESSAGE(STATUS "  Found FirstAmp API...")
		INCLUDE_DIRECTORIES(${PATH_USBFirstAmpAPI})
		FIND_LIBRARY(LIB_USBFirstAmpAPI FirstAmp PATHS ${PATH_USBFirstAmpAPI} )
		IF(LIB_USBFirstAmpAPI)
			MESSAGE(STATUS "    [  OK  ] lib ${LIB_USBFirstAmpAPI}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_USBFirstAmpAPI} )
		ELSE(LIB_USBFirstAmpAPI)
			MESSAGE(STATUS "    [FAILED] lib FirstAmp")
		ENDIF(LIB_USBFirstAmpAPI)

		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyUSBFirstAmpAPI)
	ELSE(PATH_USBFirstAmpAPI)
		MESSAGE(STATUS "  FAILED to find FirstAmp API - cmake looked in 'C:/Program Files/FaSDK' and '$ENV{OpenViBE_dependencies}'")
	ENDIF(PATH_USBFirstAmpAPI)
ENDIF(WIN32)