@echo off
setlocal EnableDelayedExpansion
setlocal enableextensions 

SET InitEnvScript="win32-init_env_command.cmd"

call "!InitEnvScript!"

REM needed for debugging in VS ...
SET "OV_DISTROOT=%CD%\..\dist"
SET "OV_BINDIR=%OV_DISTROOT%\bin"
SET "OV_DATADIR=%OV_DISTROOT%\share"
SET "OV_LIBDIR=%OV_DISTROOT%\bin"
SET "PATH=%OV_DISTROOT%\bin;%PATH%"

REM for visual studio express...

"%VSINSTALLDIR%Common7\IDE\VCExpress.exe" ..\local-tmp\visual\OpenViBE.sln

