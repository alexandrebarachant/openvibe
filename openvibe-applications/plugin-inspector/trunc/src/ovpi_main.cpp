#include "ovpi_base.h"

#include <string>
#include <iostream>
#include <stdio.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace std;

CString transform(const CString& sInput)
{
	std::string l_sInput(sInput);
	std::string l_sOutput;
	bool l_bLastWasSeparator=false;

	for(std::string::size_type i=0; i<l_sInput.length(); i++)
	{
		if((l_sInput[i]>='a' && l_sInput[i]<='z') || (l_sInput[i]>='A' && l_sInput[i]<='Z') || (l_sInput[i]>='0' && l_sInput[i]<='9'))
		{
			if(l_bLastWasSeparator)
			{
				if('a' <= l_sInput[i] && l_sInput[i] <= 'z')
				{
					l_sOutput+=l_sInput[i]+'A'-'a';
				}
				else
				{
					l_sOutput+=l_sInput[i];
				}
				l_bLastWasSeparator=false;
			}
			else
			{
				l_sOutput+=l_sInput[i];
			}
		}
		else
		{
			l_bLastWasSeparator=true;
		}
	}
	return l_sOutput.c_str();
}

// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

class CPluginObjectDescEnum
{
public:

	CPluginObjectDescEnum(const IKernelContext& rKernelContext) : m_rKernelContext(rKernelContext) { }
	virtual ~CPluginObjectDescEnum(void) { }

	virtual boolean enumeratePluginObjectDesc(void)
	{
		CIdentifier l_oIdentifier;
		while((l_oIdentifier=m_rKernelContext.getPluginManager().getNextPluginObjectDescIdentifier(l_oIdentifier))!=OV_UndefinedIdentifier)
		{
			this->callback(*m_rKernelContext.getPluginManager().getPluginObjectDesc(l_oIdentifier));
		}
		return true;
	}

	virtual boolean enumeratePluginObjectDesc(
		const CIdentifier& rParentClassIdentifier)
	{
		CIdentifier l_oIdentifier;
		while((l_oIdentifier=m_rKernelContext.getPluginManager().getNextPluginObjectDescIdentifier(l_oIdentifier, rParentClassIdentifier))!=OV_UndefinedIdentifier)
		{
			this->callback(*m_rKernelContext.getPluginManager().getPluginObjectDesc(l_oIdentifier));
		}
		return true;
	}

	virtual boolean callback(
		const Plugins::IPluginObjectDesc& rPluginObjectDesc)=0;

protected:

	const IKernelContext& m_rKernelContext;
};

// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

class CHeaderGenerator : public CPluginObjectDescEnum
{
public:

	CHeaderGenerator(const IKernelContext& rKernelContext, const char* sFilename)
		:CPluginObjectDescEnum(rKernelContext)
		,m_pFile(NULL)
	{
		m_pFile=fopen(sFilename, "wt");
		if(!m_pFile) return;

		fprintf(m_pFile, "#ifndef __OpenViBEPlugins_Global_Defines_H__\n");
		fprintf(m_pFile, "#define __OpenViBEPlugins_Global_Defines_H__\n");
		fprintf(m_pFile, "\n");
	}

	virtual ~CHeaderGenerator(void)
	{
		if(!m_pFile) return;

		fprintf(m_pFile, "#endif // __OpenViBEPlugins_Global_Defines_H__\n");
		fclose(m_pFile);
	}

	virtual boolean callback(const Plugins::IPluginObjectDesc& rPluginObjectDesc)
	{
		if(!m_pFile) return true;

		int l_iWidth=120;

		m_rKernelContext.getLogManager() << LogLevel_Info << "Dumping [" << rPluginObjectDesc.getName() << "]\n";

		IAlgorithmManager& l_rAlgorithmManager=m_rKernelContext.getAlgorithmManager();
		IAlgorithmProxy& l_rAlgorithmProxy=l_rAlgorithmManager.getAlgorithm(l_rAlgorithmManager.createAlgorithm(rPluginObjectDesc.getCreatedClass()));
		CIdentifier l_oIdentifier;

		fprintf(m_pFile, "// ----------------------------------------------------\n");
		fprintf(m_pFile, "// %s\n", rPluginObjectDesc.getName().toASCIIString());
		fprintf(m_pFile, "// ----------------------------------------------------\n");
		fprintf(m_pFile, "\n");

		{
			std::string l_sDefName;
			l_sDefName+="OVP_GD_ClassId_Algorithm_";
			l_sDefName+=transform(rPluginObjectDesc.getName());
			l_sDefName+="";

			std::string l_sSpaces;
			l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');

			fprintf(m_pFile, "#define %s%s%s\n", l_sDefName.c_str(), l_sSpaces.c_str(), rPluginObjectDesc.getCreatedClassIdentifier().toString().toASCIIString());
		}

		{
			std::string l_sDefName;
			l_sDefName+="OVP_GD_ClassId_Algorithm_";
			l_sDefName+=transform(rPluginObjectDesc.getName());
			l_sDefName+="Desc";

			std::string l_sSpaces;
			l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');

			fprintf(m_pFile, "#define %s%s%s\n", l_sDefName.c_str(), l_sSpaces.c_str(), rPluginObjectDesc.getClassIdentifier().toString().toASCIIString());
		}

		while((l_oIdentifier=l_rAlgorithmProxy.getNextInputParameterIdentifier(l_oIdentifier))!=OV_UndefinedIdentifier)
		{
			std::string l_sDefName;
			l_sDefName+="OVP_GD_Algorithm_";
			l_sDefName+=transform(rPluginObjectDesc.getName());
			l_sDefName+="_InputParameterId_";
			l_sDefName+=transform(l_rAlgorithmProxy.getInputParameterName(l_oIdentifier));

			std::string l_sSpaces;
			l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');

			fprintf(m_pFile, "#define %s%s%s\n", l_sDefName.c_str(), l_sSpaces.c_str(), l_oIdentifier.toString().toASCIIString());
		}

		while((l_oIdentifier=l_rAlgorithmProxy.getNextOutputParameterIdentifier(l_oIdentifier))!=OV_UndefinedIdentifier)
		{
			std::string l_sDefName;
			l_sDefName+="OVP_GD_Algorithm_";
			l_sDefName+=transform(rPluginObjectDesc.getName());
			l_sDefName+="_OutputParameterId_";
			l_sDefName+=transform(l_rAlgorithmProxy.getOutputParameterName(l_oIdentifier));

			std::string l_sSpaces;
			l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');

			fprintf(m_pFile, "#define %s%s%s\n", l_sDefName.c_str(), l_sSpaces.c_str(), l_oIdentifier.toString().toASCIIString());
		}

		while((l_oIdentifier=l_rAlgorithmProxy.getNextInputTriggerIdentifier(l_oIdentifier))!=OV_UndefinedIdentifier)
		{
			std::string l_sDefName;
			l_sDefName+="OVP_GD_Algorithm_";
			l_sDefName+=transform(rPluginObjectDesc.getName());
			l_sDefName+="_InputTriggerId_";
			l_sDefName+=transform(l_rAlgorithmProxy.getInputTriggerName(l_oIdentifier));

			std::string l_sSpaces;
			l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');

			fprintf(m_pFile, "#define %s%s%s\n", l_sDefName.c_str(), l_sSpaces.c_str(), l_oIdentifier.toString().toASCIIString());
		}

		while((l_oIdentifier=l_rAlgorithmProxy.getNextOutputTriggerIdentifier(l_oIdentifier))!=OV_UndefinedIdentifier)
		{
			std::string l_sDefName;
			l_sDefName+="OVP_GD_Algorithm_";
			l_sDefName+=transform(rPluginObjectDesc.getName());
			l_sDefName+="_OutputTriggerId_";
			l_sDefName+=transform(l_rAlgorithmProxy.getOutputTriggerName(l_oIdentifier));

			std::string l_sSpaces;
			l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');

			fprintf(m_pFile, "#define %s%s%s\n", l_sDefName.c_str(), l_sSpaces.c_str(), l_oIdentifier.toString().toASCIIString());
		}

		fprintf(m_pFile, "\n");

		return true;
	}

protected:

	::FILE* m_pFile;
};

// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

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
						CHeaderGenerator l_oHeaderGenerator(*l_pKernel->getContext(), "ovp_global_defines.h");
						l_oHeaderGenerator.enumeratePluginObjectDesc(OV_ClassId_Plugins_AlgorithmDesc);
					}

					// l_rLogManager << LogLevel_Info << "Dumping box algorithms...\n";
					// l_oHeaderGenerator.enumeratePluginObjectDesc(OV_ClassId_Plugins_BoxAlgorithmDesc);

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
