#include "ovpiCPluginObjectDescEnumAlgorithmGlobalDefinesGenerator.h"
#include "ovpiCPluginObjectDescEnumBoxAlgorithmSnapshotGenerator.h"
#include "ovpiCPluginObjectDescEnumAlgorithmSnapshotGenerator.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace std;

typedef struct _SConfiguration
{
	_SConfiguration(void) :
		m_bKernelPathOverload(false),
		m_bLoadPluginsFromPaths(false),
		m_bDumpPathOverload(false),
		m_bConfigPathOverload(false)
	{}

	bool m_bKernelPathOverload;
	bool m_bLoadPluginsFromPaths;
	bool m_bDumpPathOverload;
	bool m_bConfigPathOverload;
	std::string m_sKernelPath;
	std::string m_sDumpPath;
	std::string m_sConfigPath;
	std::vector<std::string> m_vPluginPaths;
} SConfiguration;



boolean parse_arguments(int argc, char** argv, SConfiguration& rConfiguration)
{
	int i;
	std::vector < std::string > l_vArgValue;
	std::vector < std::string >::const_iterator it;

	for(i=1; i<argc; i++)
	{
		l_vArgValue.push_back(argv[i]);
	}

	for(it=l_vArgValue.begin(); it!=l_vArgValue.end(); it++)
	{
		if (*it=="-h" || *it=="--help")
		{
			std::cout << "Usage: " << argv[0] << " [-p <dir1:dir2...>] [-d <dump_path>]" << std::endl;
			return true;
		}
		// get a list of folders to load plugins from
		else if (*it=="-p")
		{
			if (it + 1 != l_vArgValue.end())
			{
				std::string l_sPluginDirectories = *++it;

				// split the argument to separate paths, the delimiter is "#"
				std::stringstream ss(l_sPluginDirectories);
				std::string item;
				while(std::getline(ss, item, '#'))
				{
					rConfiguration.m_vPluginPaths.push_back(item);
				}
				rConfiguration.m_bLoadPluginsFromPaths = true;
			}
			else
			{
				return false;
			}
		}
		// Configuration path
		else if (*it=="-c")
		{
			if (it + 1 != l_vArgValue.end())
			{
				rConfiguration.m_bConfigPathOverload = true;
				rConfiguration.m_sConfigPath = *++it;
			}
			else
			{
				return false;
			}
		}
		// Kernel path
		else if (*it=="-k")
		{
			if (it + 1 != l_vArgValue.end())
			{
				rConfiguration.m_bKernelPathOverload = true;
				rConfiguration.m_sKernelPath = *++it;
			}
			else
			{
				return false;
			}
		}
		// Dump path
		else if (*it=="-d")
		{
			if (it + 1 != l_vArgValue.end())
			{
				rConfiguration.m_bDumpPathOverload = true;
				rConfiguration.m_sDumpPath = *++it;
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}



int main(int argc, char ** argv)
{
//___________________________________________________________________//
//                                                                   //
	SConfiguration l_oConfiguration;

	if (!parse_arguments(argc, argv, l_oConfiguration))
	{
		std::cout << "Error parsing arguments" << std::endl;
	}

	CKernelLoader l_oKernelLoader;

	cout<<"[  INF  ] Created kernel loader, trying to load kernel module"<<endl;
	CString l_sError;
#if defined TARGET_OS_Windows
	CString l_sKernelFile = "/openvibe-kernel.dll";
#else
	CString l_sKernelFile = "/libopenvibe-kernel.so";
#endif

	if (l_oConfiguration.m_bKernelPathOverload)
	{
		l_sKernelFile = CString(l_oConfiguration.m_sKernelPath.c_str()) + l_sKernelFile;
	}
	else
	{
		l_sKernelFile = OpenViBE::Directories::getLibDir() + l_sKernelFile;
	}

	if(!l_oKernelLoader.load(l_sKernelFile, &l_sError))
	{
			cout<<"[ FAILED ] Error loading kernel ("<<l_sError<<")" << " from [" << l_sKernelFile << "]\n";
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
			CString l_sConfigPath = OpenViBE::Directories::getDataDir() + "/openvibe.conf";
			if (l_oConfiguration.m_bConfigPathOverload)
			{
				l_sConfigPath = CString(l_oConfiguration.m_sConfigPath.c_str());
			}

			l_pKernelContext=l_pKernelDesc->createKernel("plugin-inspector", l_sConfigPath );
			if(!l_pKernelContext)
			{
				cout<<"[ FAILED ] No kernel created by kernel descriptor"<<endl;
			}
			else
			{
				OpenViBEToolkit::initialize(*l_pKernelContext);

				IConfigurationManager& l_rConfigurationManager=l_pKernelContext->getConfigurationManager();

				if (l_oConfiguration.m_bLoadPluginsFromPaths)
				{
					l_pKernelContext->getLogManager() << LogLevel_Info << "Loading plugins from specified folders\n";

					std::string l_sPluginPattern;
					// Choose the right pattern for libraries to load depending on the OS
					#if defined TARGET_OS_Windows
						l_sPluginPattern = "openvibe-plugins-*.dll";
					#elif defined TARGET_OS_Linux
						l_sPluginPattern = "libopenvibe-plugins-*.so";
					#endif

					typedef std::vector<std::string>::iterator stringVectorIt;
					for (stringVectorIt it = l_oConfiguration.m_vPluginPaths.begin(); it != l_oConfiguration.m_vPluginPaths.end(); ++it)
					{
						std::string l_sPluginPath = *it + "/" + l_sPluginPattern;
						std::cout << l_sPluginPath << std::endl;
						l_pKernelContext->getPluginManager().addPluginsFromFiles(CString(l_sPluginPath.c_str()));
					}
				}
				else
				{
					l_pKernelContext->getLogManager() << LogLevel_Info << "Loading plugins as specified by kernel\n";
					l_pKernelContext->getPluginManager().addPluginsFromFiles(l_rConfigurationManager.expand("${Kernel_Plugins}"));
				}

				//initialise Gtk before 3D context
				gtk_init(&argc, &argv);
				// gtk_rc_parse(OpenViBE::Directories::getDataDir() + "/openvibe-applications/designer/interface.gtkrc");

				if(l_rConfigurationManager.expandAsBoolean("${Kernel_3DVisualisationEnabled}"))
				{
					// l_pKernelContext->getVisualisationManager().initialize3DContext();
				}

				CString l_sGlobalDefinesDirectory = "";

				CString l_sAlgorithmSnapshotDirectory = "";
				CString l_sAlgorithmDocTemplateDirectory = "";
				CString l_sBoxAlgorithmSnapshotDirectory = "";
				CString l_sBoxAlgorithmDocTemplateDirectory = "";

				if (l_oConfiguration.m_bDumpPathOverload)
				{
					l_pKernelContext->getLogManager() << LogLevel_Info << "Dumping stuff to [" << l_oConfiguration.m_sDumpPath.c_str() << "]\n";
					l_sAlgorithmSnapshotDirectory = CString(l_oConfiguration.m_sDumpPath.c_str()) + "/algorithm-snapshots";
					l_sAlgorithmDocTemplateDirectory = CString(l_oConfiguration.m_sDumpPath.c_str()) + "/algorithm-doc";
					l_sBoxAlgorithmSnapshotDirectory = CString(l_oConfiguration.m_sDumpPath.c_str()) + "/box-algorithm-snapshots";
					l_sBoxAlgorithmDocTemplateDirectory = CString(l_oConfiguration.m_sDumpPath.c_str()) + "/box-algorithm-doc";
					l_sGlobalDefinesDirectory = CString(l_oConfiguration.m_sDumpPath.c_str());
				}
				else
				{
					l_pKernelContext->getLogManager() << LogLevel_Info << "Loading paths from Kernel configuration\n";
					l_sAlgorithmSnapshotDirectory      =l_rConfigurationManager.expand("${PluginInspector_DumpAlgorithmSnapshotDirectory}");
					l_sAlgorithmDocTemplateDirectory   =l_rConfigurationManager.expand("${PluginInspector_DumpAlgorithmDocTemplateDirectory}");
					l_sBoxAlgorithmSnapshotDirectory   =l_rConfigurationManager.expand("${PluginInspector_DumpBoxAlgorithmSnapshotDirectory}");
					l_sBoxAlgorithmDocTemplateDirectory=l_rConfigurationManager.expand("${PluginInspector_DumpBoxAlgorithmDocTemplateDirectory}");
					l_sGlobalDefinesDirectory          =l_rConfigurationManager.expand("${PluginInspector_DumpGlobalDefinesDirectory}");
				}

				l_pKernelContext->getLogManager() << LogLevel_Info << "Dumping global defines...\n";
				if(l_sGlobalDefinesDirectory!=CString(""))
				{
					CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator l_oGlobalDefinesGenerator(*l_pKernelContext, l_sGlobalDefinesDirectory);
					l_oGlobalDefinesGenerator.enumeratePluginObjectDesc(OV_ClassId_Plugins_AlgorithmDesc);
				} 
				else
				{
					l_pKernelContext->getLogManager() << LogLevel_Info << "Skipped, related PluginInspector tokens are empty in openvibe.conf\n";
				}

				l_pKernelContext->getLogManager() << LogLevel_Info << "Dumping algorithm snapshots... to [" << l_sAlgorithmSnapshotDirectory << "]\n";
				if(l_sAlgorithmSnapshotDirectory!=CString("") && l_sAlgorithmDocTemplateDirectory!=CString(""))
				{
					CPluginObjectDescEnumAlgorithmSnapshotGenerator l_oAlgorithmSnapshotGenerator(*l_pKernelContext, l_sAlgorithmSnapshotDirectory, l_sAlgorithmDocTemplateDirectory);
					l_oAlgorithmSnapshotGenerator.enumeratePluginObjectDesc(OV_ClassId_Plugins_AlgorithmDesc);
				}
				else 
				{
					l_pKernelContext->getLogManager() << LogLevel_Info << "Skipped, related PluginInspector tokens are empty in openvibe.conf\n";
				}

				l_pKernelContext->getLogManager() << LogLevel_Info << "Dumping box algorithm snapshots...\n";
				if(l_sBoxAlgorithmSnapshotDirectory!=CString("") && l_sBoxAlgorithmDocTemplateDirectory!=CString(""))
				{
					CPluginObjectDescEnumBoxAlgorithmSnapshotGenerator l_oBoxAlgorithmSnapshotGenerator(*l_pKernelContext, l_sBoxAlgorithmSnapshotDirectory, l_sBoxAlgorithmDocTemplateDirectory);
					l_oBoxAlgorithmSnapshotGenerator.enumeratePluginObjectDesc(OV_ClassId_Plugins_BoxAlgorithmDesc);
				} 
				else
				{
					l_pKernelContext->getLogManager() << LogLevel_Info << "Skipped, related PluginInspector tokens are empty in openvibe.conf\n";
				}

				l_pKernelContext->getLogManager() << LogLevel_Info << "Application terminated, releasing allocated objects \n";

				OpenViBEToolkit::uninitialize(*l_pKernelContext);

				l_pKernelDesc->releaseKernel(l_pKernelContext);
			}
		}
		l_oKernelLoader.uninitialize();
		l_oKernelLoader.unload();
	}

	return 0;
}
