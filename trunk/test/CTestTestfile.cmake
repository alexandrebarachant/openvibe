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


SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/acquisition/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/turbofieldtrip/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/tools/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/vrpn/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/stimulation/branches/wip-emaby/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/stimulation/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/feature-extraction/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/classification-gpl/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/signal-processing/branches/wip-emaby/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/signal-processing/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/simple-visualisation/branches/wip-lbonnet-p300/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/simple-visualisation/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/file-io/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/samples/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/examples/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/classification/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/stream-codecs/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/python/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/streaming/branches/wip-emaby/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/streaming/branches/wip-emaby/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/streaming/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/signal-processing-gpl/branches/wip-emaby/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/signal-processing-gpl/branches/wip-acellard-ARFeatures/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/signal-processing-gpl/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/matlab/branches/wip-lbonnet-matlabdll/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-plugins/matlab/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/id-generator/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/external-stimulation-connection-example/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/designer/tags/wip-omk4/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/designer/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/ssvep-demo/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/skeleton-generator/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/vr-demo/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/vrpn-simulator/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/plugin-inspector/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/acquisition-server/branches/wip-lbonnet-micromed/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/acquisition-server/branches/wip-lbonnet-gtec/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/acquisition-server/branches/wip-emaby/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/acquisition-server/branches/wip-jfruitet/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/acquisition-server/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-documentation/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe/tags/wip-omk4/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-toolkit/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-scenarios/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-kernel-omk/tags/wip-omk4/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-kernel-omk/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-modules/xml/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-modules/automaton/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-modules/stream/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-modules/ebml/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-modules/socket/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-modules/fs/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-modules/system/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/scripts/software/tmp/vrpn/java_vrpn/test")


