FILE(GLOB_RECURSE source_files *.cpp)
FILE(GLOB_RECURSE header_files *.h)

ADD_LIBRARY(${PROJECT_NAME}-static STATIC ${source_files})
ADD_LIBRARY(${PROJECT_NAME}-dynamic SHARED ${source_files})

SET_TARGET_PROPERTIES(${PROJECT_NAME}-dynamic PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	COMPILE_FLAGS "-DOVTK_Exports -DOVTK_Shared")

INCLUDE("FindOpenViBE")
INCLUDE("FindOpenViBEModuleEBML")
INCLUDE("FindOpenViBEModuleSystem")

# -----------------------------
# Copies all header files to
# include path and mark copied
# files to be deleted at clean
# stage.
# NOTE: created directories
# won't be removed
# -----------------------------
SET(created_directories "")
SET(created_files "")
STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ ";" header_files ${header_files})
FOREACH(current_header ${header_files})
	GET_FILENAME_COMPONENT(current_header_path ${current_header} PATH)
	IF(NOT EXISTS ${INCLUDE_OUTPUT_PATH}/${current_header_path})
		IF(created_directories MATCHES "${current_header_path}")
		ELSE(created_directories MATCHES "${current_header_path}")
#			ADD_CUSTOM_COMMAND(
#				TARGET ${PROJECT_NAME}-static
#				POST_BUILD
#				COMMAND mkdir ${current_header_path})
			SET(created_directories ${created_directories} ${current_header_path})
#			MESSAGE(STATUS "Marked to create at post build stage ${current_header_path}")
		ENDIF(created_directories MATCHES "${current_header_path}")
	ENDIF(NOT EXISTS ${INCLUDE_OUTPUT_PATH}/${current_header_path})
	ADD_CUSTOM_COMMAND(
		TARGET ${PROJECT_NAME}-static
		POST_BUILD
		COMMAND ${CMAKE_COMMAND}
		ARGS  -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${current_header}" "${INCLUDE_OUTPUT_PATH}/${current_header}")
	SET(created_files ${created_files} ${current_header})
#	MESSAGE(STATUS "Marked to copy at post build stage ${current_header}")
ENDFOREACH(current_header)
SET(remove "")
FOREACH(created_files_remove ${created_files})
	SET(remove ${remove};${INCLUDE_OUTPUT_PATH}/${created_files_remove})
ENDFOREACH(created_files_remove)
FOREACH(created_directories_remove ${created_directories})
	SET(remove ${remove};${INCLUDE_OUTPUT_PATH}/${created_directories_remove})
ENDFOREACH(created_directories_remove)
SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${remove}")
# MESSAGE(STATUS "Marked to delete at clean stage ${remove}")
