#include "ovd_base.h"

#include <system/Time.h>

#include <stack>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>

#include "ovdCDesignerVisualisation.h"
#include "ovdCPlayerVisualisation.h"
#include "ovdCInterfacedObject.h"
#include "ovdCInterfacedScenario.h"
#include "ovdCApplication.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEDesigner;
using namespace std;

map<uint32, ::GdkColor> g_vColors;

class CPluginObjectDescEnum
{
public:

	CPluginObjectDescEnum(const IKernelContext& rKernelContext)
		:m_rKernelContext(rKernelContext)
	{
	}

	virtual ~CPluginObjectDescEnum(void)
	{
	}

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

class CPluginObjectDescCollector : public CPluginObjectDescEnum
{
public:

	CPluginObjectDescCollector(const IKernelContext& rKernelContext)
		:CPluginObjectDescEnum(rKernelContext)
	{
	}

	virtual boolean callback(
		const Plugins::IPluginObjectDesc& rPluginObjectDesc)
	{
		string l_sFullName=string(rPluginObjectDesc.getCategory())+"/"+string(rPluginObjectDesc.getName());
		map<string, const IPluginObjectDesc* >::iterator itPluginObjectDesc=m_vPluginObjectDesc.find(l_sFullName);
		if(itPluginObjectDesc!=m_vPluginObjectDesc.end())
		{
			m_rKernelContext.getLogManager() << LogLevel_ImportantWarning << "Duplicate plugin object name " << CString(l_sFullName.c_str()) << " " << itPluginObjectDesc->second->getCreatedClass() << " and " << rPluginObjectDesc.getCreatedClass() << "\n";
		}
		m_vPluginObjectDesc[l_sFullName]=&rPluginObjectDesc;
		return true;
	}

	map<string, const IPluginObjectDesc*>& getPluginObjectDescMap(void)
	{
		return m_vPluginObjectDesc;
	}

private:

	map<string, const IPluginObjectDesc*> m_vPluginObjectDesc;
};

// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

class CPluginObjectDescLogger : public CPluginObjectDescEnum
{
public:

	CPluginObjectDescLogger(const IKernelContext& rKernelContext)
		:CPluginObjectDescEnum(rKernelContext)
	{
	}

	virtual boolean callback(
		const Plugins::IPluginObjectDesc& rPluginObjectDesc)
	{
		// Outputs plugin info to console
		m_rKernelContext.getLogManager() << LogLevel_Trace << "Plugin <" << rPluginObjectDesc.getName() << ">\n";
		m_rKernelContext.getLogManager() << LogLevel_Debug << " | Plugin category        : " << rPluginObjectDesc.getCategory() << "\n";
		m_rKernelContext.getLogManager() << LogLevel_Debug << " | Class identifier       : " << rPluginObjectDesc.getCreatedClass() << "\n";
		m_rKernelContext.getLogManager() << LogLevel_Debug << " | Author name            : " << rPluginObjectDesc.getAuthorName() << "\n";
		m_rKernelContext.getLogManager() << LogLevel_Debug << " | Author company name    : " << rPluginObjectDesc.getAuthorCompanyName() << "\n";
		m_rKernelContext.getLogManager() << LogLevel_Debug << " | Short description      : " << rPluginObjectDesc.getShortDescription() << "\n";
		m_rKernelContext.getLogManager() << LogLevel_Debug << " | Detailed description   : " << rPluginObjectDesc.getDetailedDescription() << "\n";

		return true;
	}
};

// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

static void insertPluginObjectDesc_to_GtkTreeStore(map<string, const IPluginObjectDesc*>& vPluginObjectDesc, ::GtkTreeStore* pTreeStore)
{
	map<string, const Plugins::IPluginObjectDesc*>::iterator itPluginObjectDesc;
	for(itPluginObjectDesc=vPluginObjectDesc.begin(); itPluginObjectDesc!=vPluginObjectDesc.end(); itPluginObjectDesc++)
	{
		const Plugins::IPluginObjectDesc* l_pPluginObjectDesc=itPluginObjectDesc->second;

		CString l_sStockItemName;

		const Plugins::IBoxAlgorithmDesc* l_pBoxAlgorithmDesc=dynamic_cast<const Plugins::IBoxAlgorithmDesc*>(l_pPluginObjectDesc);
		if(l_pBoxAlgorithmDesc)
		{
			l_sStockItemName=l_pBoxAlgorithmDesc->getStockItemName();
		}

		::GtkStockItem l_oStockItem;
		if(!gtk_stock_lookup(l_sStockItemName, &l_oStockItem))
		{
			l_sStockItemName=GTK_STOCK_NEW;
		}

		// Splits the plugin category
		vector<string> l_vCategory;
		string l_sCategory=string(l_pPluginObjectDesc->getCategory());
		size_t j, i=(size_t)-1;
		while((j=l_sCategory.find('/', i+1))!=string::npos)
		{
			string l_sSubCategory=string(l_sCategory, i+1, j-i-1);
			if(l_sSubCategory!=string(""))
			{
				l_vCategory.push_back(l_sSubCategory);
			}
			i=j;
		}
		if(i+1!=l_sCategory.length())
		{
			l_vCategory.push_back(string(l_sCategory, i+1, l_sCategory.length()-i-1));
		}

		// Fills plugin in the tree
		vector<string>::iterator it;
		::GtkTreeIter l_oGtkIter1;
		::GtkTreeIter l_oGtkIter2;
		::GtkTreeIter* l_pGtkIterParent=NULL;
		::GtkTreeIter* l_pGtkIterChild=&l_oGtkIter1;
		for(it=l_vCategory.begin(); it!=l_vCategory.end(); it++)
		{
			boolean l_bFound=false;
			boolean l_bValid=gtk_tree_model_iter_children(
				GTK_TREE_MODEL(pTreeStore),
				l_pGtkIterChild,
				l_pGtkIterParent)?true:false;
			while(l_bValid && !l_bFound)
			{
				gchar* l_sName=NULL;
				gboolean l_bIsPlugin;
				gtk_tree_model_get(
					GTK_TREE_MODEL(pTreeStore),
					l_pGtkIterChild,
					Resource_StringName, &l_sName,
					Resource_BooleanIsPlugin, &l_bIsPlugin,
					-1);
				if(!l_bIsPlugin && l_sName==*it)
				{
					l_bFound=true;
				}
				else
				{
					l_bValid=gtk_tree_model_iter_next(
						GTK_TREE_MODEL(pTreeStore),
						l_pGtkIterChild)?true:false;
				}
			}
			if(!l_bFound)
			{
				gtk_tree_store_append(
					GTK_TREE_STORE(pTreeStore),
					l_pGtkIterChild,
					l_pGtkIterParent);
				gtk_tree_store_set(
					GTK_TREE_STORE(pTreeStore),
					l_pGtkIterChild,
					Resource_StringName, it->c_str(),
					Resource_StringShortDescription, "",
					Resource_StringStockIcon, "gtk-directory",
					Resource_BooleanIsPlugin, false,
					-1);
			}
			if(!l_pGtkIterParent)
			{
				l_pGtkIterParent=&l_oGtkIter2;
			}
			::GtkTreeIter* l_pGtkIterSwap=l_pGtkIterChild;
			l_pGtkIterChild=l_pGtkIterParent;
			l_pGtkIterParent=l_pGtkIterSwap;
		}
		gtk_tree_store_append(
			GTK_TREE_STORE(pTreeStore),
			l_pGtkIterChild,
			l_pGtkIterParent);
		gtk_tree_store_set(
			GTK_TREE_STORE(pTreeStore),
			l_pGtkIterChild,
			Resource_StringName, (const char*)l_pPluginObjectDesc->getName(),
			Resource_StringShortDescription, (const char*)l_pPluginObjectDesc->getShortDescription(),
			Resource_StringIdentifier, (const char*)l_pPluginObjectDesc->getCreatedClass().toString(),
			Resource_StringStockIcon, (const char*)l_sStockItemName,
			Resource_BooleanIsPlugin, true,
			-1);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

char** g_argv;
int g_argc;

// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

int go(int argc, char ** argv)
{
	g_argc=argc;
	g_argv=argv;

	/*
		{ 0,     0,     0,     0 },
		{ 0, 16383, 16383, 16383 },
		{ 0, 32767, 32767, 32767 },
		{ 0, 49151, 49151, 49151 },
		{ 0, 65535, 65535, 65535 },
	*/

	#define gdk_color_set(c, r, g, b) { c.pixel=0; c.red=r; c.green=g; c.blue=b; }
	gdk_color_set(g_vColors[Color_BackgroundPlayerStarted], 32767, 32767, 32767);
	gdk_color_set(g_vColors[Color_BoxBackgroundSelected],   65535, 65535, 49151);
	gdk_color_set(g_vColors[Color_BoxBackgroundMissing],    49151, 32767, 32767);
	gdk_color_set(g_vColors[Color_BoxBackgroundObsolete],   32767, 49151, 49151);
	gdk_color_set(g_vColors[Color_BoxBackground],           65535, 65535, 65535);
	gdk_color_set(g_vColors[Color_BoxBorderSelected],           0,     0,     0);
	gdk_color_set(g_vColors[Color_BoxBorder],                   0,     0,     0);
	gdk_color_set(g_vColors[Color_BoxInputBackground],      65535, 49151, 32767);
	gdk_color_set(g_vColors[Color_BoxInputBorder],          16383, 16383, 16383);
	gdk_color_set(g_vColors[Color_BoxOutputBackground],     32767, 65535, 49151);
	gdk_color_set(g_vColors[Color_BoxOutputBorder],         16383, 16383, 16383);
	gdk_color_set(g_vColors[Color_BoxSettingBackground],    49151, 32767, 65535);
	gdk_color_set(g_vColors[Color_BoxSettingBorder],        16383, 16383, 16383);
	gdk_color_set(g_vColors[Color_Link],                        0,     0,     0);
	gdk_color_set(g_vColors[Color_LinkSelected],            49151, 16383, 16383);
	gdk_color_set(g_vColors[Color_SelectionArea],          0x3f00,0x3f00,0x3f00);
	gdk_color_set(g_vColors[Color_SelectionAreaBorder],         0,     0,     0);
	#undef gdk_color_set

//___________________________________________________________________//
//                                                                   //

	IKernelLoader* l_pKernelLoader=OpenViBE::createKernelLoader();
	if(l_pKernelLoader)
	{
		cout<<"[  INF  ] Created kernel loader"<<endl;
		CString m_sError;
#if defined OVD_OS_Windows
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
					l_rLogManager.activate(LogLevel_Trace, false);
					l_rLogManager.activate(LogLevel_Info, true);
					l_rLogManager.activate(LogLevel_Warning, true);
					l_rLogManager.activate(LogLevel_ImportantWarning, true);
					l_rLogManager.activate(LogLevel_Error, true);
					l_rLogManager.activate(LogLevel_Fatal, true);

// For Mister Vincent !
#ifdef OVD_OS_Windows
#ifndef NDEBUG
					_asm int 3;
#endif
#endif

					IPluginManager& l_rPluginManager=l_pKernel->getContext()->getPluginManager();
#if defined OVD_OS_Linux
					l_rPluginManager.addPluginsFromFiles("../lib/libOpenViBE-plugins-*.so");
#elif defined OVD_OS_Windows
					l_rPluginManager.addPluginsFromFiles("../lib/OpenViBE-plugins-*.dll");
#else
#endif

					//initialise Gtk before 3D context
					gtk_init(&g_argc, &g_argv);
					// gtk_rc_parse("../share/openvibe-applications/designer/interface.gtkrc");

					IVisualisationManager& l_rVisualisationManager=l_pKernel->getContext()->getVisualisationManager();

					//retrieve Ogre plugin file (assumed to be in Ogre binaries directory)
					char* l_pOgreDir = getenv("OGRE_HOME");
					if(l_pOgreDir == NULL)
					{
						l_rLogManager << LogLevel_Error << "Couldn't retrieve OGRE_HOME environment variable! Ogre initialisation will fail...\n";
					}
					std::string l_sPluginsFile(l_pOgreDir);

#if defined OVD_OS_Windows
	#ifndef NDEBUG
					l_sPluginsFile += "/bin/debug/Plugins.cfg";
	#else
					l_sPluginsFile += "/bin/release/Plugins.cfg";
	#endif
#elif defined OVD_OS_Linux
					l_sPluginsFile += "/lib/OGRE/Plugins.cfg";
#endif
					boolean l_bLogOgreToScreen = true;
					CIdentifier l_oDesignerResourcesIdentifier;
					l_rVisualisationManager.initialize3DContext(l_sPluginsFile.c_str(), l_bLogOgreToScreen, ""); // "ogre.log"
					l_rVisualisationManager.createResourceGroup(l_oDesignerResourcesIdentifier, "DesignerResources");
					l_rVisualisationManager.addResourceLocation(l_oDesignerResourcesIdentifier, "../../dependencies/ogre/media/scooter", ResourceType_Directory, false);
					l_rVisualisationManager.addResourceLocation(l_oDesignerResourcesIdentifier, "../../dependencies/ogre/media/tempo", ResourceType_Directory, false);
					l_rVisualisationManager.initializeResourceGroup(l_oDesignerResourcesIdentifier);

					::CApplication app(l_pKernel);
					app.initialize();
					app.newScenarioCB();

					CPluginObjectDescCollector cb_collector1(*l_pKernel->getContext());
					CPluginObjectDescCollector cb_collector2(*l_pKernel->getContext());
					CPluginObjectDescLogger cb_logger(*l_pKernel->getContext());
					cb_logger.enumeratePluginObjectDesc();
					cb_collector1.enumeratePluginObjectDesc(OV_ClassId_Plugins_BoxAlgorithmDesc);
					cb_collector2.enumeratePluginObjectDesc(OV_ClassId_Plugins_AlgorithmDesc);
					insertPluginObjectDesc_to_GtkTreeStore(cb_collector1.getPluginObjectDescMap(), app.m_pBoxAlgorithmTreeModel);
					insertPluginObjectDesc_to_GtkTreeStore(cb_collector2.getPluginObjectDescMap(), app.m_pAlgorithmTreeModel);

					gtk_main();

					cout<<"[  INF  ] Application terminated, releasing allocated objects"<<endl;

					l_rLogManager.activate(LogLevel_Debug, true);
					l_rLogManager.activate(LogLevel_Benchmark, false);
					l_rLogManager.activate(LogLevel_Trace, true);
					l_rLogManager.activate(LogLevel_Info, true);
					l_rLogManager.activate(LogLevel_Warning, true);
					l_rLogManager.activate(LogLevel_ImportantWarning, true);
					l_rLogManager.activate(LogLevel_Error, true);
					l_rLogManager.activate(LogLevel_Fatal, true);

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

int main(int argc, char ** argv)
{
	int l_iRet=-1;
	try
	{
		l_iRet=go(argc, argv);
	}
	catch (...)
	{
		std::cout << "Caught an exception at the very top...\nLeaving application!\n";
	}
	return l_iRet;
}
