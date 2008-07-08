#include "ovkCOgreResourceGroup.h"

using namespace std;
using namespace Ogre;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;

COgreResourceGroup::COgreResourceGroup(const IKernelContext& rKernelContext, const std::string& rName) :
	m_rKernelContext(rKernelContext),
	m_sName(rName),
	m_ui32RefCount(0),
	m_bInitialized(false)
{
	try
	{
		ResourceGroupManager::getSingleton().createResourceGroup(m_sName);
	}
	catch (Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to create resource group : " << e.what() << "\n";
	}
}

COgreResourceGroup::~COgreResourceGroup()
{
	//ensure resource group actually gets destroyed
	m_ui32RefCount = 1;
	//free resources
	destroy();
}

const std::string& COgreResourceGroup::getName()
{
	return m_sName;
}

bool COgreResourceGroup::addResourceLocation(const std::string& rPath, const std::string& rType,	bool bRecursive)
{
	if(m_bInitialized == true)
	{
		return false;
	}

	//ensure no duplicates
	//TODO : keep track of resource paths via a hash map instead?
	//ResourceGroupManager::getSingleton().removeResourceLocation(path, resourceGroup);

	try
	{
		ResourceGroupManager::getSingleton().addResourceLocation(rPath, rType, m_sName, bRecursive);
		return true;
	}
	catch(Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to add resource location : " << e.what() << "\n";
	}

	return false;
}
/*
bool COgreResourceGroup::addResourceLocations(const std::string& resourcesFile)
{
	if(m_bInitialized == true)
	{
		return false;
	}

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
}*/

bool COgreResourceGroup::initialize()
{
	m_ui32RefCount++;

	//initialize once only
	if(m_bInitialized == false)
	{
		try
		{
			//must clear resource group and reinitialize it for new resources to be found
			//ResourceGroupManager::getSingleton().clearResourceGroup(name);
			ResourceGroupManager::getSingleton().initialiseResourceGroup(m_sName);
			ResourceGroupManager::getSingleton().loadResourceGroup(m_sName);
			m_bInitialized = true;
			return true;
		}
		catch(Ogre::Exception& e)
		{
			m_rKernelContext.getLogManager()
				<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
				<< "Failed to initialize/load resource group : " << e.what() << "\n";
		}
	}

	return false;
}

bool COgreResourceGroup::destroy()
{
	//ensure we don't destroy an already destroyed group
	if(m_ui32RefCount==0)
	{
		return false;
	}

	--m_ui32RefCount;

	if(m_ui32RefCount == 0)
	{
		try
		{
			ResourceGroupManager::getSingleton().destroyResourceGroup(m_sName);
			m_bInitialized = false;
			return true;
		}
		catch(Ogre::Exception& e)
		{
			m_rKernelContext.getLogManager()
				<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
				<< "Failed to destroy resource group : " << e.what() << "\n";
		}
	}

	return false;
}
