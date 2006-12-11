@echo off

set PATH=%PATH%;C:\Program Files\cmake-2.4.3-win32-x86\bin
set PATH=%PATH%;C:\Program Files\pkg-config-0.20\bin

call "%VS80COMNTOOLS%\vsvars32.bat"

set PATH=%PATH%;E:\Microsoft Visual Studio 2005\VC\redist\Debug_NonRedist\x86\Microsoft.VC80.DebugCRT
set PATH=%PATH%;C:\Expat-2.0.0\Libs

set          OpenViBE_base=E:\Dev\openvibe
set               OpenViBE=%OpenViBE_base%\openvibe\branches\wip-yrenard
set       OpenViBE_modules=%OpenViBE_base%\openvibe-modules
set   OpenViBE_module_ebml=%OpenViBE_modules%\ebml\trunc
set     OpenViBE_module_fs=%OpenViBE_modules%\fs\trunc
set OpenViBE_module_socket=%OpenViBE_modules%\socket\trunc
set OpenViBE_module_system=%OpenViBE_modules%\system\trunc
set    OpenViBE_module_xml=%OpenViBE_modules%\xml\trunc
set        OpenViBE_kernel=%OpenViBE_base%\openvibe-kernel-omk\branches\wip-yrenard

set OpenMASKDIR=E:\OpenViBE\OpenMASK - VC 8.0\SDK

set PKG_CONFIG_PATH=C:\GTK\lib\pkgconfig
