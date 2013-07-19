REM @ECHO OFF
SETLOCAL EnableExtensions

REM Get the directory location of this script, assume it contains the OpenViBE dist tree. These variables will be used by OpenViBE executables.
SET "OV_SOURCE=%1"
SET "OV_BUILD=%2"
SHIFT
SHIFT

SET "OV_PATH_BIN=%OV_BUILD%\openvibe\"
SET "OV_PATH_LIB=%OV_BUILD%\openvibe\;%OV_BUILD%\modules\ebml\;%OV_BUILD%\modules\system\;%OV_BUILD%\modules\fs\;%OV_BUILD%\modules\xml\;%OV_BUILD%\modules\socket\;%OV_BUILD%\modules\automaton\;%OV_BUILD%\toolkit\"
SET "OV_PATH_DATA=%OV_BUILD%\share\openvibe"

SET PATH=%OV_PATH_LIB%;%PATH%


SET "OV_ENVIRONMENT_FILE=%OV_SOURCE%\scripts\win32-dependencies.cmd"
IF NOT EXIST "%OV_ENVIRONMENT_FILE%" (
	ECHO Error: "%OV_ENVIRONMENT_FILE%" was not found
	GOTO EndOfScript
)
CALL "%OV_ENVIRONMENT_FILE%"
@ECHO ON

"%OV_BUILD%\applications\developer-tools\plugin-inspector\openvibe-plugin-inspector.exe"  %*
	
:EndOfScript