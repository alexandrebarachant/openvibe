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
		const IPluginObjectDesc& rPluginObjectDesc)=0;

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
		const IPluginObjectDesc& rPluginObjectDesc)
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
		const IPluginObjectDesc& rPluginObjectDesc)
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

static void insertPluginObjectDesc_to_GtkTreeStore(const IKernelContext& rKernelContext, map<string, const IPluginObjectDesc*>& vPluginObjectDesc, ::GtkTreeStore* pTreeStore)
{
	map<string, const IPluginObjectDesc*>::iterator itPluginObjectDesc;
	for(itPluginObjectDesc=vPluginObjectDesc.begin(); itPluginObjectDesc!=vPluginObjectDesc.end(); itPluginObjectDesc++)
	{
		const IPluginObjectDesc* l_pPluginObjectDesc=itPluginObjectDesc->second;

		CString l_sStockItemName;

		const IBoxAlgorithmDesc* l_pBoxAlgorithmDesc=dynamic_cast<const IBoxAlgorithmDesc*>(l_pPluginObjectDesc);
		if(l_pBoxAlgorithmDesc)
		{
			l_sStockItemName=l_pBoxAlgorithmDesc->getStockItemName();
		}

		boolean l_bShouldShow=true;

		if  (rKernelContext.getPluginManager().isPluginObjectFlaggedAsDeprecated(l_pPluginObjectDesc->getCreatedClass())
		 && !rKernelContext.getConfigurationManager().expandAsBoolean("${Designer_ShowDeprecated}", false))
		{
			l_bShouldShow=false;
		}

		if  (rKernelContext.getPluginManager().isPluginObjectFlaggedAsUnstable(l_pPluginObjectDesc->getCreatedClass())
		 && !rKernelContext.getConfigurationManager().expandAsBoolean("${Designer_ShowUnstable}", false))
		{
			l_bShouldShow=false;
		}

		if(l_bShouldShow)
		{
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
						Resource_StringColor, "#000000",
						Resource_BooleanIsPlugin, (gboolean)FALSE,
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
				Resource_StringColor,
					rKernelContext.getPluginManager().isPluginObjectFlaggedAsDeprecated(l_pPluginObjectDesc->getCreatedClass())?"#3f7f7f":
					rKernelContext.getPluginManager().isPluginObjectFlaggedAsUnstable(l_pPluginObjectDesc->getCreatedClass())?"#6f6f6f":
					"black",
				Resource_BooleanIsPlugin, (gboolean)TRUE,
				-1);
		};
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

int go(int argc, char ** argv)
{
	/*
		{ 0,     0,     0,     0 },
		{ 0, 16383, 16383, 16383 },
		{ 0, 32767, 32767, 32767 },
		{ 0, 49151, 49151, 49151 },
		{ 0, 65535, 65535, 65535 },
	*/

	#define gdk_color_set(c, r, g, b) { c.pixel=0; c.red=r; c.green=g; c.blue=b; }
	gdk_color_set(g_vColors[Color_BackgroundPlayerStarted],  32767, 32767, 32767);
	gdk_color_set(g_vColors[Color_BoxBackgroundSelected],    65535, 65535, 49151);
	gdk_color_set(g_vColors[Color_BoxBackgroundMissing],     49151, 32767, 32767);
	gdk_color_set(g_vColors[Color_BoxBackgroundDeprecated],  24575, 32767, 32767);
	gdk_color_set(g_vColors[Color_BoxBackgroundNeedsUpdate], 57343, 57343, 57343);
	gdk_color_set(g_vColors[Color_BoxBackgroundUnstable],    49151, 49151, 49151);
	gdk_color_set(g_vColors[Color_BoxBackground],            65535, 65535, 65535);
	gdk_color_set(g_vColors[Color_BoxBorderSelected],            0,     0,     0);
	gdk_color_set(g_vColors[Color_BoxBorder],                    0,     0,     0);
	gdk_color_set(g_vColors[Color_BoxInputBackground],       65535, 49151, 32767);
	gdk_color_set(g_vColors[Color_BoxInputBorder],           16383, 16383, 16383);
	gdk_color_set(g_vColors[Color_BoxOutputBackground],      32767, 65535, 49151);
	gdk_color_set(g_vColors[Color_BoxOutputBorder],          16383, 16383, 16383);
	gdk_color_set(g_vColors[Color_BoxSettingBackground],     49151, 32767, 65535);
	gdk_color_set(g_vColors[Color_BoxSettingBorder],         16383, 16383, 16383);
	gdk_color_set(g_vColors[Color_Link],                         0,     0,     0);
	gdk_color_set(g_vColors[Color_LinkSelected],             49151, 49151, 16383);
	gdk_color_set(g_vColors[Color_LinkUpCast],               32767, 16383, 16383);
	gdk_color_set(g_vColors[Color_LinkDownCast],             16383, 32767, 16383);
	gdk_color_set(g_vColors[Color_LinkInvalid],              49151, 16383, 16383);
	gdk_color_set(g_vColors[Color_SelectionArea],           0x3f00,0x3f00,0x3f00);
	gdk_color_set(g_vColors[Color_SelectionAreaBorder],          0,     0,     0);
	#undef gdk_color_set

//___________________________________________________________________//
//                                                                   //

	CKernelLoader l_oKernelLoader;

	cout<<"[  INF  ] Created kernel loader, trying to load kernel module"<<endl;
	CString m_sError;
#if defined OVD_OS_Windows
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
			l_pKernelContext=l_pKernelDesc->createKernel("designer", "../share/openvibe.conf");
			if(!l_pKernelContext)
			{
				cout<<"[ FAILED ] No kernel created by kernel descriptor"<<endl;
			}
			else
			{
				OpenViBEToolkit::initialize(*l_pKernelContext);

// For Mister Vincent !
#ifdef OVD_OS_Windows
#ifndef NDEBUG
					//_asm int 3;
#endif
#endif

				IConfigurationManager& l_rConfigurationManager=l_pKernelContext->getConfigurationManager();

				l_pKernelContext->getPluginManager().addPluginsFromFiles(l_rConfigurationManager.expand("${Kernel_Plugins}"));

				//FIXME : set locale only when needed
				CString l_sLocale = l_rConfigurationManager.expand("${Designer_Locale}");
				if(l_sLocale == CString(""))
				{
					l_sLocale = "C";
				}
				setlocale( LC_ALL, l_sLocale.toASCIIString() );

				//initialise Gtk before 3D context
				gtk_init(&argc, &argv);
				// gtk_rc_parse("../share/openvibe-applications/designer/interface.gtkrc");

				if(l_rConfigurationManager.expandAsBoolean("${Kernel_3DVisualisationEnabled}"))
				{
					l_pKernelContext->getVisualisationManager().initialize3DContext();
				}

				{
					::CApplication app(*l_pKernelContext);
					app.initialize();
					app.newScenarioCB();

					CPluginObjectDescCollector cb_collector1(*l_pKernelContext);
					CPluginObjectDescCollector cb_collector2(*l_pKernelContext);
					CPluginObjectDescLogger cb_logger(*l_pKernelContext);
					cb_logger.enumeratePluginObjectDesc();
					cb_collector1.enumeratePluginObjectDesc(OV_ClassId_Plugins_BoxAlgorithmDesc);
					cb_collector2.enumeratePluginObjectDesc(OV_ClassId_Plugins_AlgorithmDesc);
					insertPluginObjectDesc_to_GtkTreeStore(*l_pKernelContext, cb_collector1.getPluginObjectDescMap(), app.m_pBoxAlgorithmTreeModel);
					insertPluginObjectDesc_to_GtkTreeStore(*l_pKernelContext, cb_collector2.getPluginObjectDescMap(), app.m_pAlgorithmTreeModel);

					l_pKernelContext->getLogManager() << LogLevel_Info << "Initialization took " << l_pKernelContext->getConfigurationManager().expand("$Core{real-time}") << " ms\n";

					try
					{
						gtk_main();
					}
					catch(...)
					{
						l_pKernelContext->getLogManager() << LogLevel_Fatal << "Catched top level exception\n";
					}
				}

				cout<<"[  INF  ] Application terminated, releasing allocated objects"<<endl;

				OpenViBEToolkit::uninitialize(*l_pKernelContext);

				l_pKernelDesc->releaseKernel(l_pKernelContext);
			}
		}
		l_oKernelLoader.uninitialize();
		l_oKernelLoader.unload();
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
