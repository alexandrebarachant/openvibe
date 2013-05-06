#
# This function is used to add a project to the build in the case where the current directory only contains 
# 'branches', 'tags' and 'trunk' directories of a single project. The project will be added using the 
# branch specified (if any) in the root CMakeList.txt or trunk otherwise.
#
# The branch variable name that is checked is made up from ${CURRENT_BRANCH_PREFIX}_${DIRNAMEUPPER}. 
#
# The script also adds the directory to the global list of projects.
#

FUNCTION(OV_ADD_SINGLE_PROJECT CURRENT_BRANCH_PREFIX)

	SET(BRANCH_VARIABLE_NAME "${CURRENT_BRANCH_PREFIX}")

	# MESSAGE(STATUS "Parts ${CURRENT_BRANCH_PREFIX} Now ${BRANCH_VARIABLE_NAME}")
	# MESSAGE(STATUS "Value ${${BRANCH_VARIABLE_NAME}}")

	SET(SUBDIR "SUBDIR-NOTFOUND")

	# MESSAGE(STATUS "Checking branch for ${BRANCH_VARIABLE_NAME}")	
			
	IF(${BRANCH_VARIABLE_NAME})	
		SET(BRANCH ${${BRANCH_VARIABLE_NAME}})
		IF("${BRANCH}" STREQUAL "__SKIPME") 
			MESSAGE(STATUS "Note: ${BRANCH_VARIABLE_NAME} has been disabled by setting it to __SKIPME")
		ELSE("${BRANCH}" STREQUAL "__SKIPME")			
			SET(SUBDIR "${BRANCH}")
		ENDIF("${BRANCH}" STREQUAL "__SKIPME")
	ELSE(${BRANCH_VARIABLE_NAME})
	#	MESSAGE(STATUS "Going for trunk...")
		SET(SUBDIR "${OV_TRUNK}")
	ENDIF(${BRANCH_VARIABLE_NAME})
			
	IF(SUBDIR)
		# MESSAGE(STATUS "Inserting folder ${SUBDIR}")

		# Add the dir to be parsed for documentation later. We need to do this before adding subdir, in case the subdir is the actual docs dir.
		GET_PROPERTY(OV_TMP GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS)
		SET(OV_TMP "${OV_TMP};${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}")
		SET_PROPERTY(GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS ${OV_TMP})

#		MESSAGE(STATUS "BDIR ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}")
		GET_PROPERTY(OV_TMP GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS_BUILD_DIR)
		SET(OV_TMP "${OV_TMP};${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}")
		SET_PROPERTY(GLOBAL PROPERTY OV_PROP_CURRENT_PROJECTS_BUILD_DIR ${OV_TMP})


		ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR})
				
	ENDIF(SUBDIR)
			
ENDFUNCTION(OV_ADD_SINGLE_PROJECT)
