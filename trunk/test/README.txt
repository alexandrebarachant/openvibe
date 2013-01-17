This directory contains scripts and configuration files for automatic checkout sources, building and potentially run tests of openViBe. This files need to be placed in $HOME directory of test account.
After every test a Cdash rapport is emitted to OpenViBe Cdash server :

http://cdash.inria.fr/CDash/index.php?project=OpenViBe

Dependencies: 

to run these scripts, you need ctest (cmake suit), sh-utils (coreutils on windows cygwin or gnuwin32) and svn command-line tools

Run:

for a Nightly test:

ctest -VV -S ~/openVibeTests.cmake,Nightly 

for a experimental test:

ctest -VV -S ~/openVibeTests.cmake,Experimental 
