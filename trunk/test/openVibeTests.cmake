CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

# REMOVE if all test is passed 
# set(CMAKE_LEGACY_CYGWIN_WIN32 0)  # Remove when CMake >= 2.8.4 is required
#------------------------

# to exec test ctest -S thisfile
# -----------------------------------------------------------  
# -- Get environment
# -----------------------------------------------------------  

## -- Set hostname
## --------------------------
find_program(HOSTNAME_CMD NAMES hostname)
exec_program(${HOSTNAME_CMD} ARGS OUTPUT_VARIABLE HOSTNAME)

set(CTEST_SITE                          "${HOSTNAME}")

## -- Set site / build name
## --------------------------

find_program(UNAME NAMES uname)
macro(getuname name flag)
  exec_program("${UNAME}" ARGS "${flag}" OUTPUT_VARIABLE "${name}")
endmacro(getuname)

IF(WIN32)
	SET(distrib "")
	SET(distrib-release "")
ELSE(WIN32)
	find_program(LSB_RELEASE NAMES lsb_release)
	exec_program("${LSB_RELEASE}" ARGS "-si" OUTPUT_VARIABLE "distrib")
	exec_program("${LSB_RELEASE}" ARGS "-sr" OUTPUT_VARIABLE "distrib-release")
ENDIF(WIN32)

getuname(osname -s)
getuname(osrel  -r)
getuname(cpu    -m)

set(CTEST_BUILD_NAME                    "${osname}_${cpu}_${distrib}${distrib-release}")

## -- SVN command
## ----------------
find_program(CTEST_SVN_COMMAND NAMES svn)

# -----------------------------------------------------------  
# -- build specific
# -----------------------------------------------------------  

SET(MODEL Nightly)
IF(${CTEST_SCRIPT_ARG} MATCHES Experimental)
  SET(MODEL Experimental)
ENDIF(${CTEST_SCRIPT_ARG} MATCHES Experimental)
IF(${CTEST_SCRIPT_ARG} MATCHES Continuous)
  SET(MODEL Continuous)
ENDIF(${CTEST_SCRIPT_ARG} MATCHES Continuous)

IF(${MODEL} MATCHES Continuous)
	set(OV_ROOT_DIR              "${CMAKE_CURRENT_SOURCE_DIR}")
ELSE(${MODEL} MATCHES Continuous)
	# create a temporary directory with very short name to host build
	exec_program("mktemp" ARGS "--tmpdir -d ov.XXX" OUTPUT_VARIABLE OV_ROOT_DIR)
ENDIF(${MODEL} MATCHES Continuous)


####

## -- SRC Dir
set(CTEST_SOURCE_DIRECTORY              "${OV_ROOT_DIR}/trunk")

## -- BIN Dir
set(CTEST_BINARY_DIRECTORY              "${OV_ROOT_DIR}/dist") 

## -- DashBoard Root
set(CTEST_DASHBOARD_ROOT                "${CMAKE_CURRENT_SOURCE_DIR}")

# -----------------------------------------------------------  
# -- commands
# -----------------------------------------------------------  

## -- Checkout command
if(NOT EXISTS "${CTEST_SOURCE_DIRECTORY}")
	set(CTEST_CHECKOUT_COMMAND     "${CTEST_SVN_COMMAND} checkout svn://scm.gforge.inria.fr/svnroot/openvibe/trunk ${CTEST_SOURCE_DIRECTORY}")
endif(NOT EXISTS "${CTEST_SOURCE_DIRECTORY}")

## -- Update Command
set(CTEST_UPDATE_COMMAND               "${CTEST_SVN_COMMAND}")


SET(NEED_CONFIGURE TRUE) 
IF(WIN32)
	## -- Configure Command
	IF(${MODEL} MATCHES Continuous)
		IF(EXISTS "${CTEST_SOURCE_DIRECTORY}/dependencies/Uninstall.exe")
			SET(NEED_CONFIGURE FALSE) 
		ENDIF(EXISTS "${CTEST_SOURCE_DIRECTORY}/dependencies/Uninstall.exe")
	ENDIF(${MODEL} MATCHES Continuous)
	set(CTEST_CONFIGURE_COMMAND            "${CTEST_SOURCE_DIRECTORY}/scripts/win32-install_dependencies.exe /S")

	## -- Build Command
	set(CTEST_BUILD_COMMAND                "cmd /C \"win32-build.cmd --no-pause\"")
ELSE(WIN32)
	## -- Configure Command
	set(CTEST_CONFIGURE_COMMAND            "${CMAKE_CURRENT_SOURCE_DIR}/InstallDependencies")
	## -- Build Command
	set(CTEST_BUILD_COMMAND                "${CTEST_SOURCE_DIRECTORY}/scripts/linux-build")
ENDIF(WIN32)


# -----------------------------------------------------------  
# -- Configure CTest
# -----------------------------------------------------------  

## set for automatic test, not for in place local test 
SET(TEST_LOCAL FALSE)

## -- CTest Config
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestConfig.cmake  ${CTEST_BINARY_DIRECTORY}/CTestConfig.cmake)

## -- CTest Custom
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake ${CTEST_BINARY_DIRECTORY}/CTestCustom.cmake)

## -- CTest Testfile
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestTestfile.cmake ${CTEST_BINARY_DIRECTORY}/CTestTestfile.cmake)


# passthrow a environnement variable to binary path to tests
SET(ENV{OV_BINARY_PATH} "${CTEST_SOURCE_DIRECTORY}/dist")
## -- read CTestCustom.cmake file
ctest_read_custom_files("${CTEST_BINARY_DIRECTORY}")

#~ SET(CTEST_PROJECT_NAME "OpenViBe")
#~ # set time for update 
#~ SET(CTEST_NIGHTLY_START_TIME "19:00:00 CEST")
#~ SET(CTEST_DROP_METHOD "http")
#~ SET(CTEST_DROP_SITE "cdash.inria.fr")
#~ SET(CTEST_DROP_LOCATION "/CDash/submit.php?project=OpenViBe")
#~ SET(CTEST_DROP_SITE_CDASH TRUE)
# -----------------------------------------------------------  
# -- Settings
# -----------------------------------------------------------  

## -- Process timeout in seconds
set(CTEST_TIMEOUT           "7200")

## -- Set output to english
set( $ENV{LC_MESSAGES}      "en_EN" )


# -----------------------------------------------------------  
# -- Run CTest
# -----------------------------------------------------------  

## -- Start
message(" -- Start dashboard ${MODEL} - ${CTEST_BUILD_NAME} --")
ctest_start(${MODEL} TRACK ${MODEL})

set(ALL_OK TRUE)
## -- Update
IF(ALL_OK)
	message(" -- Update ${MODEL} - ${CTEST_BUILD_NAME} --")
	ctest_update( SOURCE "${CTEST_SOURCE_DIRECTORY}" RETURN_VALUE res)
	IF(res EQUAL -1) 
		message(SEND_ERROR "  Update failed.")
		set(ALL_OK FALSE)
	ENDIF(res EQUAL -1)
	##  run build and test for continuous integration model only if there was a new update
	#~ IF(res EQUAL 0 AND ${MODEL} MATCHES Continuous)
		#~ RETURN()
	#~ ENDIF(res EQUAL 0 AND ${MODEL} MATCHES Continuous)
ENDIF(ALL_OK)


## -- Configure	
IF(ALL_OK)
	IF(NEED_CONFIGURE)  
		message(" -- Configure ${MODEL} - ${CTEST_BUILD_NAME} --")
		ctest_configure(BUILD  "${CTEST_SOURCE_DIRECTORY}/scripts" SOURCE "${CTEST_SOURCE_DIRECTORY}/scripts" RETURN_VALUE res)
		IF( NOT (res EQUAL 0)) 
			message(SEND_ERROR "  Configure failed.")
			set(ALL_OK FALSE)	
		ENDIF( NOT (res EQUAL 0))
	ENDIF(NEED_CONFIGURE)  
ENDIF(ALL_OK)
	
## -- BUILD
IF(ALL_OK)
	message(" -- Build ${MODEL} - ${CTEST_BUILD_NAME} --")
	ctest_build(BUILD  "${CTEST_SOURCE_DIRECTORY}/scripts" SOURCE "${CTEST_SOURCE_DIRECTORY}/scripts" RETURN_VALUE res)
	IF( NOT (res EQUAL 0)) 
		message(SEND_ERROR "  Build failed.")
		set(ALL_OK FALSE)
	ENDIF( NOT (res EQUAL 0))
ENDIF(ALL_OK)

## -- TEST
IF(ALL_OK)
	message(" -- Test ${MODEL} - ${CTEST_BUILD_NAME} --")
	set( $ENV{CTEST_BINARY_DIRECTORY}    "${CTEST_BINARY_DIRECTORY}" )
	ctest_test(BUILD  "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
	message(STATUS "  INFO : ctest_test(...) returned '${res}'")
ENDIF(ALL_OK)

## -- SUBMIT
message(" -- Submit ${MODEL} - ${CTEST_BUILD_NAME} --")

#ctest_submit(                                              RETURN_VALUE res)
message(STATUS "  INFO : submit(...) returned '${res}'")

message(" -- Finished ${MODEL}  - ${CTEST_BUILD_NAME} --")

IF(${MODEL} MATCHES Nightly)
	message(" -- clean ${MODEL}  - ${CTEST_BUILD_NAME} --")
	ctest_empty_binary_directory(${CTEST_SOURCE_DIRECTORY})
	ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})
	exec_program("rm" ARGS "-rf ${CTEST_SOURCE_DIRECTORY}" OUTPUT_VARIABLE "cleanScript")
ENDIF(${MODEL} MATCHES Nightly)


