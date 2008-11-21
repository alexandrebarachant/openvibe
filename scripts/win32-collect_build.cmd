@echo off

if not exist "win32-init_env_command.cmd" (
	echo You have to configure your `win32-init_env_command` file.
	echo `win32-init_env_command-skeleton` is a good file to start with...
	goto terminate
)

call "win32-init_env_command.cmd"

echo collecting build...

set target_dist=..\dist

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

REM #######################################################################################

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

REM #######################################################################################

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

del NULL
