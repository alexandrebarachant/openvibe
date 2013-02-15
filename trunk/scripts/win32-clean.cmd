@echo off

if "%1" == "" goto recall_me

goto base

REM #######################################################################################

:recall_me

cmd /e /v /c %0 dummy

goto terminate

REM #######################################################################################

:base

REM #######################################################################################

call "win32-init_env_command.cmd"

REM #######################################################################################

set saved_directory=%CD%

cd ..\local-tmp\nmake

echo Cleaning up ..\local-tmp\nmake

nmake clean
IF NOT "!ERRORLEVEL!" == "0" goto terminate_error

echo.
echo Clean process terminated successfully !
echo.

pause

goto terminate

:terminate_error

echo There was a problem during clean...

:terminate

