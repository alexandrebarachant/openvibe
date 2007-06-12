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

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  ;clears dependencies file
  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" w
  FileWrite $0 "@echo off$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section /o "CMake"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-cmake-2.4.6.zip" no_need_to_download_cmake
  NSISdl::download http://www.cmake.org/files/v2.4/cmake-2.4.6-win32-x86.zip "arch\openvibe-dependency-cmake-2.4.6.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_cmake:
 
  IfFileExists "cmake" no_need_to_install_cmake
  ZipDLL::extractall "arch\openvibe-dependency-cmake-2.4.6.zip" "cmake"

no_need_to_install_cmake:

  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
  FileSeek $0 0 END
  FileWrite $0 "SET OV_DEP_CMAKE=$INSTDIR\cmake\cmake-2.4.6-win32-x86$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section /o "eXpat"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-expat-2.0.1.zip" no_need_to_download_expat
  ; NSISdl::download http://downloads.sourceforge.net/expat/expat-win32bin-2.0.1.exe "arch\openvibe-dependency-expat-2.0.1.exe"
  NSISdl::download http://url "arch\openvibe-dependency-expat-2.0.1.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_expat:
 
  IfFileExists "expat" no_need_to_install_expat
  ; ExecWait '"$INSTDIR\arch\openvibe-dependency-expat-2.0.1.exe" /silent "/dir=$INSTDIR\expat"'
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

Section /o "BOOST"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-boost-1.34.0.zip" no_need_to_download_boost
  NSISdl::download http://downloads.sourceforge.net/boost/boost_1_34_0.zip "arch\openvibe-dependency-boost-1.34.0.zip"
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

Section /o "GTK/Glade"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-gtk-2.10.11.zip" no_need_to_download_glade
  ; NSISdl::download http://downloads.sourceforge.net/gladewin32/gtk-dev-2.10.11-win32-1.exe "arch\openvibe-dependency-gtk-2.10.11.exe"
  NSISdl::download http://url "arch\openvibe-dependency-gtk-2.10.11.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_glade:
 
  IfFileExists "GTK" no_need_to_install_glade
  ZipDLL::extractall "arch\openvibe-dependency-gtk-2.10.11.zip" "gtk"
  ; ExecWait '"$INSTDIR\arch\openvibe-dependency-gtk-2.10.11.exe" /S /D=$INSTDIR\GTK'

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

Section /o "OpenMASK"

  SetOutPath "$INSTDIR"
  CreateDirectory "$INSTDIR\arch"

  IfFileExists "arch\openvibe-dependency-openmask-580.zip" no_need_to_download_openmask
  NSISdl::download http://url "arch\openvibe-dependency-openmask-580.zip"
  Pop $R0 ; Get the return value
    StrCmp $R0 "success" +3
      MessageBox MB_OK "Download failed: $R0"
      Quit

no_need_to_download_openmask:
 
  IfFileExists "openmask" no_need_to_install_openmask
  ZipDLL::extractall "arch\openvibe-dependency-openmask-580.zip" "openmask"

no_need_to_install_openmask:

  FileOpen $0 "$EXEDIR\win32-dependencies.cmd" a
  FileSeek $0 0 END
  FileWrite $0 "SET OV_DEP_OPENMASK=$INSTDIR\openmask$\n"
  FileWrite $0 "SET OpenMASKDIR=$INSTDIR\openmask$\n"
  FileClose $0

SectionEnd

;##########################################################################################################################################################
;##########################################################################################################################################################
;##########################################################################################################################################################

Section "Uninstall"

  ; ExecWait '"$INSTDIR\GTK\uninst.exe" /s'
  ; ExecWait '"$INSTDIR\expat\Uninstall\unins000.exe" /silent'

  RMDir /r "$INSTDIR\gtk"
  RMDir /r "$INSTDIR\boost"
  RMDir /r "$INSTDIR\expat"
  RMDir /r "$INSTDIR\cmake"
  RMDir /r "$INSTDIR\openmask"

  Delete "$INSTDIR\..\scripts\win32-dependencies.cmd"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

SectionEnd
