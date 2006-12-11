call win32-init_env_command.cmd

cd..

rmdir /s /q dist\bin
rmdir /s /q dist\include
rmdir /s /q dist\lib

mkdir dist
mkdir dist\bin
mkdir dist\include
mkdir dist\lib

xcopy /s %OpenViBE_module_ebml%\bin\*.exe dist\bin
xcopy /s %OpenViBE_module_fs%\bin\*.exe dist\bin
xcopy /s %OpenViBE_module_socket%\bin\*.exe dist\bin
xcopy /s %OpenViBE_module_system%\bin\*.exe dist\bin
xcopy /s %OpenViBE_module_xml%\bin\*.exe dist\bin
xcopy /s %OpenViBE%\bin\*.exe dist\bin
xcopy /s %OpenViBE_kernel%\bin\*.exe dist\bin

xcopy /s %OpenViBE_module_ebml%\include\*.* dist\include
xcopy /s %OpenViBE_module_fs%\include\*.* dist\include
xcopy /s %OpenViBE_module_socket%\include\*.* dist\include
xcopy /s %OpenViBE_module_system%\include\*.* dist\include
xcopy /s %OpenViBE_module_xml%\include\*.* dist\include
xcopy /s %OpenViBE%\include\*.* dist\include
xcopy /s %OpenViBE_kernel%\include\*.* dist\include

xcopy /s %OpenViBE_module_ebml%\lib\*.lib dist\lib
xcopy /s %OpenViBE_module_ebml%\lib\*.dll dist\lib
xcopy /s %OpenViBE_module_fs%\lib\*.lib dist\lib
xcopy /s %OpenViBE_module_fs%\lib\*.dll dist\lib
xcopy /s %OpenViBE_module_socket%\lib\*.lib dist\lib
xcopy /s %OpenViBE_module_socket%\lib\*.dll dist\lib
xcopy /s %OpenViBE_module_system%\lib\*.lib dist\lib
xcopy /s %OpenViBE_module_system%\lib\*.dll dist\lib
xcopy /s %OpenViBE_module_xml%\lib\*.lib dist\lib
xcopy /s %OpenViBE_module_xml%\lib\*.dll dist\lib
xcopy /s %OpenViBE%\lib\*.lib dist\lib
xcopy /s %OpenViBE%\lib\*.dll dist\lib
xcopy /s %OpenViBE_kernel%\lib\*.lib dist\lib
xcopy /s %OpenViBE_kernel%\lib\*.dll dist\lib

xcopy /s local-test\bin\*.exe dist\bin
xcopy /s local-test\include\*.* dist\include
xcopy /s local-test\lib\*.lib dist\lib
xcopy /s local-test\lib\*.dll dist\lib
xcopy /s local-test\src\*.glade dist\lib

xcopy /s openvibe-plugins\samples\branches\wip-yrenard\bin\*.exe dist\bin
xcopy /s openvibe-plugins\samples\branches\wip-yrenard\include\*.* dist\include
xcopy /s openvibe-plugins\samples\branches\wip-yrenard\lib\*.lib dist\lib
xcopy /s openvibe-plugins\samples\branches\wip-yrenard\lib\*.dll dist\lib

xcopy /s openvibe-plugins\acquisition\branches\wip-yrenard\bin\*.exe dist\bin
xcopy /s openvibe-plugins\acquisition\branches\wip-yrenard\include\*.* dist\include
xcopy /s openvibe-plugins\acquisition\branches\wip-yrenard\lib\*.lib dist\lib
xcopy /s openvibe-plugins\acquisition\branches\wip-yrenard\lib\*.dll dist\lib

xcopy /s openvibe-plugins\utility\branches\wip-yrenard\bin\*.exe dist\bin
xcopy /s openvibe-plugins\utility\branches\wip-yrenard\include\*.* dist\include
xcopy /s openvibe-plugins\utility\branches\wip-yrenard\lib\*.lib dist\lib
xcopy /s openvibe-plugins\utility\branches\wip-yrenard\lib\*.dll dist\lib
