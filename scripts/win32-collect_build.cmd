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

rmdir /s /q %target_dist%\bin     > NULL 2<&1
rmdir /s /q %target_dist%\include > NULL 2<&1
rmdir /s /q %target_dist%\lib     > NULL 2<&1
rmdir /s /q %target_dist%\share   > NULL 2<&1

mkdir %target_dist%               > NULL 2<&1
mkdir %target_dist%\bin           > NULL 2<&1
mkdir %target_dist%\include       > NULL 2<&1
mkdir %target_dist%\lib           > NULL 2<&1
mkdir %target_dist%\share         > NULL 2<&1

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

REM #######################################################################################

xcopy /q /s %OpenViBE_module_ebml%\bin\*.exe                    %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_module_automaton%\bin\*.exe               %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_module_fs%\bin\*.exe                      %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_module_socket%\bin\*.exe                  %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_module_system%\bin\*.exe                  %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_module_xml%\bin\*.exe                     %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE%\bin\*.exe                                %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_kernel%\bin\*.exe                         %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_toolkit%\bin\*.exe                        %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_application_designer%\bin\*.exe           %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_application_acquisition_server%\bin\*.exe %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_application_id_generator%\bin\*.exe       %target_dist%\bin     > NULL 2<&1

xcopy /q /s %OpenViBE_module_ebml%\include\*.*                  %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_module_automaton%\include\*.*             %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_module_fs%\include\*.*                    %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_module_socket%\include\*.*                %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_module_system%\include\*.*                %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_module_xml%\include\*.*                   %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE%\include\*.*                              %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_kernel%\include\*.*                       %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_toolkit%\include\*.*                      %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_application_designer%\include\*.*         %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_application_acquisition_server%\include\*.* %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_application_id_generator%\include\*.*     %target_dist%\include > NULL 2<&1

xcopy /q /s %OpenViBE_module_automaton%\lib\*.lib               %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_automaton%\lib\*.dll               %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_ebml%\lib\*.lib                    %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_ebml%\lib\*.dll                    %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_fs%\lib\*.lib                      %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_fs%\lib\*.dll                      %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_socket%\lib\*.lib                  %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_socket%\lib\*.dll                  %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_system%\lib\*.lib                  %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_system%\lib\*.dll                  %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_xml%\lib\*.lib                     %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_module_xml%\lib\*.dll                     %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE%\lib\*.lib                                %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE%\lib\*.dll                                %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_kernel%\lib\*.lib                         %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_kernel%\lib\*.dll                         %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_toolkit%\lib\*.lib                        %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_toolkit%\lib\*.dll                        %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_application_designer%\lib\*.lib           %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_application_designer%\lib\*.dll           %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_application_acquisition_server%\lib\*.lib %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_application_acquisition_server%\lib\*.dll %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_application_id_generator%\lib\*.lib       %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_application_id_generator%\lib\*.dll       %target_dist%\lib     > NULL 2<&1

xcopy /q /s %OpenViBE_module_ebml%\share\*.*                    %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE_module_automaton%\share\*.*               %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE_module_fs%\share\*.*                      %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE_module_socket%\share\*.*                  %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE_module_system%\share\*.*                  %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE_module_xml%\share\*.*                     %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE%\share\*.*                                %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE_kernel%\share\*.*                         %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE_toolkit%\share\*.*                        %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE_application_designer%\share\*.*           %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE_application_acquisition_server%\share\*.* %target_dist%\share   > NULL 2<&1
xcopy /q /s %OpenViBE_application_id_generator%\share\*.*       %target_dist%\share   > NULL 2<&1

REM #######################################################################################

xcopy /q /s %OpenViBE_plugin_acquisition%\bin\*.exe             %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_acquisition%\include\*.*           %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_acquisition%\lib\*.lib             %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_acquisition%\lib\*.dll             %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_acquisition%\share\*.*             %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_classification%\bin\*.exe          %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_classification%\include\*.*        %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_classification%\lib\*.lib          %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_classification%\lib\*.dll          %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_classification%\share\*.*          %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_feature_extraction%\bin\*.exe      %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_feature_extraction%\include\*.*    %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_feature_extraction%\lib\*.lib      %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_feature_extraction%\lib\*.dll      %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_feature_extraction%\share\*.*      %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_file_io%\bin\*.exe                 %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_file_io%\include\*.*               %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_file_io%\lib\*.lib                 %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_file_io%\lib\*.dll                 %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_file_io%\share\*.*                 %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_samples%\bin\*.exe                 %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_samples%\include\*.*               %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_samples%\lib\*.lib                 %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_samples%\lib\*.dll                 %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_samples%\share\*.*                 %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_signal_processing%\bin\*.exe       %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_signal_processing%\include\*.*     %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_signal_processing%\lib\*.lib       %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_signal_processing%\lib\*.dll       %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_signal_processing%\share\*.*       %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_signal_processing_gpl%\bin\*.exe   %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_signal_processing_gpl%\include\*.* %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_signal_processing_gpl%\lib\*.lib   %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_signal_processing_gpl%\lib\*.dll   %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_signal_processing_gpl%\share\*.*   %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_simple_visualisation%\bin\*.exe    %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_simple_visualisation%\include\*.*  %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_simple_visualisation%\lib\*.lib    %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_simple_visualisation%\lib\*.dll    %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_simple_visualisation%\share\*.*    %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_stimulation%\bin\*.exe             %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_stimulation%\include\*.*           %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_stimulation%\lib\*.lib             %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_stimulation%\lib\*.dll             %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_stimulation%\share\*.*             %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_stream_codecs%\bin\*.exe           %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_stream_codecs%\include\*.*         %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_stream_codecs%\lib\*.lib           %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_stream_codecs%\lib\*.dll           %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_stream_codecs%\share\*.*           %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_tools%\bin\*.exe                   %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_tools%\include\*.*                 %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_tools%\lib\*.lib                   %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_tools%\lib\*.dll                   %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_tools%\share\*.*                   %target_dist%\share   > NULL 2<&1

xcopy /q /s %OpenViBE_plugin_vrpn%\bin\*.exe                    %target_dist%\bin     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_vrpn%\include\*.*                  %target_dist%\include > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_vrpn%\lib\*.lib                    %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_vrpn%\lib\*.dll                    %target_dist%\lib     > NULL 2<&1
xcopy /q /s %OpenViBE_plugin_vrpn%\share\*.*                    %target_dist%\share   > NULL 2<&1

REM #######################################################################################

del NULL
