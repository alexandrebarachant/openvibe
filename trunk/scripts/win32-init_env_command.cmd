@echo off

REM ########################################################################################################################

if exist "win32-dependencies.cmd" (
	call "win32-dependencies.cmd"
) else (
	echo ERROR: win32-dependencies.cmd not found. Has the dependency installer been run?
	goto terminate
	
)

REM ########################################################################################################################

SET VSTOOLS=
SET VSCMake=

if exist "%VS100COMNTOOLS%vsvars32.bat" (
	echo Found VS100 tools...
	CALL "%VS100COMNTOOLS%vsvars32.bat"
	SET VSCMake=Visual Studio 10
) else if exist "%VS90COMNTOOLS%vsvars32.bat" (
	echo Found VS90 tools...
	CALL "%VS90COMNTOOLS%vsvars32.bat"
	SET VSCMake=Visual Studio 9 2008
) else (
	echo ######################################################################################
	echo ##                                                                                  ##
	echo ##  ERROR : Microsoft Visual Studio Common tools initialisation script not found    ##
	echo ##                                                                                  ##
	echo ######################################################################################
	goto terminate
)

REM #######################################################################################

:terminate
