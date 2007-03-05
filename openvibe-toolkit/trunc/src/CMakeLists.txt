FILE(GLOB_RECURSE source_files *.cpp)
FILE(GLOB_RECURSE header_files *.h)

ADD_LIBRARY(${PROJECT_NAME}-static ${source_files})

ADD_DEFINITIONS(-DOVTK_Exports)
ADD_DEFINITIONS(-DOVTK_Shared)
ADD_LIBRARY(${PROJECT_NAME}-dynamic SHARED ${source_files})

SET_TARGET_PROPERTIES(${PROJECT_NAME}-dynamic PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR})

ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-module-EBML-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-module-EBML-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-module-FS-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-module-FS-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-module-Socket-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-module-Socket-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-module-System-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-module-System-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-module-XML-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-dynamic OpenViBE-module-XML-static)

ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-module-EBML-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-module-EBML-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-module-FS-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-module-FS-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-module-Socket-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-module-Socket-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-module-System-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-module-System-static)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-module-XML-dynamic)
ADD_DEPENDENCIES(${PROJECT_NAME}-static OpenViBE-module-XML-static)

# ---------------------------------
# Finds module EBML
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PATH(PATH_OPENVIBE_MODULES_EBML include/ebml/defines.h PATHS $ENV{OpenViBE_modules}/ebml $ENV{OpenViBE_module_ebml})
IF(PATH_OPENVIBE_MODULES_EBML)
	MESSAGE(STATUS "  Found OpenViBE module ebml...")
	INCLUDE_DIRECTORIES(${PATH_OPENVIBE_MODULES_EBML}/include)
	FIND_LIBRARY(LIB_OPENVIBE_MODULE_EBML OpenViBE-module-EBML-${MODULE_LINKING} PATHS ${PATH_OPENVIBE_MODULES_EBML}/lib)
	IF(LIB_OPENVIBE_MODULE_EBML)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_OPENVIBE_MODULE_EBML}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_OPENVIBE_MODULE_EBML})
	ELSE(LIB_OPENVIBE_MODULE_EBML)
		MESSAGE(STATUS "    [FAILED] lib OpenViBE-module-EBML-${MODULE_LINKING}")
	ENDIF(LIB_OPENVIBE_MODULE_EBML)
ELSE(PATH_OPENVIBE_MODULES_EBML)
	MESSAGE(STATUS "  FAILED to find OpenViBE module ebml...")
ENDIF(PATH_OPENVIBE_MODULES_EBML)

# ---------------------------------
# Finds OpenViBE
# Adds library to target
# Adds include path
# ---------------------------------
FIND_PATH(PATH_OPENVIBE include/openvibe/ov_all.h PATHS $ENV{OpenViBE})
IF(PATH_OPENVIBE)
	MESSAGE(STATUS "  Found OpenViBE...")
	INCLUDE_DIRECTORIES(${PATH_OPENVIBE}/include)
	FIND_LIBRARY(LIB_OPENVIBE OpenViBE-${OPENVIBE_LINKING} PATH ${PATH_OPENVIBE}/lib)
	IF(LIB_OPENVIBE)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_OPENVIBE}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_OPENVIBE})
	ELSE(LIB_OPENVIBE)
		MESSAGE(STATUS "    [FAILED] lib OpenViBE-${OPENVIBE_LINKING}")
	ENDIF(LIB_OPENVIBE)
ELSE(PATH_OPENVIBE)
	MESSAGE(STATUS "  FAILED to find OpenViBE...")
ENDIF(PATH_OPENVIBE)

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
		ARGS  -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${current_header}" "${INCLUDE_OUTPUT_PATH}/${current_header}")
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
