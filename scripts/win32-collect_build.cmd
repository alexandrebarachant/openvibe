call win32-init_env_command.cmd

set target_dist=..\dist

rmdir /s /q %target_dist%\bin
rmdir /s /q %target_dist%\include
rmdir /s /q %target_dist%\lib
rmdir /s /q %target_dist%\share

mkdir %target_dist%
mkdir %target_dist%\bin
mkdir %target_dist%\include
mkdir %target_dist%\lib
mkdir %target_dist%\share

xcopy /s %OpenViBE_module_ebml%\bin\*.exe %target_dist%\bin
xcopy /s %OpenViBE_module_fs%\bin\*.exe %target_dist%\bin
xcopy /s %OpenViBE_module_socket%\bin\*.exe %target_dist%\bin
xcopy /s %OpenViBE_module_system%\bin\*.exe %target_dist%\bin
xcopy /s %OpenViBE_module_xml%\bin\*.exe %target_dist%\bin
xcopy /s %OpenViBE%\bin\*.exe %target_dist%\bin
xcopy /s %OpenViBE_kernel%\bin\*.exe %target_dist%\bin
xcopy /s %OpenViBE_designer%\bin\*.exe %target_dist%\bin

xcopy /s %OpenViBE_module_ebml%\include\*.* %target_dist%\include
xcopy /s %OpenViBE_module_fs%\include\*.* %target_dist%\include
xcopy /s %OpenViBE_module_socket%\include\*.* %target_dist%\include
xcopy /s %OpenViBE_module_system%\include\*.* %target_dist%\include
xcopy /s %OpenViBE_module_xml%\include\*.* %target_dist%\include
xcopy /s %OpenViBE%\include\*.* %target_dist%\include
xcopy /s %OpenViBE_kernel%\include\*.* %target_dist%\include
xcopy /s %OpenViBE_designer%\include\*.* %target_dist%\include

xcopy /s %OpenViBE_module_ebml%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE_module_ebml%\lib\*.dll %target_dist%\lib
xcopy /s %OpenViBE_module_fs%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE_module_fs%\lib\*.dll %target_dist%\lib
xcopy /s %OpenViBE_module_socket%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE_module_socket%\lib\*.dll %target_dist%\lib
xcopy /s %OpenViBE_module_system%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE_module_system%\lib\*.dll %target_dist%\lib
xcopy /s %OpenViBE_module_xml%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE_module_xml%\lib\*.dll %target_dist%\lib
xcopy /s %OpenViBE%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE%\lib\*.dll %target_dist%\lib
xcopy /s %OpenViBE_kernel%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE_kernel%\lib\*.dll %target_dist%\lib
xcopy /s %OpenViBE_designer%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE_designer%\lib\*.dll %target_dist%\lib

xcopy /s %OpenViBE_module_ebml%\share\*.* %target_dist%\share
xcopy /s %OpenViBE_module_fs%\share\*.* %target_dist%\share
xcopy /s %OpenViBE_module_socket%\share\*.* %target_dist%\share
xcopy /s %OpenViBE_module_system%\share\*.* %target_dist%\share
xcopy /s %OpenViBE_module_xml%\share\*.* %target_dist%\share
xcopy /s %OpenViBE%\share\*.* %target_dist%\share
xcopy /s %OpenViBE_kernel%\share\*.* %target_dist%\share
xcopy /s %OpenViBE_designer%\share\*.* %target_dist%\share

xcopy /s %OpenViBE_plugin_acquisition%\bin\*.exe %target_dist%\bin
xcopy /s %OpenViBE_plugin_acquisition%\include\*.* %target_dist%\include
xcopy /s %OpenViBE_plugin_acquisition%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE_plugin_acquisition%\lib\*.dll %target_dist%\lib
xcopy /s %OpenViBE_plugin_acquisition%\share\*.* %target_dist%\share

xcopy /s %OpenViBE_plugin_utility%\bin\*.exe %target_dist%\bin
xcopy /s %OpenViBE_plugin_utility%\include\*.* %target_dist%\include
xcopy /s %OpenViBE_plugin_utility%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE_plugin_utility%\lib\*.dll %target_dist%\lib
xcopy /s %OpenViBE_plugin_utility%\share\*.* %target_dist%\share

xcopy /s %OpenViBE_plugin_samples%\bin\*.exe %target_dist%\bin
xcopy /s %OpenViBE_plugin_samples%\include\*.* %target_dist%\include
xcopy /s %OpenViBE_plugin_samples%\lib\*.lib %target_dist%\lib
xcopy /s %OpenViBE_plugin_samples%\lib\*.dll %target_dist%\lib
xcopy /s %OpenViBE_plugin_samples%\share\*.* %target_dist%\share
