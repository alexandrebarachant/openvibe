This directory contains scripts and configuration files for automatic checkout sources, building and potentially run tests of openViBe. This files need to be placed in $HOME directory of test account.
After every test a Cdash rapport is emitted to OpenViBe Cdash server :

http://cdash.inria.fr/CDash/index.php?project=OpenViBe

Dependencies: 
------------

to run these scripts, you need ctest (cmake suit), sh-utils (coreutils on windows cygwin or gnuwin32), svn command-line tools and a c++ compiler (C++ Visual Studio 2010 on windows) 

On Linux fedora you need to install :
yum install cmake svn redhat-lsb gcc-c++ expect

SUDOER on Linux :
------
On linux, you need to have an account with sodoers privileges without password

WARNING 1 : be careful, this not good for your personal computer or server, it must be reserved for unsafe slaves machines used to run automatic tests

launch:
VISUAL=/usr/bin/vi visudo
then put this kind of line and the end of the file:
user ALL=(ALL) NOPASSWD: ALL


WARNING 2 : execution of test change de root password in Fedora distributions to : openvibe 




Get tests scripts:
-----------------
svn checkout svn://scm.gforge.inria.fr/svnroot/openvibe/trunk/test
cd test



Run test:
--------

 - for a Nightly test:

 ctest -VV -S openVibeTests.cmake,Nightly 

 - for a experimental test:

 ctest -VV -S openVibeTests.cmake,Experimental

 - for a continuous building test:

 ctest -VV -S openVibeTests.cmake,Continuous 

For a local test in your machine with not cdash rapport :

 ctest 


How to add new test:
-------------------

You can add new test using DartTestFile.txt placed in test directory from a specific module.

For example, designer specific tests will be placed in DartTestFile.txt in trunk/openvibe-applications/designer/trunc/test/
And specific test for samples plugin will be placed in DartTestFile.txt in trunk/openvibe-plugins/samples/trunc/test/
To be sure that test are executed, you need to add this subdirectories in file trunk/test/CTestfile.cmake.
For last examples:
...
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/openvibe-applications/designer/trunc/test")
SUBDIRS("${CTEST_SOURCE_DIRECTORY}/test/trunk/openvibe-plugins/samples/trunc/test")
...



There is an exemple of test using Sinus Oscillator in trunk/openvibe-plugins/samples/trunc/test/DartTestFile.txt :

SET(TEST_NAME SinusOscillator)
SET(SCENARIO_TO_TEST "${TEST_NAME}.xml")

IF(WIN32)
	SET(EXT cmd)
	SET(OS_FLAGS "--no-pause")
ELSE(WIN32)
	SET(EXT sh)
	SET(OS_FLAGS "")
ENDIF(WIN32)

ADD_TEST(run_${TEST_NAME} "$ENV{OV_BINARY_PATH}/ov-designer.${EXT}" ${OS_FLAGS} "--no-gui" "--play" ${SCENARIO_TO_TEST})
ADD_TEST(comparator_${TEST_NAME} "diff" "${TEST_NAME}.csv" "${TEST_NAME}.ref.csv")

## add some properties that help to debug 
SET_TESTS_PROPERTIES(run_${TEST_NAME} PROPERTIES ATTACHED_FILES_ON_FAIL "ENV${HOME}.config/openvibe/log/openvibe-designer.log"}
SET_TESTS_PROPERTIES(comparator_${TEST_NAME} PROPERTIES ATTACHED_FILES_ON_FAIL "${TEST_NAME}.csv"}
SET_TESTS_PROPERTIES(comparator_${TEST_NAME} PROPERTIES DEPENDS run_${TEST_NAME}}


Some Remarks :

    This test run designer with no GUI, but in Linux it still need a X11 context. So you need to be sure that test can access to a X server. That will be do by a automatic start-up of "Xorg -ac&" command to ensure that X server is launched at test moment. 
    That's why we need to define DISPLAY environment variable before launch test.
    This test run in path : trunk/openvibe-plugins/samples/trunc/test/.
    This test works with a specific scenario that content automatic stop set to a 1 second (using Clock stimulator+Player Controller),
    This test produce a CSV file output that contents output of Sinus oscillator for 1 second (using CSV File Writer)
    Second test is a "white box" test that compare current output signal file with a reference file using "diff" program as comparator. Signal reference file was obtained with the same scenario "a day when all work fine" (actually, non-regression test) 
    This DartTestFile.txt will be adapted to others boxes to produce same type of tests.
