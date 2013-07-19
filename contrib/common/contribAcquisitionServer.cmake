INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/contrib/common")

SET(ADDITIONAL_PATH "${CMAKE_SOURCE_DIR}/contrib/plugins/server-extensions/external-stimulations/")
INCLUDE_DIRECTORIES(${ADDITIONAL_PATH})
FILE(GLOB_RECURSE additional_source_files ${ADDITIONAL_PATH}/*.cpp ${ADDITIONAL_PATH}/*.h)
SET(source_files "${source_files};${additional_source_files}")


FUNCTION(OV_ADD_CONTRIB_DRIVER DRIVER_PATH)

	SET(ADDITIONAL_PATH ${DRIVER_PATH})
	INCLUDE_DIRECTORIES(${ADDITIONAL_PATH}/src)
	FILE(GLOB_RECURSE additional_source_files ${ADDITIONAL_PATH}/src/*.cpp ${ADDITIONAL_PATH}/src/*.h)
	SET(source_files "${source_files};${additional_source_files}" PARENT_SCOPE)

	MESSAGE(STATUS "DO I EXIST: ${ADDITIONAL_PATH}/share/")
	IF(EXISTS "${ADDITIONAL_PATH}/share/")
		MESSAGE(STATUS "I EXIST: ${ADDITIONAL_PATH}/share/")
		INSTALL(DIRECTORY "${ADDITIONAL_PATH}/share/" DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/openvibe/applications/acquisition-server/" PATTERN ".svn" EXCLUDE)
	ENDIF(EXISTS "${ADDITIONAL_PATH}/share/")

ENDFUNCTION(OV_ADD_CONTRIB_DRIVER)

OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/contrib/plugins/server-drivers/brainmaster-discovery")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/contrib/plugins/server-drivers/brainproducts-actichamp")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/contrib/plugins/server-drivers/brainproducts-brainvisionrecorder")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/contrib/plugins/server-drivers/ctfvsm-meg")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/contrib/plugins/server-drivers/field-trip-protocol")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/contrib/plugins/server-drivers/gtec-gipsa")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/contrib/plugins/server-drivers/gtec-gusbamp")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/contrib/plugins/server-drivers/mitsarEEG202A")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/contrib/plugins/server-drivers/openal-mono16bit-audiocapture")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/contrib/plugins/server-drivers/openeeg-modulareeg")
