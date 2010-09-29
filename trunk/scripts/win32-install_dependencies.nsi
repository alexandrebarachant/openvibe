	!include "MUI.nsh"
	!include "zipdll.nsh"

	;Name and file
	Name "OpenViBE dependencies"
	OutFile "win32-install_dependencies.exe"

	;Default installation folder
	InstallDir "$EXEDIR\..\dependencies"

;Interface Settings

	!define MUI_ABORTWARNING

;Pages

	!insertmacro MUI_PAGE_COMPONENTS
	!insertmacro MUI_PAGE_INSTFILES

;  !insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES

;Languages

	!insertmacro MUI_LANGUAGE "English"

;Installer and uninstaller icons

	Icon "${NSISDIR}\Contrib\Graphics\Icons\box-install.ico"
	UninstallIcon "${NSISDIR}\Contrib\Graphics\Icons\box-uninstall.ico"

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "-base"

	;Finds Microsoft Platform SDK

	ReadRegStr $r0 HKLM "SOFTWARE\Microsoft\Win32SDK\Directories" "Install Dir"
	StrCmp $r0 "" base_failed_to_find_sdk_1 base_found_sdk
base_failed_to_find_sdk_1:
	ReadRegStr $r0 HKLM "SOFTWARE\Microsoft\MicrosoftSDK\Directories" "Install Dir"
	StrCmp $r0 "" base_failed_to_find_sdk_2 base_found_sdk
base_failed_to_find_sdk_2:
	ReadRegStr $r0 HKLM "SOFTWARE\Microsoft\Microsoft SDKs\Windows" "CurrentInstallFolder"
	StrCmp $r0 "" base_failed_to_find_sdk_3 base_found_sdk
base_failed_to_find_sdk_3:
	goto base_failed_to_find_sdk

base_failed_to_find_sdk:
	MessageBox MB_OK|MB_ICONEXCLAMATION "Failed to find Microsoft Platform SDK$\nPlease update your win32-dependencies.cmd script by hand"
	goto base_go_on
base_found_sdk:
	MessageBox MB_OK "Microsoft Platform SDK found at :$\n$r0"
	goto base_go_on

base_go_on:

	;Create uninstaller
	WriteUninstaller "$INSTDIR\Uninstall.exe"

	;clears dependencies file
	FileOpen $0 "$EXEDIR\win32-dependencies.cmd" w
	FileWrite $0 "@echo off$\r$\n"
	FileWrite $0 "$\r$\n"
	FileWrite $0 "SET OV_DEP_MSSDK=$r0$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "CMake"

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-dependency-cmake-2.8.2-win32-x86.zip" no_need_to_download_cmake
	NSISdl::download http://openvibe.inria.fr/dependencies/win32/cmake-2.8.2-win32-x86.zip "arch\openvibe-dependency-cmake-2.8.2-win32-x86.zip"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit

no_need_to_download_cmake:

	IfFileExists "cmake" no_need_to_install_cmake
	ZipDLL::extractall "arch\openvibe-dependency-cmake-2.8.2-win32-x86.zip" "cmake"

no_need_to_install_cmake:

	FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
	FileSeek $0 0 END
	FileWrite $0 "SET OV_DEP_CMAKE=$INSTDIR\cmake\cmake-2.8.2-win32-x86$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "eXpat"

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-dependency-expat-2.0.1.zip" no_need_to_download_expat
	NSISdl::download http://openvibe.inria.fr/dependencies/win32/expat-2.0.1.zip "arch\openvibe-dependency-expat-2.0.1.zip"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit

no_need_to_download_expat:

	IfFileExists "expat" no_need_to_install_expat
	ZipDLL::extractall "arch\openvibe-dependency-expat-2.0.1.zip" "expat"

no_need_to_install_expat:

	FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
	FileSeek $0 0 END
	FileWrite $0 "SET OV_DEP_EXPAT=$INSTDIR\expat$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "BOOST"

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-dependency-boost-1.42.0.zip" no_need_to_download_boost
	NSISdl::download http://openvibe.inria.fr/dependencies/win32/boost-1.42.0.zip "arch\openvibe-dependency-boost-1.42.0.zip"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit

no_need_to_download_boost:

	IfFileExists "boost" no_need_to_install_boost
	ZipDLL::extractall "arch\openvibe-dependency-boost-1.42.0.zip" "boost"

no_need_to_install_boost:

	FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
	FileSeek $0 0 END
	FileWrite $0 "SET OV_DEP_BOOST=$INSTDIR\boost$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "GTK+"

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-dependency-gtk-2.16.6.zip" no_need_to_download_gtk
	NSISdl::download http://openvibe.inria.fr/dependencies/win32/gtk-2.16.6.zip "arch\openvibe-dependency-gtk-2.16.6.zip"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit

no_need_to_download_gtk:

	IfFileExists "GTK" no_need_to_install_gtk
	ZipDLL::extractall "arch\openvibe-dependency-gtk-2.16.6.zip" "gtk"

no_need_to_install_gtk:

	FileOpen $0 "$INSTDIR\gtk\lib\pkgconfig\gtk+-win32-2.0.pc" w
	FileWrite $0 "prefix=$INSTDIR\gtk$\r$\n"
	FileWrite $0 "exec_prefix=$${prefix}$\r$\n"
	FileWrite $0 "libdir=$${exec_prefix}/lib$\r$\n"
	FileWrite $0 "includedir=$${prefix}/include$\r$\n"
	FileWrite $0 "target=win32$\r$\n"
	FileWrite $0 "$\r$\n"
	FileWrite $0 "Name: GTK+$\r$\n"
	FileWrite $0 "Description: GTK+ Graphical UI Library ($${target} target)$\r$\n"
	FileWrite $0 "Version: 2.16.6$\r$\n"
	FileWrite $0 "Requires: gdk-$${target}-2.0 atk cairo gio-2.0$\r$\n"
	FileWrite $0 "Libs: -L$${libdir} -lgtk-$${target}-2.0$\r$\n"
	FileWrite $0 "Cflags: -I$${includedir}/gtk-2.0$\r$\n"
	FileClose $0

	FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
	FileSeek $0 0 END
	FileWrite $0 "SET OV_DEP_GTK=$INSTDIR\gtk$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section /o "GTK+ themes"

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-dependency-gtk-themes-2009.09.07.zip" no_need_to_download_gtk_themes
	NSISdl::download http://openvibe.inria.fr/dependencies/win32/gtk-themes-2009.09.07.zip "arch\openvibe-dependency-gtk-themes-2009.09.07.zip"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit

no_need_to_download_gtk_themes:

;	IfFileExists "GTK" no_need_to_install_gtk_themes
	ZipDLL::extractall "arch\openvibe-dependency-gtk-themes-2009.09.07.zip" "gtk"

;no_need_to_install_gtk_themes:

	FileOpen $0 "$INSTDIR\gtk\etc\gtk-2.0\gtkrc" w
	FileWrite $0 "gtk-theme-name = $\"Redmond$\"$\r$\n"
	FileWrite $0 "style $\"user-font$\"$\r$\n"
	FileWrite $0 "{$\r$\n"
	FileWrite $0 "	font_name=$\"Sans 8$\"$\r$\n"
	FileWrite $0 "}$\r$\n"
	FileWrite $0 "widget_class $\"*$\" style $\"user-font$\"$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "IT++ for Visual C++ 2008"

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-dependency-itpp-4.0.7-vs90.zip" no_need_to_download_itpp
	NSISdl::download http://openvibe.inria.fr/dependencies/win32/itpp-4.0.7-vs90.zip "arch\openvibe-dependency-itpp-4.0.7-vs90.zip"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit

no_need_to_download_itpp:

	IfFileExists "itpp" no_need_to_install_itpp
	ZipDLL::extractall "arch\openvibe-dependency-itpp-4.0.7-vs90.zip" "itpp"

no_need_to_install_itpp:

	FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
	FileSeek $0 0 END
	FileWrite $0 "SET OV_DEP_ITPP=$INSTDIR\itpp$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "Lua"

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-dependency-lua-5.1.4-30.zip" no_need_to_download_lua
	NSISdl::download http://openvibe.inria.fr/dependencies/win32/lua-5.1.4-30.zip "arch\openvibe-dependency-lua-5.1.4-30.zip"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit

no_need_to_download_lua:

	IfFileExists "lua" no_need_to_install_lua
	ZipDLL::extractall "arch\openvibe-dependency-lua-5.1.4-30.zip" "lua"

no_need_to_install_lua:

	FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
	FileSeek $0 0 END
	FileWrite $0 "SET OV_DEP_LUA=$INSTDIR\lua$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "Ogre3D for Visual C++ 2008" ogre_vc_2008

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-dependency-ogre-1.7.0-vc2008.zip" no_need_to_download_ogre_vc2008
	NSISdl::download http://openvibe.inria.fr/dependencies/win32/ogre-1.7.0-vc2008.zip "arch\openvibe-dependency-ogre-1.7.0-vc2008.zip"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit

no_need_to_download_ogre_vc2008:

	IfFileExists "ogre-vc2008" no_need_to_install_ogre_vc2008
	ZipDLL::extractall "arch\openvibe-dependency-ogre-1.7.0-vc2008.zip" "ogre-vc2008"

no_need_to_install_ogre_vc2008:

	FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
	FileSeek $0 0 END
	FileWrite $0 "SET OV_DEP_OGRE=$INSTDIR\ogre-vc2008$\r$\n"
	FileWrite $0 "SET OGRE_HOME=$INSTDIR\ogre-vc2008$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "CEGUI for Visual C++ 2008" cegui_vc_2008

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-dependency-cegui-0.7.1-vc2008.zip" no_need_to_download_cegui_vc2008
	NSISdl::download http://openvibe.inria.fr/dependencies/win32/cegui-0.7.1-vc2008.zip "arch\openvibe-dependency-cegui-0.7.1-vc2008.zip"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit

no_need_to_download_cegui_vc2008:

	IfFileExists "cegui-vc2008" no_need_to_install_cegui_vc2008
	ZipDLL::extractall "arch\openvibe-dependency-cegui-0.7.1-vc2008.zip" "cegui-vc2008"

no_need_to_install_cegui_vc2008:

	FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
	FileSeek $0 0 END
	FileWrite $0 "SET OV_DEP_CEGUI=$INSTDIR\cegui-vc2008$\r$\n"
	FileClose $0

	FileOpen $0 "$INSTDIR\cegui-vc2008\resources.cfg" w
	FileWrite $0 "FileSystem=$INSTDIR\cegui-vc2008\datafiles\configs$\r$\n"
	FileWrite $0 "FileSystem=$INSTDIR\cegui-vc2008\datafiles\fonts$\r$\n"
	FileWrite $0 "FileSystem=$INSTDIR\cegui-vc2008\datafiles\imagesets$\r$\n"
	FileWrite $0 "FileSystem=$INSTDIR\cegui-vc2008\datafiles\layouts$\r$\n"
	FileWrite $0 "FileSystem=$INSTDIR\cegui-vc2008\datafiles\looknfeel$\r$\n"
	FileWrite $0 "FileSystem=$INSTDIR\cegui-vc2008\datafiles\lua_scripts$\r$\n"
	FileWrite $0 "FileSystem=$INSTDIR\cegui-vc2008\datafiles\schemes$\r$\n"
	FileWrite $0 "FileSystem=$INSTDIR\cegui-vc2008\datafiles\xml_schemes$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "VRPN for Visual C++ 2008"

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-dependency-vrpn-7.26-vs90.zip" no_need_to_download_vrpn
	NSISdl::download http://openvibe.inria.fr/dependencies/win32/vrpn-7.26-vs90.zip "arch\openvibe-dependency-vrpn-7.26-vs90.zip"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit

no_need_to_download_vrpn:

	IfFileExists "vrpn" no_need_to_install_vrpn
	ZipDLL::extractall "arch\openvibe-dependency-vrpn-7.26-vs90.zip" "vrpn"

no_need_to_install_vrpn:

	FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
	FileSeek $0 0 END
	FileWrite $0 "SET OV_DEP_VRPN=$INSTDIR\vrpn$\r$\n"
	FileWrite $0 "SET VRPNROOT=$INSTDIR\vrpn$\r$\n"
	FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "DirectX"

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "$SYSDIR\d3dx9_38.dll" no_need_to_install_directx
	IfFileExists "arch\openvibe-directx.exe" no_need_to_download_directx
	NSISdl::download "http://www.microsoft.com/downloads/info.aspx?na=90&p=&SrcDisplayLang=en&SrcCategoryId=&SrcFamilyId=04ac064b-00d1-474e-b7b1-442d8712d553&u=http%3a%2f%2fdownload.microsoft.com%2fdownload%2fB%2f7%2f9%2fB79FC9D7-47B8-48B7-A75E-101DEBEB5AB4%2fdirectx_aug2009_redist.exe" "arch\openvibe-directx.exe"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit
no_need_to_download_directx:
	ExecWait '"arch\openvibe-directx.exe" /T:"$INSTDIR\tmp" /Q'
	ExecWait '"$INSTDIR\tmp\DXSETUP.exe" /silent'
no_need_to_install_directx:

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "Visual Redistributable Packages"

	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\arch"

	IfFileExists "arch\openvibe-vcredist-2005-sp1.exe" no_need_to_download_vcredist_2005_sp1
	NSISdl::download "http://download.microsoft.com/download/a/3/7/a379292d-24f2-4bbb-841b-c2aeb1100471/vcredist_x86.exe" "arch\openvibe-vcredist-2005-sp1.exe"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit
no_need_to_download_vcredist_2005_sp1:
	ExecWait '"arch\openvibe-vcredist-2005-sp1.exe" /q'
;no_need_to_install_vcredist_2005_sp1:

	IfFileExists "arch\openvibe-vcredist-2008-sp1.exe" no_need_to_download_vcredist_2008_sp1
	NSISdl::download "http://download.microsoft.com/download/9/e/d/9edd8390-011b-4c6d-9806-d8dc2b10c0fb/vcredist_x86.exe" "arch\openvibe-vcredist-2008-sp1.exe"
	Pop $R0 ; Get the return value
		StrCmp $R0 "success" +3
			MessageBox MB_OK "Download failed: $R0"
			Quit
no_need_to_download_vcredist_2008_sp1:
	ExecWait '"arch\openvibe-vcredist-2008-sp1.exe" /q'
;no_need_to_install_vcredist_2008_sp1:

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "Uninstall"

	RMDir /r "$INSTDIR\gtk"
	RMDir /r "$INSTDIR\boost"
	RMDir /r "$INSTDIR\expat"
	RMDir /r "$INSTDIR\cmake"
	RMDir /r "$INSTDIR\itpp"
	RMDir /r "$INSTDIR\lua"
	RMDir /r "$INSTDIR\ogre"
	RMDir /r "$INSTDIR\vrpn"
	RMDir /r "$INSTDIR\tmp"

	Delete "$INSTDIR\..\scripts\win32-dependencies.cmd"

	Delete "$INSTDIR\Uninstall.exe"

	RMDir "$INSTDIR"

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################