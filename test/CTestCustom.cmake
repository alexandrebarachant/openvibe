#This file is used to customize CTest. A detailed description of all options, can be found here. In order to be read in properly this file has to be placed in the binary directory ${CTEST_BINARY_DIRECTORY}.

#It can also be read from any other location by specifying

#  ctest_read_custom_files("<PATH TO CTestCustom.cmake FILE>")


# This are only some examples of customization. The full list and detailed description can be found here: http://public.kitware.com/Wiki/CMake_Testing_With_CTest#Customizing_CTest


#set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_WARNINGS "500" )
#set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_ERRORS   "200" )

# Add fake warnings to a list of exceptions
#set(CTEST_CUSTOM_WARNING_EXCEPTION
#	${CTEST_CUSTOM_WARNING_EXCEPTION}
#
#	# -- doxygen warnings
#	"are not documented:"
#	"Skipping documentation"
#	)

