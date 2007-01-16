@echo off

call win32-init_env_command.cmd
call win32-collect_build.cmd

cd ..\dist\bin
set PATH=..\lib;%PATH%
OpenViBE-designer-dynamic.exe &
OpenViBE-acquisition-server-dynamic.exe &
