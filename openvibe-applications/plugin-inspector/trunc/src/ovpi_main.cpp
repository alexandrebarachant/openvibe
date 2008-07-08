#include "ovpiCPluginObjectDescEnumAlgorithmGlobalDefinesGenerator.h"
#include "ovpiCPluginObjectDescEnumBoxAlgorithmSnapshotGenerator.h"

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace std;

int main(int argc, char ** argv)
{
//___________________________________________________________________//
//                                                                   //

	IKernelLoader* l_pKernelLoader=OpenViBE::createKernelLoader();
	if(l_pKernelLoader)
	{
		cout<<"[  INF  ] Created kernel loader"<<endl;
		CString m_sError;
#ifdef OVD_OS_Windows
		if(!l_pKernelLoader->load("../lib/OpenViBE-kernel-dynamic.dll", &m_sError))
#else
		if(!l_pKernelLoader->load("../lib/libOpenViBE-kernel-dynamic.so", &m_sError))
#endif
		{
			cout<<"[ FAILED ] Error loading kernel ("<<m_sError<<")"<<endl;
		}
		else
		{
			cout<<"[  INF  ] Kernel module loaded"<<endl;
			IKernelDesc* l_pKernelDesc=NULL;
			IKernel* l_pKernel=NULL;
			l_pKernelLoader->initialize();
			l_pKernelLoader->getKernelDesc(l_pKernelDesc);
			if(!l_pKernelDesc)
			{
				cout<<"[ FAILED ] No kernel descriptor"<<endl;
			}
			else
			{
				cout<<"[  INF  ] Found kernel descriptor"<<endl;
				l_pKernel=l_pKernelDesc->create();
				if(!l_pKernel)
				{
					cout<<"[ FAILED ] No kernel created by kernel descriptor"<<endl;
				}
				else
				{
					cout<<"[  INF  ] Created Kernel, going on testing"<<endl;

					OpenViBEToolkit::initialize(*l_pKernel->getContext());

					ILogManager& l_rLogManager=l_pKernel->getContext()->getLogManager();
					l_rLogManager.activate(LogLevel_Debug, false);
					l_rLogManager.activate(LogLevel_Benchmark, false);
					l_rLogManager.activate(LogLevel_Trace, true);
					l_rLogManager.activate(LogLevel_Info, true);
					l_rLogManager.activate(LogLevel_Warning, true);
					l_rLogManager.activate(LogLevel_ImportantWarning, true);
					l_rLogManager.activate(LogLevel_Error, true);
					l_rLogManager.activate(LogLevel_Fatal, true);

					IPluginManager& l_rPluginManager=l_pKernel->getContext()->getPluginManager();
					l_rPluginManager.addPluginsFromFiles("../lib/libOpenViBE-Plugins-*.so");
					l_rPluginManager.addPluginsFromFiles("../lib/libOpenViBE-*.so");
					l_rPluginManager.addPluginsFromFiles("../lib/OpenViBE-Plugins-*.dll");
					l_rPluginManager.addPluginsFromFiles("../lib/OpenViBE-*.dll");

					{
						CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator l_oGlobalDefinesGenerator(*l_pKernel->getContext(), "ovp_global_defines.h");
						l_oGlobalDefinesGenerator.enumeratePluginObjectDesc(OV_ClassId_Plugins_AlgorithmDesc);
					}

					{
						CPluginObjectDescEnumBoxAlgorithmSnapshotGenerator l_oBoxAlgorithmSnapshotGenerator(*l_pKernel->getContext());
						l_oBoxAlgorithmSnapshotGenerator.enumeratePluginObjectDesc(OV_ClassId_Plugins_BoxAlgorithmDesc);
					}

					cout<<"[  INF  ] Everything finished, realeasing objects"<<endl;

					OpenViBEToolkit::uninitialize(*l_pKernel->getContext());

					l_pKernel->release();
				}
				l_pKernelDesc->release();
			}
			l_pKernelLoader->uninitialize();
			l_pKernelLoader->unload();
		}
		l_pKernelLoader->release();
	}

	return 0;
}
