@echo off

if "%1" == "" goto base

set what_full=%1
echo set what_relative=%%what_full:%OpenViBE_base%\=%% > windows_sucks.cmd
call windows_sucks.cmd
del windows_sucks.cmd

mkdir ..\local-tmp\%what_relative% 2> NULL
cd ..\local-tmp\%what_relative%
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=" /DWIN32 /D_WINDOWS /W3 /Zm1000 /EHsc /GR /wd4355" -DCMAKE_MODULE_PATH="%saved_directory:\=/%/../cmake-modules;${CMAKE_MODULE_PATH}" %what_full% -G"NMake Makefiles"
IF NOT "%ERRORLEVEL%" == "0" goto sub_error
nmake
IF NOT "%ERRORLEVEL%" == "0" goto sub_error
cd %saved_directory%

goto terminate_2

:sub_error

return 1

goto terminate_2

:base

if not exist "win32-init_env_command.cmd" (
	echo You have to configure your `win32-init_env_command` file.
	echo `win32-init_env_command-skeleton` is a good file to start with...
	goto terminate
)

call "win32-init_env_command.cmd"

set saved_directory=%CD%

mkdir ..\local-tmp 2> NULL
for /F %%s in (%OpenViBE_build_order%) do (
	cmd /e /c %0 %%s
)
del NULL

:terminate

pause

:terminate_2
