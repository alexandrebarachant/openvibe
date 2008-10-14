ADD_CUSTOM_TARGET(${PROJECT_NAME} ALL)

FIND_PROGRAM(doxygen_bin "doxygen" PATHS ${OpenViBE_dependencies}/bin NO_DEFAULT_PATH)
FIND_PROGRAM(doxygen_bin "doxygen" PATHS ${OpenViBE_dependencies}/bin)
IF(doxygen_bin)
	MESSAGE(STATUS "  Found doxygen...")

	SET(ov_doxy_input "")
	MESSAGE(STATUS "  Found candidate doc directories...")
	STRING(REPLACE "\n" ";" OpenViBE_build_order_spaces "$ENV{OpenViBE_build_order}")
	FOREACH(current_project ${OpenViBE_build_order_spaces})
		SET(current_project_src "${current_project}/src")
		SET(ov_doxy_input "${ov_doxy_input} \"${current_project_src}\"")
		MESSAGE(STATUS "    [  OK  ] Candidate doc directory ${current_project_src}")

		FILE(GLOB_RECURSE resource_files_tmp "${current_project_src}/*.png" "${current_project_src}/*.css")
		SET(resource_files ${resource_files} ${resource_files_tmp})
	ENDFOREACH(current_project)

	ADD_CUSTOM_COMMAND(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${doxygen_bin} ${PROJECT_SOURCE_DIR}/src/doxyfile
		WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/doc"
		COMMENT "      --->   Running doxygen..."
		VERBATIM)

	IF(resource_files)
		MESSAGE(STATUS "  Found resources...")
		FOREACH(current_resource ${resource_files})
			GET_FILENAME_COMPONENT(current_resource_stripped ${current_resource} NAME)
			MESSAGE(STATUS "    [  OK  ] Resource file ${current_resource}")
			ADD_CUSTOM_COMMAND(
				TARGET ${PROJECT_NAME}
				POST_BUILD
				COMMAND ${CMAKE_COMMAND}
				ARGS -E copy "${current_resource}" "${PROJECT_SOURCE_DIR}/doc/html/${current_resource_stripped}"
				COMMENT "      --->   Copying resource file ${current_resource_stripped}..."
			VERBATIM)
		ENDFOREACH(current_resource)
	ENDIF(resource_files)

	SET(ov_doxy_strip_from_path ${ov_doxy_input})
	SET(ov_doxy_version ${PROJECT_VERSION})

	CONFIGURE_FILE(
		doxyfile-skeleton
		${PROJECT_SOURCE_DIR}/src/doxyfile
		@ONLY)
ELSE(doxygen_bin)
	MESSAGE(STATUS "  FAILED to find doxygen...")
ENDIF(doxygen_bin)
