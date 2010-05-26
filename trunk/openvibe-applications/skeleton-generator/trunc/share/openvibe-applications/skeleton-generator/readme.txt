******************************************************
* Thanks for using the OpenViBE Skeleton Generator ! *
******************************************************

WARNING:
Writing a new driver for an EEG device is hardly dependent on hardware specifications, such as OS compatibility
or communication protocol. The generator gives you only the basic skeletons that , please refer to your device specifications
(API, protocol description, etc.) for the


The generator produced the following files:

The Driver class:
- ovasCDriver@@ClassName@@.h
- ovasCDriver@@ClassName@@.cpp
Please put these files in your local repository.
If you have a branch in the repository, put the driver files in a dedicated directory:
     [openvibe-repository]/trunk/openvibe-applications/acquisition-server/branches/[my-branch-name]/src/[my-device]/
If you don't use a branch, put the driver files in a dedicated directory:
     [openvibe-repository]/trunk/openvibe-applications/acquisition-server/trunk/src/[my-device]/

The configuration class:
- ovasCConfiguration@@ClassName@@.h
- ovasCConfiguration@@ClassName@@.cpp
Please put these files in your local repository.
If you have a branch in the repository, put the configuration files in a dedicated directory:
     [openvibe-repository]/trunk/openvibe-applications/acquisition-server/branches/[my-branch-name]/src/[my-device]/
If you don't use a branch, put the configuration files in in a dedicated directory:
     [openvibe-repository]/trunk/openvibe-applications/acquisition-server/trunk/src/[my-device]/

The glade interface:
- interface-@@ClassName@@.glade
Please put these files in your local repository.
If you have a branch in the repository, put the driver files in:
     [openvibe-repository]/trunk/openvibe-applications/acquisition-server/branches/[my-branch-name]/share/openvibe-applications/acquisition-server/
If you don't use a branch, put the sriver files in:
     [openvibe-repository]/trunk/openvibe-applications/acquisition-server/trunk/share/openvibe-applications/acquisition-server/


For more information about building a new driver and filling the blancks in your skeleton, please read the official tutorial:
http://openvibe.inria.fr/documentation/unstable/Doc_CreatingANewDriver.html
