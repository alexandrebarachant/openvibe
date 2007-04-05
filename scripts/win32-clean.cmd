@echo off

if not exist "win32-init_env_command.cmd" (
	echo You have to configure your `win32-init_env_command` file.
	echo `win32-init_env_command-skeleton` is a good file to start with...
	goto terminate
)

call "win32-init_env_command.cmd"

set saved_directory=%CD%

mkdir ..\local-tmp 2> NULL
for /d %%s in (cmake\*.*) do (
	mkdir ..\local-tmp\%%s 2> NULL
	cd ..\local-tmp\%%s
	cmake -DCMAKE_BUILD_TYPE=Release %saved_directory%/%%s -G"NMake Makefiles"
	nmake clean
	cd %saved_directory%
)
rmdir /s /q ..\local-tmp
del NULL

:terminate

pause
