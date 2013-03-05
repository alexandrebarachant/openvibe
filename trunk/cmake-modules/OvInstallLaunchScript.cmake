#
# Creates launch script from a common OpenViBE template (in "cmake-modules/launchers/"), suitable for most apps
#
# The mandatory 1st argument SCRIPT_PREFIX specifies what the resulting script is called. A platform specific postfix will be added.
# The optional 2nd argument ARGV1 specifies some extra argument or switch that is given to the launched executable by the script
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
	
	CONFIGURE_FILE(${CMAKE_MODULE_PATH}/launchers/ov-launcher${SCRIPT_POSTFIX}-base ${CMAKE_CURRENT_BINARY_DIR}/${SCRIPT_NAME} @ONLY)
	
	INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${SCRIPT_NAME} DESTINATION .)	
ENDFUNCTION(OV_INSTALL_LAUNCH_SCRIPT)
