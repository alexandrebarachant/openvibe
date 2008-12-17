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
  FileWrite $0 "@echo off$\n"
  FileWrite $0 "set OV_DEP_MSSDK=$r0$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "CMake"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-cmake-2.6.2.zip" no_need_to_download_cmake
  NSISdl::download http://www.irisa.fr/bunraku/OpenViBE/dependencies/win32/cmake-2.6.2-win32-x86.zip "arch\openvibe-dependency-cmake-2.6.2.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_cmake:

  IfFileExists "cmake" no_need_to_install_cmake
  ZipDLL::extractall "arch\openvibe-dependency-cmake-2.6.2.zip" "cmake"

no_need_to_install_cmake:

  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
  FileSeek $0 0 END
  FileWrite $0 "SET OV_DEP_CMAKE=$INSTDIR\cmake\cmake-2.6.2-win32-x86$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "eXpat"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-expat-2.0.1.zip" no_need_to_download_expat
  NSISdl::download http://www.irisa.fr/bunraku/OpenViBE/dependencies/win32/expat-win32bin-2.0.1.zip "arch\openvibe-dependency-expat-2.0.1.zip"
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
  FileWrite $0 "SET OV_DEP_EXPAT=$INSTDIR\expat$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "BOOST"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-boost-1.34.0.zip" no_need_to_download_boost
  NSISdl::download http://www.irisa.fr/bunraku/OpenViBE/dependencies/win32/boost_1_34_0.zip "arch\openvibe-dependency-boost-1.34.0.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_boost:

  IfFileExists "boost" no_need_to_install_boost
  ZipDLL::extractall "arch\openvibe-dependency-boost-1.34.0.zip" "boost"

no_need_to_install_boost:

  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
  FileSeek $0 0 END
  FileWrite $0 "SET OV_DEP_BOOST=$INSTDIR\boost\boost_1_34_0$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "GTK/Glade"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-gtk-2.10.11.zip" no_need_to_download_glade
  NSISdl::download http://www.irisa.fr/bunraku/OpenViBE/dependencies/win32/gtk-dev-2.10.11-win32-1.zip "arch\openvibe-dependency-gtk-2.10.11.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_glade:

  IfFileExists "GTK" no_need_to_install_glade
  ZipDLL::extractall "arch\openvibe-dependency-gtk-2.10.11.zip" "gtk"

no_need_to_install_glade:

  FileOpen $0 "$INSTDIR\gtk\lib\pkgconfig\libglade-2.0.pc" w
  FileWrite $0 "prefix=$INSTDIR\gtk$\n"
  FileWrite $0 "exec_prefix=$${prefix}$\n"
  FileWrite $0 "libdir=$${exec_prefix}/lib$\n"
  FileWrite $0 "includedir=$${prefix}/include$\n"
  FileWrite $0 "$\n"
  FileWrite $0 "# so people can do $\"pkg-config --variable moduledir libglade-2.0$\"$\n"
  FileWrite $0 "# (which is broken - users can now use the LIBGLADE_MODULE_PATH$\n"
  FileWrite $0 "# environment variable$\n"
  FileWrite $0 "moduledir=$${libdir}/libglade/2.0$\n"
  FileWrite $0 "$\n"
  FileWrite $0 "Name: Libglade$\n"
  FileWrite $0 "Description: a library for dynamically loading GLADE interface files$\n"
  FileWrite $0 "Version: 2.5.1$\n"
  FileWrite $0 "Requires: gtk+-2.0 libxml-2.0$\n"
  FileWrite $0 "Libs: -L$${libdir} -lglade-2.0$\n"
  FileWrite $0 "Cflags: -I$${includedir}/libglade-2.0$\n"
  FileClose $0

  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
  FileSeek $0 0 END
  FileWrite $0 "SET OV_DEP_GLADE=$INSTDIR\gtk$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "IT++"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-itpp-4.0.1.zip" no_need_to_download_itpp
  NSISdl::download http://www.irisa.fr/bunraku/OpenViBE/dependencies/win32/itpp-4.0.1.zip "arch\openvibe-dependency-itpp-4.0.1.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_itpp:

  IfFileExists "itpp" no_need_to_install_itpp
  ZipDLL::extractall "arch\openvibe-dependency-itpp-4.0.1.zip" "itpp"

no_need_to_install_itpp:

  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
  FileSeek $0 0 END
  FileWrite $0 "SET OV_DEP_ITPP=$INSTDIR\itpp$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "OBT"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-obt-22.zip" no_need_to_download_obt
  NSISdl::download http://www.irisa.fr/bunraku/OpenViBE/dependencies/win32/obt-22.zip "arch\openvibe-dependency-obt-22.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_obt:

  IfFileExists "obt" no_need_to_install_obt
  ZipDLL::extractall "arch\openvibe-dependency-obt-22.zip" "obt"

no_need_to_install_obt:

  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
  FileSeek $0 0 END
  FileWrite $0 "SET OV_DEP_OBT=$INSTDIR\obt$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "Ogre3D"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-ogre-1.4.9.zip" no_need_to_download_ogre
  NSISdl::download http://www.irisa.fr/bunraku/OpenViBE/dependencies/win32/ogre-1.4.9.zip "arch\openvibe-dependency-ogre-1.4.9.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_ogre:

  IfFileExists "ogre" no_need_to_install_ogre
  ZipDLL::extractall "arch\openvibe-dependency-ogre-1.4.9.zip" "ogre"

no_need_to_install_ogre:

  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
  FileSeek $0 0 END
  FileWrite $0 "SET OV_DEP_OGRE=$INSTDIR\ogre$\n"
  FileWrite $0 "SET OGRE_HOME=$INSTDIR\ogre$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "OpenMASK 4 / DotSceneLoader"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-openmask4~dotsceneloader-164.zip" no_need_to_download_openmask
  NSISdl::download http://www.irisa.fr/bunraku/OpenViBE/dependencies/win32/openmask4~dotsceneloader-164.zip "arch\openvibe-dependency-openmask4~dotsceneloader-164.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_openmask:

  IfFileExists "openmask" no_need_to_install_openmask
  ZipDLL::extractall "arch\openvibe-dependency-openmask4~dotsceneloader-164.zip" "openmask"

no_need_to_install_openmask:

  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
  FileSeek $0 0 END
  FileWrite $0 "SET OV_DEP_OPENMASK=$INSTDIR\openmask$\n"
  FileWrite $0 "SET OV_DEP_DSI=$INSTDIR\openmask$\n"
  FileWrite $0 "SET OMK_HOME=$INSTDIR\openmask$\n"
  FileWrite $0 "SET OMK_BIN=$INSTDIR\openmask$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "VRPN"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-vrpn-7.73.zip" no_need_to_download_vrpn
  NSISdl::download http://www.irisa.fr/bunraku/OpenViBE/dependencies/win32/vrpn-7.13.zip "arch\openvibe-dependency-vrpn-7.13.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_vrpn:

  IfFileExists "vrpn" no_need_to_install_vrpn
  ZipDLL::extractall "arch\openvibe-dependency-vrpn-7.13.zip" "vrpn"

no_need_to_install_vrpn:

  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
  FileSeek $0 0 END
  FileWrite $0 "SET OV_DEP_VRPN=$INSTDIR\vrpn$\n"
  FileWrite $0 "SET VRPNROOT=$INSTDIR\vrpn$\n"
  FileClose $0

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
  RMDir /r "$INSTDIR\obt"
  RMDir /r "$INSTDIR\ogre"
  RMDir /r "$INSTDIR\openmask"
  RMDir /r "$INSTDIR\vrpn"

  Delete "$INSTDIR\..\scripts\win32-dependencies.cmd"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

SectionEnd
