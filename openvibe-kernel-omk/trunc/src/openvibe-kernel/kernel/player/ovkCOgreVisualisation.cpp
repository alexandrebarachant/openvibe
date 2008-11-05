#include "ovkCMaterial.h"
#include "ovkCOgreWindow.h"
#include "ovkCOgreScene.h"
#include "ovkCOgreResourceGroup.h"
#include "ovkCOgreVisualisation.h"

using namespace std;
using namespace Ogre;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;

COgreVisualisation::COgreVisualisation(const OpenViBE::Kernel::IKernelContext& rKernelContext) :
	m_rKernelContext(rKernelContext),
	m_bOgreInitialised(false),
	m_bResourcesInitialised(false),
	m_pRoot(NULL),
	m_bOutputCapturedMessages(false),
	m_pLog(NULL)
{
}

COgreVisualisation::~COgreVisualisation()
{
	finish();
}

void COgreVisualisation::messageLogged(const String& message, LogMessageLevel lml, bool maskDebug, const String &logName)
{
	if(m_bOutputCapturedMessages == true)
	{
		switch(lml)
		{
		case Ogre::LML_TRIVIAL:
		case Ogre::LML_NORMAL:
			m_rKernelContext.getLogManager() << LogLevel_Trace << message.c_str() << "\n";
			break;

		case Ogre::LML_CRITICAL:
			m_rKernelContext.getLogManager() << LogLevel_Error << message.c_str() << "\n";
			break;
		}
	}
}

boolean COgreVisualisation::initializeOgre(const CString& rPluginsFile, boolean bLogToScreen, const CString& rLogFileName) throw (std::exception)
{
	CNameValuePairList l_oOgreParams;		
	l_oOgreParams.setValue("PluginsFile", rPluginsFile.toASCIIString());	
	l_oOgreParams.setValue("CaptureMessages", "1");	
	l_oOgreParams.setValue("OutputCapturedMessages", bLogToScreen ? "1" : "0");	
	l_oOgreParams.setValue("OutputFileName", rLogFileName);

	return initializeOgre(l_oOgreParams);	
}

boolean COgreVisualisation::initializeOgre(const CNameValuePairList& rNameValuePairList) throw (std::exception)
{
	try
	{
	//retrieve optional file name to which Ogre messages should be dumped
	CString l_sOutputFileName;
	rNameValuePairList.getValue("OutputFileName", l_sOutputFileName);

	//should Ogre messages be captured by OpenViBE?
	boolean l_bCaptureMessages;
	rNameValuePairList.getValue("CaptureMessages", l_bCaptureMessages);
		
	//if Ogre messages are to be captured, a custom log manager must be created
	if(l_bCaptureMessages == true)
	{
		//should Ogre messages be forwarded to OpenViBE's log manager?
		rNameValuePairList.getValue("OutputCapturedMessages", m_bOutputCapturedMessages);
			
		//configure custom log
		//--------------------
		LogManager* l_pLogManager = new LogManager();
		//this is Ogre's default log
		bool l_bIsDefaultLog = true;
		//don't send Ogre log to debugger
		bool l_bSendLogToDebuggerOutput = false;
		//suppress logging to file if no filename is provided
		bool l_bSuppressFileLog = string(l_sOutputFileName).empty();
		//create custom log
		m_pLog = l_pLogManager->createLog(l_sOutputFileName.toASCIIString(), l_bIsDefaultLog, l_bSendLogToDebuggerOutput, l_bSuppressFileLog);
		//get log messages to forward them to OV's log manager, if required
		m_pLog->addListener(this);
	}

	//initialize Ogre
	//---------------

	//configuration file is assumed to be copied to working dir
	std::string l_sConfigFile("../share/openvibe-kernel-omk/ogre.cfg");

	//retrieve plugins file
	CString l_sPluginsFile;
	rNameValuePairList.getValue("PluginsFile", l_sPluginsFile);

	//create Ogre Root
	m_pRoot = new Ogre::Root(l_sPluginsFile.toASCIIString(), l_sConfigFile, l_bCaptureMessages ? "" : l_sOutputFileName.toASCIIString());

	//resources file is assumed to be copied to working dir
	std::string l_sResourcesFile("../share/openvibe-kernel-omk/resources.cfg");

	//add resource locations from resources file (if any)
	addResourceLocations(l_sResourcesFile);

	//init Ogre from last configuration, or show config dialog if none was found
	if((m_pRoot->restoreConfig() == true) || (m_pRoot->showConfigDialog() == true))
	{
		//init Ogre w/o creating a default window
		m_pRoot->initialise(false);
		m_bOgreInitialised = true;
	}
	}
	catch(Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to initialize Ogre : " << e.what() << "\n";
	}

	return m_bOgreInitialised;
}

boolean COgreVisualisation::ogreInitialized()
{
	return m_bOgreInitialised;
}

boolean COgreVisualisation::initializeResources() throw (Ogre::Exception)
{
	//ensure ogre initialisation hasn't been performed already
	if(m_bResourcesInitialised == true)
	{
		return false;
	}

	//init resources
	try
	{
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}
	catch(Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to initialize Ogre resources : " << e.what() << "\n";
	}

	m_bResourcesInitialised = true;

	return true;
}

boolean COgreVisualisation::resourcesInitialized()
{
	return m_bResourcesInitialised;
}

boolean COgreVisualisation::finish()
{
	try
	{
		//destroy windows
		std::map<OpenViBE::CIdentifier, COgreWindow*>::iterator itWindows = m_mOgreWindows.begin();
		while(itWindows != m_mOgreWindows.end())
		{
			delete itWindows->second;
			m_mOgreWindows.erase(itWindows);
			itWindows = m_mOgreWindows.begin();
		}

		//destroy scenes
		std::map<OpenViBE::CIdentifier, COgreScene*>::iterator itScenes = m_mOgreScenes.begin();
		while(itScenes != m_mOgreScenes.end())
		{
			delete itScenes->second;
			m_mOgreScenes.erase(itScenes);
			itScenes = m_mOgreScenes.begin();
		}

		//destroy resources
		std::map<OpenViBE::CIdentifier, COgreResourceGroup*>::iterator itResources = m_mOgreResourceGroups.begin();
		while(itResources != m_mOgreResourceGroups.end())
		{
			delete itResources->second;
			m_mOgreResourceGroups.erase(itResources);
			itResources = m_mOgreResourceGroups.begin();
		}

		if(m_pRoot != NULL)
		{
			delete m_pRoot;
			m_pRoot = NULL;
		}
	}
	catch(Ogre::Exception)
	{
		//
		return false;
	}

	return true;
}

void COgreVisualisation::idle()
{
	//update all windows
	std::map<OpenViBE::CIdentifier, COgreWindow*>::iterator it;

	for(it = m_mOgreWindows.begin(); it != m_mOgreWindows.end(); it++)
	{
		it->second->update();
	}
}

RenderWindow* COgreVisualisation::createRenderWindow(const std::string& rName, const std::string& rExternalHandle, unsigned int uiWidth, unsigned int uiHeight)
{
	RenderWindow* l_pRenderWindow = NULL;

	//store external window handle in struct
	Ogre::NameValuePairList params;
	params["externalWindowHandle"] = rExternalHandle;

	//create render window from settings passed in argument
	try
	{
		l_pRenderWindow = Ogre::Root::getSingleton().createRenderWindow(
			rName,
			uiWidth,
			uiHeight,
			false, //don't use fullscreen with non top level windows
			&params);
	}
	catch(Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to create render window : " << e.what() << "\n";
		return NULL;
	}

	return l_pRenderWindow;
}

COgreResourceGroup* COgreVisualisation::createResourceGroup(const CIdentifier& rResourceGroupIdentifier, const std::string& rResourceGroupName)
{
	//if group already exists, don't duplicate it
	if(m_mOgreResourceGroups.find(rResourceGroupIdentifier) == m_mOgreResourceGroups.end())
	{
		m_mOgreResourceGroups[rResourceGroupIdentifier] = new COgreResourceGroup(m_rKernelContext, rResourceGroupName);
	}
	return m_mOgreResourceGroups[rResourceGroupIdentifier];
}

boolean COgreVisualisation::addResourceLocation(const CIdentifier& rResourceGroupIdentifier, const std::string& rPath, const std::string& rType, bool bRecursive)
{
	if(m_mOgreResourceGroups.find(rResourceGroupIdentifier) == m_mOgreResourceGroups.end())
	{
		return false;
	}

	return m_mOgreResourceGroups[rResourceGroupIdentifier]->addResourceLocation(rPath, rType, bRecursive);
}

boolean COgreVisualisation::initializeResourceGroup(const CIdentifier& rResourceGroupIdentifier)
{
	if(m_mOgreResourceGroups.find(rResourceGroupIdentifier) == m_mOgreResourceGroups.end())
	{
		return false;
	}
	return m_mOgreResourceGroups[rResourceGroupIdentifier]->initialize();
}

boolean COgreVisualisation::destroyResourceGroup(const CIdentifier& rResourceGroupIdentifier)
{
	if(m_mOgreResourceGroups.find(rResourceGroupIdentifier) == m_mOgreResourceGroups.end())
	{
		return false;
	}
	return m_mOgreResourceGroups[rResourceGroupIdentifier]->destroy();
}

COgreScene* COgreVisualisation::createScene(const CIdentifier& rSceneIdentifier)
{
	//ensure scene hasn't been created yet
	if(m_mOgreScenes.find(rSceneIdentifier) == m_mOgreScenes.end())
	{
		char l_sBuffer[1024];
		sprintf(l_sBuffer, "Scene_%u", m_mOgreScenes.size());
		m_mOgreScenes[rSceneIdentifier] = new COgreScene(m_rKernelContext, String(l_sBuffer), this);
	}
	return m_mOgreScenes[rSceneIdentifier];
}

COgreWindow* COgreVisualisation::createWindow(CIdentifier rWindowIdentifier, const string& rWindowName, CIdentifier rSceneIdentifier)
{
	//ensure scene has been created
	if(m_mOgreScenes.find(rSceneIdentifier) == m_mOgreScenes.end())
	{
		return NULL;
	}

	//ensure window hasn't been created yet
	if(m_mOgreWindows.find(rWindowIdentifier) != m_mOgreWindows.end())
	{
		return NULL;
	}

	//create Ogre window
	m_mOgreWindows[rWindowIdentifier] = new COgreWindow(m_rKernelContext, rWindowName, this, m_mOgreScenes[rSceneIdentifier]);

	return m_mOgreWindows[rWindowIdentifier];
}

Ogre::Root* COgreVisualisation::getOgreRoot()
{
	return m_pRoot;
}

COgreWindow* COgreVisualisation::getOgreWindow(CIdentifier rWindowIdentifier)
{
	if(m_mOgreWindows.find(rWindowIdentifier) == m_mOgreWindows.end())
	{
		return NULL;
	}
	return m_mOgreWindows[rWindowIdentifier];
}

COgreScene* COgreVisualisation::getOgreScene(CIdentifier rSceneIdentifier)
{
	if(m_mOgreScenes.find(rSceneIdentifier) == m_mOgreScenes.end() || m_mOgreScenes[rSceneIdentifier] == NULL)
	{
		return NULL;
	}
	return m_mOgreScenes[rSceneIdentifier];
}

boolean COgreVisualisation::deleteWindow(CIdentifier rWindowIdentifier)
{
	if(m_mOgreWindows.find(rWindowIdentifier) == m_mOgreWindows.end())
	{
		return false;
	}
	m_rKernelContext.getLogManager() << LogLevel_Trace << "Destroying 3D window " << m_mOgreWindows[rWindowIdentifier]->getName().c_str() << "\n";
	delete m_mOgreWindows[rWindowIdentifier];
	m_mOgreWindows.erase(rWindowIdentifier);

	return true;
}

boolean COgreVisualisation::deleteScene(CIdentifier rSceneIdentifier)
{
	if(m_mOgreScenes.find(rSceneIdentifier) == m_mOgreScenes.end())
	{
		return false;
	}
	m_rKernelContext.getLogManager() << LogLevel_Trace << "Destroying 3D scene " << m_mOgreScenes[rSceneIdentifier]->getName().c_str() << "\n";
	delete m_mOgreScenes[rSceneIdentifier];
	m_mOgreScenes.erase(rSceneIdentifier);
	return true;
}

void COgreVisualisation::addResourceLocations(const std::string& resourcesFile)
{
	//load resource paths from config file
	ConfigFile configFile;
	configFile.load(resourcesFile);

	String resourceGroupName;
	String resourceTypeName;
	String resourceName;

	//go through all sections
	ConfigFile::SectionIterator sectionIterator(configFile.getSectionIterator());
	while(sectionIterator.hasMoreElements() == true)
	{
		//each section corresponds to a resource group
		resourceGroupName = sectionIterator.peekNextKey();
		ConfigFile::SettingsMultiMap* settings(sectionIterator.getNext());

		//go through all resources
		for(ConfigFile::SettingsMultiMap::iterator i(settings->begin()); i != settings->end(); ++i)
		{
			//each entry is a resource location for the current resource group
			resourceTypeName = i->first;
			resourceName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(resourceName, resourceTypeName, resourceGroupName);
		}
	}
}
