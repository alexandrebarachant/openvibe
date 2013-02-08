#
# Creates launch script from a common OpenViBE template (in "cmake-modules/launchers/"), suitable for most apps
#
# The optional second argument ARGV1 specifies some extra argument to the launched command
# The optional second argument ARGV2 specifies the working directory for the launched command. If unspecified, its "bin".
#
FUNCTION(OV_INSTALL_LAUNCH_SCRIPT SCRIPT_PREFIX)
	IF(WIN32)
		SET(SCRIPT_POSTFIX ".cmd")
	ENDIF(WIN32)
	IF(UNIX)
		SET(SCRIPT_POSTFIX ".sh")
	ENDIF(UNIX)
	
	# Extract the filename of the project executable, the variable in the script base will be replaced with it by CONFIGURE_FILE()
	GET_TARGET_PROPERTY(TMP_PROJECT_TARGET_PATH ${PROJECT_NAME}-dynamic LOCATION)	
	GET_FILENAME_COMPONENT(OV_CMD_EXECUTABLE ${TMP_PROJECT_TARGET_PATH} NAME)
	SET(SCRIPT_NAME ${SCRIPT_PREFIX}${SCRIPT_POSTFIX})
	
	# OV_CMD_ARGS, if present in the -base file, will be replaced by contents of ARGV1 (later by CONFIGURE_FILE)
	IF(ARGC GREATER 1)
		SET(OV_CMD_ARGS ${ARGV1})
	ELSE(ARGC GREATER 1)
		SET(OV_CMD_ARGS "")
	ENDIF(ARGC GREATER 1)

	# OV_CMD_WORKING_DIR, if present in the -base file, will be replaced by contents of ARGV2 (later by CONFIGURE_FILE)
	IF(ARGC GREATER 2)
		IF(WIN32) 
			STRING(REPLACE "/" "\\" OV_CMD_WORKING_DIR ${ARGV2})
		ELSE(WIN32)
			SET(OV_CMD_WORKING_DIR ${ARGV2})
		ENDIF(WIN32)
	#	MESSAGE(STATUS "Working dir is ${OV_CMD_WORKING_DIR}")
	ELSE(ARGC GREATER 2)
		SET(OV_CMD_WORKING_DIR "bin")
	ENDIF(ARGC GREATER 2)	
	
	CONFIGURE_FILE(${CMAKE_MODULE_PATH}/launchers/ov-launcher${SCRIPT_POSTFIX}-base ${CMAKE_CURRENT_BINARY_DIR}/${SCRIPT_NAME} @ONLY)
	
#	IF(ARGC GREATER 2)
#		CONFIGURE_FILE(${ARGV2}/${SCRIPT_NAME}-base ${CMAKE_CURRENT_BINARY_DIR}/${SCRIPT_NAME})	
#	ELSE(ARGC GREATER 2)
#		CONFIGURE_FILE(${CMAKE_MODULE_PATH}/launchers/ov-launcher${SCRIPT_POSTFIX}-base ${CMAKE_CURRENT_BINARY_DIR}/${SCRIPT_NAME})
#	ENDIF(ARGC GREATER 2)
	
	INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${SCRIPT_NAME} DESTINATION .)	
ENDFUNCTION(OV_INSTALL_LAUNCH_SCRIPT)
