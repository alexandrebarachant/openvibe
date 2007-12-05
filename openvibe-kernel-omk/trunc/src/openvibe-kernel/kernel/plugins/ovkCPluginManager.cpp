#include "ovkCPluginManager.h"

#include <fs/IEntryEnumerator.h>
#include <fs/Files.h>

#include <iostream>
#include <map>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace std;

namespace OpenViBE
{
	namespace Kernel
	{
		class CEntryEnumeratorCallBack : virtual public FS::IEntryEnumeratorCallBack
		{
		public:

			CEntryEnumeratorCallBack(const IKernelContext& rKernelContext, vector<IPluginModule*>& rPluginModule, map<IPluginModule*, vector<IPluginObjectDesc*> >& rPluginObjectDesc)
				:m_rKernelContext(rKernelContext)
				,m_rKernelObjectFactory(rKernelContext.getKernelObjectFactory())
				,m_rPluginModule(rPluginModule)
				,m_rPluginObjectDesc(rPluginObjectDesc)
			{
			}

			virtual FS::boolean callback(
				FS::IEntryEnumerator::IEntry& rEntry,
				FS::IEntryEnumerator::IAttributes& rAttributes)
			{
				vector<IPluginModule*>::iterator i;
				for(i=m_rPluginModule.begin(); i!=m_rPluginModule.end(); i++)
				{
					CString l_sPluginModuleName;
					if(!(*i)->getFileName(l_sPluginModuleName))
					{
						return true;
					}

					if(FS::Files::equals(rEntry.getName(), (const char*)l_sPluginModuleName))
					{
						log() << LogLevel_Warning << "Module [" << CString(rEntry.getName()) << "] has already been loaded\n";
						return true;
					}
				}

				IObject* l_pObject=m_rKernelObjectFactory.createObject(OV_ClassId_Kernel_Plugins_PluginModule);
				if(!l_pObject)
				{
					log() << LogLevel_Warning << "Loading [" << CString(rEntry.getName()) << "]\n";
					return true;
				}

				IPluginModule* l_pPluginModule=dynamic_cast<IPluginModule*>(l_pObject);
				if(!l_pPluginModule)
				{
					m_rKernelObjectFactory.releaseObject(l_pObject);
					log() << LogLevel_Warning << "Loading [" << CString(rEntry.getName()) << "]\n";
					return true;
				}

				CString l_sLoadError;
				if(!l_pPluginModule->load(rEntry.getName(), &l_sLoadError))
				{
					m_rKernelObjectFactory.releaseObject(l_pObject);
					log() << LogLevel_Warning << "File [" << CString(rEntry.getName()) << "] is not a plugin module (error:" << l_sLoadError << ")\n";
					return true;
				}

				if(!l_pPluginModule->initialize())
				{
					l_pPluginModule->uninitialize();
					l_pPluginModule->unload();
					m_rKernelObjectFactory.releaseObject(l_pObject);
					log() << LogLevel_Warning << "Module [" << CString(rEntry.getName()) << "] did not initialize correctly\n";
					return true;
				}

				boolean l_bPluginObjectDescAdded=false;
				uint32 l_ui32Index=0;
				IPluginObjectDesc* l_pPluginObjectDesc=NULL;
				while(l_pPluginModule->getPluginObjectDescription(l_ui32Index, l_pPluginObjectDesc))
				{
					if(!l_bPluginObjectDescAdded)
					{
						m_rPluginModule.push_back(l_pPluginModule);
						l_bPluginObjectDescAdded=true;
					}
					m_rPluginObjectDesc[l_pPluginModule].push_back(l_pPluginObjectDesc);
					l_ui32Index++;
					l_pPluginObjectDesc=NULL;
				}

				if(l_bPluginObjectDescAdded)
				{
					log() << LogLevel_Info << "Added " << (uint32)m_rPluginObjectDesc[l_pPluginModule].size() << " plugin object descriptor(s) from [" << CString(rEntry.getName()) << "]\n";
				}
				else
				{
					log() << LogLevel_Warning << "No 'plugin object descriptor' found from [" << CString(rEntry.getName()) << "] even if it looked like a plugin module\n";
				}

				return true;
			}

		protected:

			ILogManager& log(void)
			{
				return m_rKernelContext.getLogManager();
			}

		protected:

			const IKernelContext& m_rKernelContext;
			IKernelObjectFactory& m_rKernelObjectFactory;
			vector<IPluginModule*>& m_rPluginModule;
			map<IPluginModule*, vector<IPluginObjectDesc*> >& m_rPluginObjectDesc;
		};
	};
};

CPluginManager::CPluginManager(const IKernelContext& rKernelContext)
	:TKernelObject<IPluginManager>(rKernelContext)
{
}

boolean CPluginManager::addPluginsFromFiles(
	const CString& rFileNameWildCard)
{
	log() << LogLevel_Info << "Adding [" << rFileNameWildCard << "]\n";

	boolean l_bResult;
	CEntryEnumeratorCallBack l_rCB(getKernelContext(), m_vPluginModule, m_vPluginObjectDesc);
	FS::IEntryEnumerator* l_pEntryEnumerator=FS::createEntryEnumerator(l_rCB);
	l_bResult=l_pEntryEnumerator->enumerate(rFileNameWildCard);
	l_pEntryEnumerator->release();
	return l_bResult;
}

boolean CPluginManager::enumeratePluginObjectDesc(
	IPluginManager::IPluginObjectDescEnum& rCallback) const
{
	log() << LogLevel_Debug << "Enumerating 'plugin object descriptor(s)'\n";

	map<IPluginModule*, vector<IPluginObjectDesc*> >::const_iterator i;
	vector<IPluginObjectDesc*>::const_iterator j;
	for(i=m_vPluginObjectDesc.begin(); i!=m_vPluginObjectDesc.end(); i++)
	{
		for(j=i->second.begin(); j!=i->second.end(); j++)
		{
			if(!rCallback.callback(*(i->first), *(*j)))
			{
				return true;
			}
		}
	}
	return true;
}

boolean CPluginManager::enumeratePluginObjectDesc(
	IPluginManager::IPluginObjectDescEnum& rCallback,
	const OpenViBE::CIdentifier& rBaseClassIdentifier) const
{
	log() << LogLevel_Debug << "Enumerating 'plugin object descriptor(s)' with base type " << rBaseClassIdentifier << "\n";

	map<IPluginModule*, vector<IPluginObjectDesc*> >::const_iterator i;
	vector<IPluginObjectDesc*>::const_iterator j;
	for(i=m_vPluginObjectDesc.begin(); i!=m_vPluginObjectDesc.end(); i++)
	{
		for(j=i->second.begin(); j!=i->second.end(); j++)
		{
			if((*j)->isDerivedFromClass(rBaseClassIdentifier))
			{
				if(!rCallback.callback(*(i->first), *(*j)))
				{
					return true;
				}
			}
		}
	}
	return true;
}

boolean CPluginManager::canCreatePluginObject(
	const CIdentifier& rClassIdentifier)
{
	log() << LogLevel_Debug << "Searching if can build plugin object\n";

	map<IPluginModule*, vector<IPluginObjectDesc*> >::const_iterator i;
	vector<IPluginObjectDesc*>::const_iterator j;
	for(i=m_vPluginObjectDesc.begin(); i!=m_vPluginObjectDesc.end(); i++)
	{
		for(j=i->second.begin(); j!=i->second.end(); j++)
		{
			if((*j)->getCreatedClass()==rClassIdentifier)
			{
				return true;
			}
		}
	}

	return false;
}

const IPluginObjectDesc* CPluginManager::getPluginObjectDescCreating(
	const CIdentifier& rClassIdentifier) const
{
	log() << LogLevel_Debug << "Searching plugin object descriptor\n";

	map<IPluginModule*, vector<IPluginObjectDesc*> >::const_iterator i;
	vector<IPluginObjectDesc*>::const_iterator j;
	for(i=m_vPluginObjectDesc.begin(); i!=m_vPluginObjectDesc.end(); i++)
	{
		for(j=i->second.begin(); j!=i->second.end(); j++)
		{
			if((*j)->getCreatedClass()==rClassIdentifier)
			{
				return *j;
			}
		}
	}

	log() << LogLevel_Warning << "Class identifier " << rClassIdentifier << " not found\n";
	return NULL;

}

IPluginObject* CPluginManager::createPluginObject(
	const CIdentifier& rClassIdentifier)
{
	log() << LogLevel_Debug << "Creating plugin object\n";

	map<IPluginModule*, vector<IPluginObjectDesc*> >::iterator i;
	vector<IPluginObjectDesc*>::iterator j;
	for(i=m_vPluginObjectDesc.begin(); i!=m_vPluginObjectDesc.end(); i++)
	{
		for(j=i->second.begin(); j!=i->second.end(); j++)
		{
			if((*j)->getCreatedClass()==rClassIdentifier)
			{
				IPluginObject* l_pResult=(*j)->create();
				m_vPluginObject[i->first].push_back(l_pResult);
				return l_pResult;
			}
		}
	}

	log() << LogLevel_Warning << "Class identifier " << rClassIdentifier << " not found\n";
	return NULL;
}

boolean CPluginManager::releasePluginObject(
	IPluginObject* pPluginObject)
{
	log() << LogLevel_Debug << "Releasing plugin object\n";

	map<IPluginModule*, vector<IPluginObject*> >::iterator i;
	vector<IPluginObject*>::iterator j;
	for(i=m_vPluginObject.begin(); i!=m_vPluginObject.end(); i++)
	{
		for(j=i->second.begin(); j!=i->second.end(); j++)
		{
			if((*j)==pPluginObject)
			{
				m_vPluginObject[i->first].erase(j);
				pPluginObject->release();
				return true;
			}
		}
	}

	log() << LogLevel_Warning << "Plugin object not found\n";
	return false;
}


IAlgorithm* CPluginManager::createAlgorithm(
	const CIdentifier& rClassIdentifier,
	const IAlgorithmDesc** ppAlgorithmDesc)
{
	if(ppAlgorithmDesc)
	{
		*ppAlgorithmDesc=NULL;
	}

	IPluginObjectDesc* l_pPluginObjectDesc=NULL;
	map<IPluginModule*, vector<IPluginObjectDesc*> >::iterator i;
	vector<IPluginObjectDesc*>::iterator j;
	for(i=m_vPluginObjectDesc.begin(); i!=m_vPluginObjectDesc.end(); i++)
	{
		for(j=i->second.begin(); j!=i->second.end(); j++)
		{
			if((*j)->getCreatedClass()==rClassIdentifier)
			{
				l_pPluginObjectDesc=*j;
			}
		}
	}
	if(!l_pPluginObjectDesc)
	{
		return NULL;
	}

	IPluginObject* l_pPluginObject=l_pPluginObjectDesc->create();
	if(!l_pPluginObjectDesc)
	{
		return NULL;
	}

	IAlgorithmDesc* l_pAlgorithmDesc=dynamic_cast<IAlgorithmDesc*>(l_pPluginObjectDesc);
	IAlgorithm* l_pAlgorithm=dynamic_cast<IAlgorithm*>(l_pPluginObject);
	if(!l_pAlgorithmDesc || !l_pAlgorithm)
	{
		l_pPluginObject->release();
		return NULL;
	}

	if(ppAlgorithmDesc)
	{
		*ppAlgorithmDesc=l_pAlgorithmDesc;
	}
	return l_pAlgorithm;
}

IBoxAlgorithm* CPluginManager::createBoxAlgorithm(
	const CIdentifier& rClassIdentifier,
	const IBoxAlgorithmDesc** ppBoxAlgorithmDesc)
{
	if(ppBoxAlgorithmDesc)
	{
		*ppBoxAlgorithmDesc=NULL;
	}

	IPluginObjectDesc* l_pPluginObjectDesc=NULL;
	map<IPluginModule*, vector<IPluginObjectDesc*> >::iterator i;
	vector<IPluginObjectDesc*>::iterator j;
	for(i=m_vPluginObjectDesc.begin(); i!=m_vPluginObjectDesc.end(); i++)
	{
		for(j=i->second.begin(); j!=i->second.end(); j++)
		{
			if((*j)->getCreatedClass()==rClassIdentifier)
			{
				l_pPluginObjectDesc=*j;
			}
		}
	}
	if(!l_pPluginObjectDesc)
	{
		return NULL;
	}

	IPluginObject* l_pPluginObject=l_pPluginObjectDesc->create();
	if(!l_pPluginObjectDesc)
	{
		return NULL;
	}

	IBoxAlgorithmDesc* l_pBoxAlgorithmDesc=dynamic_cast<IBoxAlgorithmDesc*>(l_pPluginObjectDesc);
	IBoxAlgorithm* l_pBoxAlgorithm=dynamic_cast<IBoxAlgorithm*>(l_pPluginObject);
	if(!l_pBoxAlgorithmDesc || !l_pBoxAlgorithm)
	{
		l_pPluginObject->release();
		return NULL;
	}

	if(ppBoxAlgorithmDesc)
	{
		*ppBoxAlgorithmDesc=l_pBoxAlgorithmDesc;
	}
	return l_pBoxAlgorithm;
}
