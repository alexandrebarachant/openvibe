INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/../contrib/common")

SET(ADDITIONAL_PATH "${CMAKE_SOURCE_DIR}/../contrib/gipsa-lab/plugins/server-extensions/external-stimulations/")
INCLUDE_DIRECTORIES(${ADDITIONAL_PATH})
FILE(GLOB_RECURSE additional_source_files ${ADDITIONAL_PATH}/*.cpp ${ADDITIONAL_PATH}/*.h)
SET(source_files "${source_files};${additional_source_files}")

INCLUDE("FindThirdPartyGUSBampCAPI")


FUNCTION(OV_ADD_CONTRIB_DRIVER DRIVER_PATH)

	SET(ADDITIONAL_PATH ${DRIVER_PATH})
	INCLUDE_DIRECTORIES(${ADDITIONAL_PATH}/src)
	FILE(GLOB_RECURSE additional_source_files ${ADDITIONAL_PATH}/src/*.cpp ${ADDITIONAL_PATH}/src/*.h)
	SET(source_files "${source_files};${additional_source_files}" PARENT_SCOPE)

ENDFUNCTION(OV_ADD_CONTRIB_DRIVER)

OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/../contrib/various/plugins/server-drivers/gtec-gusbamp")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/../contrib/inserm/plugins/server-drivers/ctfvsm-meg")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/../contrib/inserm/plugins/server-drivers/brainproducts-brainvisionrecorder")
OV_ADD_CONTRIB_DRIVER("${CMAKE_SOURCE_DIR}/../contrib/gipsa-lab/plugins/server-drivers/mitsarEEG202A")
