This directory contains scripts and configuration files for automatic checkout sources, building and potentially run tests of openViBe. This files need to be placed in $HOME directory of test account.
After every test a Cdash rapport is emitted to OpenViBe Cdash server :

http://cdash.inria.fr/CDash/index.php?project=OpenViBe

Dependencies: 
------------

to run these scripts, you need ctest (cmake suit), sh-utils (coreutils on windows cygwin or gnuwin32), svn command-line tools and a c++ compiler (C++ Visual Studio 2010 on windows) 

On Linux fedora you need to install :
yum install cmake svn redhat-lsb gcc-c++ expect

SUDOER on Linux :
------
On linux, you need to have an account with sodoers privileges without password

WARNING 1 : be careful, this not good for your personal computer or server, it must be reserved for unsafe slaves machines used to run automatic tests

launch:
VISUAL=/usr/bin/vi visudo
then put this kind of line and the end of the file:
user ALL=(ALL) NOPASSWD: ALL


WARNING 2 : execution of test change de root password in Fedora distributions to : openvibe 


Get tests scripts:
-----------------
svn checkout svn://scm.gforge.inria.fr/svnroot/openvibe/trunk/test
cd test



Run test:
--------

 - for a Nightly test:

ctest -VV -S openVibeTests.cmake,Nightly 

 - for a experimental test:

ctest -VV -S openVibeTests.cmake,Experimental 
