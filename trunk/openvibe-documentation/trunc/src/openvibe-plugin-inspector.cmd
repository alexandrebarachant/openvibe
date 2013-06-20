REM @ECHO OFF
SETLOCAL EnableExtensions

REM Get the directory location of this script, assume it contains the OpenViBE dist tree. These variables will be used by OpenViBE executables.
SET "OV_SOURCE=%1"
SET "OV_BUILD=%2"
SHIFT
SHIFT

SET "OV_PATH_BIN=%OV_BUILD%\openvibe\"
SET "OV_PATH_LIB=%OV_BUILD%\openvibe\trunc;%OV_BUILD%\openvibe-modules\ebml\trunc;%OV_BUILD%\openvibe-modules\system\trunc;%OV_BUILD%\openvibe-modules\fs\trunc;%OV_BUILD%\openvibe-modules\xml\trunc;%OV_BUILD%\openvibe-toolkit\trunc"
SET "OV_PATH_DATA=%OV_BUILD%\share"

SET PATH=%OV_PATH_LIB%;%PATH%


SET "OV_ENVIRONMENT_FILE=%OV_SOURCE%\scripts\win32-dependencies.cmd"
IF NOT EXIST "%OV_ENVIRONMENT_FILE%" (
	ECHO Error: "%OV_ENVIRONMENT_FILE%" was not found
	GOTO EndOfScript
)
CALL "%OV_ENVIRONMENT_FILE%"
@ECHO ON

"%OV_BUILD%\openvibe-applications\plugin-inspector\trunc\openvibe-plugin-inspector.exe"  %*
	
:EndOfScript