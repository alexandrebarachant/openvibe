@echo off

if "%1" == "" goto base

REM #######################################################################################

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
REM nmake OpenViBE-documentation
cd %saved_directory%

goto terminate_no_pause

REM #######################################################################################

:sub_error

return 1

goto terminate_no_pause

:base

REM #######################################################################################

if not exist "win32-init_env_command.cmd" (
	echo You have to configure your `win32-init_env_command` file.
	echo `win32-init_env_command-skeleton` is a good file to start with...
	goto terminate
)

call "win32-init_env_command.cmd"

REM #######################################################################################

set saved_directory=%CD%
set target_dist=..\dist

mkdir ..\local-tmp 2> NULL
for /F %%s in (%OpenViBE_build_order%) do (
	cmd /e /c %0 %%s
)

REM #######################################################################################

rmdir /s /q %target_dist%         > NULL 2<&1

mkdir %target_dist%               > NULL 2<&1
mkdir %target_dist%\bin           > NULL 2<&1
mkdir %target_dist%\include       > NULL 2<&1
mkdir %target_dist%\lib           > NULL 2<&1
mkdir %target_dist%\share         > NULL 2<&1
mkdir %target_dist%\etc           > NULL 2<&1
mkdir %target_dist%\doc           > NULL 2<&1
mkdir %target_dist%\log           > NULL 2<&1
mkdir %target_dist%\tmp           > NULL 2<&1

echo @echo off                                               >  %target_dist%\test-acquisition-server.cmd
echo pushd ..\scripts                                        >> %target_dist%\test-acquisition-server.cmd
echo call win32-init_env_command.cmd                         >> %target_dist%\test-acquisition-server.cmd
echo popd                                                    >> %target_dist%\test-acquisition-server.cmd
echo cd lib                                                  >> %target_dist%\test-acquisition-server.cmd
echo ..\bin\OpenViBE-acquisition-server-dynamic.exe          >> %target_dist%\test-acquisition-server.cmd

echo @echo off                                               >  %target_dist%\test-designer.cmd
echo pushd ..\scripts                                        >> %target_dist%\test-designer.cmd
echo call win32-init_env_command.cmd                         >> %target_dist%\test-designer.cmd
echo popd                                                    >> %target_dist%\test-designer.cmd
echo cd lib                                                  >> %target_dist%\test-designer.cmd
echo ..\bin\OpenViBE-designer-dynamic.exe                    >> %target_dist%\test-designer.cmd

echo @echo off                                               >  %target_dist%\test-id-generator.cmd
echo pushd ..\scripts                                        >> %target_dist%\test-id-generator.cmd
echo call win32-init_env_command.cmd                         >> %target_dist%\test-id-generator.cmd
echo popd                                                    >> %target_dist%\test-id-generator.cmd
echo cd lib                                                  >> %target_dist%\test-id-generator.cmd
echo ..\bin\OpenViBE-id-generator-dynamic.exe                >> %target_dist%\test-id-generator.cmd
echo pause                                                   >> %target_dist%\test-id-generator.cmd

echo @echo off                                               >  %target_dist%\test-plugin-inspector.cmd
echo pushd ..\scripts                                        >> %target_dist%\test-plugin-inspector.cmd
echo call win32-init_env_command.cmd                         >> %target_dist%\test-plugin-inspector.cmd
echo popd                                                    >> %target_dist%\test-plugin-inspector.cmd
echo cd lib                                                  >> %target_dist%\test-plugin-inspector.cmd
echo ..\bin\OpenViBE-plugin-inspector-dynamic.exe            >> %target_dist%\test-plugin-inspector.cmd
echo pause                                                   >> %target_dist%\test-plugin-inspector.cmd

for /F %%s in (%OpenViBE_build_order%) do (
	xcopy /q /s %%s\bin\*.exe          %target_dist%\bin     > NULL 2<&1
	xcopy /q /s %%s\include\*.*        %target_dist%\include > NULL 2<&1
	xcopy /q /s %%s\lib\*.lib          %target_dist%\lib     > NULL 2<&1
	xcopy /q /s %%s\lib\*.dll          %target_dist%\lib     > NULL 2<&1
	xcopy /q /s %%s\share\*.*          %target_dist%\share   > NULL 2<&1
	xcopy /q /s %%s\etc\*.*            %target_dist%\etc     > NULL 2<&1
	xcopy /q /s %%s\doc\*.*            %target_dist%\doc     > NULL 2<&1
)

REM #######################################################################################

:terminate

pause

:terminate_no_pause

del NULL
