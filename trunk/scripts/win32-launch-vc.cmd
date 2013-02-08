@echo off
setlocal EnableDelayedExpansion
setlocal enableextensions 

SET InitEnvScript="win32-init_env_command.cmd"

if not exist "!InitEnvScript!" (
	if not "!InitEnvScript!"=="win32-init_env_command.cmd" (
		echo `!InitEnvScript!` file not found !
		echo Init-env script used : "win32-init_env_command.cmd"
		set !InitEnvScript!="win32-init_env_command.cmd"
	)
	if not exist "!InitEnvScript!" (
		echo "!InitEnvScript!" not found !
		echo `win32-init_env_command.cmd-skeleton` is a good file to start with... (I'll copy this file for you^)
		copy "win32-init_env_command.cmd-skeleton" "!InitEnvScript!"
	)
)
call "!InitEnvScript!"

REM needed for debugging vr-demo ...
SET PATH=%CD%\..\dist\bin;%PATH%

REM for visual studio express...

"%VSINSTALLDIR%Common7\IDE\VCExpress.exe" ..\local-tmp\visual\OpenViBE.sln

