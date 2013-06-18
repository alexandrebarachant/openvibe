# This file specifies the test which should be executed

# The file should be placed in the binary directory ${CTEST_BINARY_DIRECTORY}.

#Each test consists of:

#    The unique name of the test ( eg.: testname1 )
#    The full path to the executable of the test ( eg.: "$ENV{HOME}/bin/TEST_EXECUTABLE_1.sh" )
#    A List of arguments to the executable ( eg.: "ARGUMENT_1" "ARGUMENT_2" etc. ) 
	
# basic test (just for sample) check that binary directory is readable 
#ADD_TEST(LS_BINARY_PATH "ls" "-all")
#ADD_TEST(PWD_BINARY_PATH "pwd")



## -- Other Tests : Place a file named DartTestfile.txt in path with tests.

# ${TEST_LOCAL} is repleased by "FALSE" only by automatic test call, else {TEST_LOCAL} is not defined
SET(LOCAL ${TEST_LOCAL})
IF(NOT (LOCAL MATCHES "FALSE"))
	IF(WIN32)
		# triky way to get absolute path for ../. directory in windows with cygwin
		# todo: try to find a cmake way to get this for simple call to ctest in OV_ROOT_DIR\test directory in the aim to run all test
		exec_program("cygpath" ARGS "-a -w ../." OUTPUT_VARIABLE "OV_ROOT_DIR")
	ELSE(WIN32)
		set(OV_ROOT_DIR              "$ENV{PWD}/..")
	ENDIF(WIN32)
	
	set(CTEST_SOURCE_DIRECTORY		"${OV_ROOT_DIR}")
	set(ENV{OV_BINARY_PATH} "${OV_ROOT_DIR}/dist")
	message("running local test here= $ENV{OV_BINARY_PATH}")
ENDIF(NOT (LOCAL MATCHES "FALSE"))

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()


SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/acquisition/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/turbofieldtrip/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/tools/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/vrpn/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/stimulation/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/feature-extraction/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/classification-gpl/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/signal-processing/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/simple-visualisation/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/file-io/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/samples/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/examples/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/classification/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/stream-codecs/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/python/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/streaming/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/signal-processing-gpl/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/plugins/processing/matlab/test")
# SUBDIRS("${CTEST_SOURCE_DIRECTORY}/applications/external-stimulation-connection-example/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/applications/platform/acquisition-server/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/applications/platform/designer/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/applications/demos/ssvep-demo/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/applications/demos/vr-demo/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/applications/developer-tools/id-generator/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/applications/developer-tools/skeleton-generator/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/applications/developer-tools/vrpn-simulator/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/applications/developer-tools/plugin-inspector/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/documentation/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/toolkit/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/scenarios/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/kernel/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/modules/xml/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/modules/automaton/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/modules/stream/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/modules/ebml/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/modules/socket/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/modules/fs/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/modules/system/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/scripts/software/tmp/vrpn/java_vrpn/test")


