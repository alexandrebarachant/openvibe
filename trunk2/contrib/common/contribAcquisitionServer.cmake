INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/../contrib/common")

SET(ADDITIONAL_PATH "${CMAKE_SOURCE_DIR}/../contrib/gipsa-lab/plugins/server-extensions/external-stimulations/")

INCLUDE_DIRECTORIES(${ADDITIONAL_PATH})

FILE(GLOB_RECURSE additional_source_files ${ADDITIONAL_PATH}/*.cpp ${ADDITIONAL_PATH}/*.h)

SET(source_files "${source_files};${additional_source_files}")
