# This file specifies the test which should be executed in

#ctest_test(     BUILD  "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)

# The file should be placed in the binary directory ${CTEST_BINARY_DIRECTORY}.


#Each test consists of:

#    The unique name of the test ( eg.: testname1 )
#    The full path to the executable of the test ( eg.: "$ENV{HOME}/bin/TEST_EXECUTABLE_1.sh" )
#    A List of arguments to the executable ( eg.: "ARGUMENT_1" "ARGUMENT_2" etc. ) 


# basic test (just for sample) check that binary directory is readable 
ADD_TEST(LS_BINARY_PATH "ls" "-all")

## -- Other Tests : Place a file named DartTestfile.txt in this path with tests.
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe/trunc/test/")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe/")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}openvibe-kernel-omk/")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}openvibe-modules/")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}openvibe-toolkit/")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}openvibe-plugins/")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}openvibe-scenarios/")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}openvibe-applications/")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}openvibe-externals/")



 
