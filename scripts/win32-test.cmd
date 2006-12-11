@echo off

call win32-init_env_command.cmd
call win32-collect_build.cmd

cd ..\dist\lib
set PATH=..\lib;%PATH%
..\bin\OpenViBE-authoring-tool-dynamic.exe
