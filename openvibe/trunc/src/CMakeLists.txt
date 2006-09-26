FILE(GLOB_RECURSE source_files *.cpp)
FILE(GLOB_RECURSE all_files *.*)

ADD_LIBRARY(${PROJECT_NAME}-static ${source_files})

ADD_DEFINITIONS(-DOV_Exports)
ADD_DEFINITIONS(-DOV_Shared)

ADD_LIBRARY(${PROJECT_NAME}-dynamic SHARED ${source_files})

SET_TARGET_PROPERTIES(${PROJECT_NAME}-dynamic PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR})

# ---------------------------------
# Finds standard library dl
# Adds library to target
# Adds include path
# ---------------------------------
IF(UNIX)
	FIND_LIBRARY(LIB_STANDARD_MODULE_DL dl)
	IF(LIB_STANDARD_MODULE_DL)
		MESSAGE(STATUS "  Found dl...")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_STANDARD_MODULE_DL})
	ELSE(LIB_STANDARD_MODULE_PTHREAD)
		MESSAGE(STATUS "  FAILED to find dl...")
	ENDIF(LIB_STANDARD_MODULE_DL)
ENDIF(UNIX)

# ---------------------------------
# Copies all files to include path
# and then remove not .h files
# ---------------------------------

# Copies all the source files

STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ ";" all_files ${all_files})
ADD_CUSTOM_COMMAND(
	TARGET ${PROJECT_NAME}-static
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS  -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_OUTPUT_PATH})

# Lists .h files to remove at clean stage
# Lists not .h files to remove immediatly

SET(remove_files "")
FOREACH(current_file ${all_files})
#	GET_FILENAME_COMPONENT(current_file_path ${INCLUDE_OUTPUT_PATH}/${current_file} PATH)
	IF(${current_file} MATCHES ".*\\.h$")
		SET(remove_files "${remove_files};${INCLUDE_OUTPUT_PATH}/${current_file};${current_file_path}")
#		MESSAGE(STATUS "${current_file} will be deleted later !")
	ELSE(${current_file} MATCHES ".*\\.h$")
		ADD_CUSTOM_COMMAND(
			TARGET ${PROJECT_NAME}-static
			POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E remove ${INCLUDE_OUTPUT_PATH}/${current_file})
		ADD_CUSTOM_COMMAND(
			TARGET ${PROJECT_NAME}-dynamic
			POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E remove ${INCLUDE_OUTPUT_PATH}/${current_file})
#		MESSAGE(STATUS "${current_file} is deleted now !")
	ENDIF(${current_file} MATCHES ".*\\.h$")
ENDFOREACH(current_file)
SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${remove_files}")
