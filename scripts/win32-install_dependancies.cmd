@echo off

echo > NULL

REM ####################################
REM ##
REM ##  switches to extended command line mode
REM ##
REM ####################################

set _expand_=test
if "!_expand_!" == "test" goto ok
cmd /v /c %0
goto terminate

:ok

set ov_wget=%CD%\wget

pushd ..

set ov_directory=%CD%

set ov_depend_folder_rel=dependancies
set ov_archives_folder_rel=arch
set ov_tmp_folder_rel=tmp
set ov_log_folder_rel=log
set ov_installed_folder_rel=installed

set ov_depend_folder=%ov_directory%\%ov_depend_folder_rel%
set ov_archives_folder=%ov_depend_folder%\%ov_archives_folder_rel%
set ov_tmp_folder=%ov_depend_folder%\%ov_tmp_folder_rel%
set ov_log_folder=%ov_depend_folder%\%ov_log_folder_rel%
set ov_installed_folder=%ov_depend_folder%\%ov_installed_folder_rel%

REM ####################################
REM ##
REM ##  initializes some env variables
REM ##
REM ####################################

set ov_should_install_gtk=true
set ov_should_install_boost=true
set ov_should_install_expat=true

set ov_dep_gtk_url=http://downloads.sourceforge.net/gladewin32/gtk-dev-2.10.11-win32-1.exe
set ov_dep_boost_url=http://www.boost-consulting.com/boost_1_34_0_setup.exe
set ov_dep_expat_url=http://downloads.sourceforge.net/expat/expat-win32bin-2.0.1.exe

set ov_dep_gtk_archive_name=%ov_archives_folder%\openvibe-dependancy-gtk-2.10.11.exe
set ov_dep_boost_archive_name=%ov_archives_folder%\openvibe-dependancy-boost-1.34.0.exe
set ov_dep_expat_archive_name=%ov_archives_folder%\openvibe-dependancy-expat-2.0.1.exe

REM ####################################
REM ##
REM ##  builds up tree structure
REM ##
REM ####################################

md %ov_depend_folder%     > NULL 2<&1
md %ov_archives_folder%   > NULL 2<&1
md %ov_tmp_folder%        > NULL 2<&1
md %ov_log_folder%        > NULL 2<&1
md %ov_installed_folder%  > NULL 2<&1

REM ####################################
REM ##
REM ##  gtk
REM ##
REM ####################################

echo Trying to install package [gtk]...
echo . Downloading package [%ov_dep_gtk_url%]...
%ov_wget% -c -O %ov_dep_gtk_archive_name% %ov_dep_gtk_url%           >> %ov_log_folder%\log-gtk.log   2<&1
echo . Installing package [%ov_dep_gtk_archive_name%]...
%ov_dep_gtk_archive_name% /S /D=%ov_installed_folder%\GTK            >> %ov_log_folder%\log-gtk.log   2<&1

REM ####################################
REM ##
REM ##  expat
REM ##
REM ####################################

echo Trying to install package [expat]...
echo . Downloading package [%ov_dep_expat_url%]...
%ov_wget% -c -O %ov_dep_expat_archive_name% %ov_dep_expat_url%       >> %ov_log_folder%\log-expat.log 2<&1
echo . Installing package [%ov_dep_expat_archive_name%]...
%ov_dep_expat_archive_name% /SILENT /DIR=%ov_installed_folder%\expat >> %ov_log_folder%\log-expat.log 2<&1


REM ####################################
REM ##
REM ##  boost
REM ##
REM ####################################

echo Trying to install package [boost]...
echo . Downloading package [%ov_dep_boost_url%]...
%ov_wget% -c -O %ov_dep_boost_archive_name% %ov_dep_boost_url%       >> %ov_log_folder%\log-boost.log 2<&1
echo . Installing package [%ov_dep_boost_archive_name%]...
%ov_dep_boost_archive_name%                                          >> %ov_log_folder%\log-boost.log 2<&1

:terminate

del NULL

popd
