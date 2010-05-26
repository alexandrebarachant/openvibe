#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <iostream>

#include "ovsgCSkeletonGenerator.h"

#if defined TARGET_OS_Windows
 #define OVSG_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVSG_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVSG_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESkeletonGenerator;

using namespace std;


int main(int argc, char** argv)
{

	CKernelLoader l_oKernelLoader;

	cout<<"[  INF  ] Created kernel loader, trying to load kernel module"<<endl;
	CString m_sError;
#if defined OVSG_OS_Windows
	if(!l_oKernelLoader.load("../bin/OpenViBE-kernel-dynamic.dll", &m_sError))
#else
	if(!l_oKernelLoader.load("../lib/libOpenViBE-kernel-dynamic.so", &m_sError))
#endif
	{
			cout<<"[ FAILED ] Error loading kernel ("<<m_sError<<")"<<endl;
	}
	else
	{
		cout<<"[  INF  ] Kernel module loaded, trying to get kernel descriptor"<<endl;
		IKernelDesc* l_pKernelDesc=NULL;
		IKernelContext* l_pKernelContext=NULL;
		l_oKernelLoader.initialize();
		l_oKernelLoader.getKernelDesc(l_pKernelDesc);
		if(!l_pKernelDesc)
		{
			cout<<"[ FAILED ] No kernel descriptor"<<endl;
		}
		else
		{
			cout<<"[  INF  ] Got kernel descriptor, trying to create kernel"<<endl;
			l_pKernelContext=l_pKernelDesc->createKernel("skeleton-generator", "../share/openvibe.conf");
			if(!l_pKernelContext)
			{
				cout<<"[ FAILED ] No kernel created by kernel descriptor"<<endl;
			}
			else
			{
				
				//initialise Gtk before 3D context
				gtk_init(&argc, &argv);
	
				CSkeletonGenerator l_Generator(*l_pKernelContext,"../share/openvibe-applications/skeleton-generator/generator-interface.glade");;

				l_Generator.go();
			}
		}
	}

	return 0;
}