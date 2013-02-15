@echo off
setlocal EnableDelayedExpansion
setlocal enableextensions 

SET InitEnvScript="win32-init_env_command.cmd"

call "!InitEnvScript!"

REM needed for debugging vr-demo ...
SET PATH=%CD%\..\dist\bin;%PATH%

REM for visual studio express...

"%VSINSTALLDIR%Common7\IDE\VCExpress.exe" ..\local-tmp\visual\OpenViBE.sln

