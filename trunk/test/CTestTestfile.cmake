# This file specifies the test which should be executed in

#ctest_test(     BUILD  "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)

# The file should be placed in the binary directory ${CTEST_BINARY_DIRECTORY}.


#Each test consists of:

#    The unique name of the test ( eg.: testname1 )
#    The full path to the executable of the test ( eg.: "$ENV{HOME}/bin/TEST_EXECUTABLE_1.sh" )
#    A List of arguments to the executable ( eg.: "ARGUMENT_1" "ARGUMENT_2" etc. ) 

ADD_TEST(testname1 "ls" "-all")
#ADD_TEST(testname2 "$ENV{HOME}/bin/TEST_EXECUTABLE_2.sh")


 
