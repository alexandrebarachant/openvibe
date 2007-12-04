@echo off

if not exist "win32-init_env_command.cmd" (
	echo You have to configure your `win32-init_env_command` file.
	echo `win32-init_env_command-skeleton` is a good file to start with...
	goto terminate
)

call "win32-init_env_command.cmd"

set saved_directory=%CD%

mkdir ..\local-tmp 2> NULL
del CMakeLists.txt 2> NULL

for /d %%s in (cmake\*.*) do (
	type %%s\CMakeLists.txt >> CMakeLists.txt
)
cd ..\local-tmp
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=" /DWIN32 /D_WINDOWS /W3 /Zm1000 /EHsc /GR /wd4355" -DCMAKE_MODULE_PATH="%saved_directory:\=/%/../cmake-modules;${CMAKE_MODULE_PATH}" %saved_directory% -G"Visual Studio 8 2005"
cd %saved_directory%
del NULL

:terminate

pause
