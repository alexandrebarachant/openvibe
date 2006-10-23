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

			CEntryEnumeratorCallBack(IKernel& rKernel, vector<IPluginModule*>& rPluginModule, map<IPluginModule*, vector<IPluginObjectDesc*> >& rPluginObjectDesc)
				:m_rKernel(rKernel)
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
						m_rKernel.getKernelLog() << "[FAILED] Module [" << rEntry.getName() << "] has already been loaded\n";
						return true;
					}
				}

				IObject* l_pObject=m_rKernel.getObjectFactory().createObject(OV_ClassId_Kernel_PluginModule);
				if(!l_pObject)
				{
					m_rKernel.getKernelLog() << "[FAILED] Loading [" << rEntry.getName() << "]\n";
					return true;
				}

				IPluginModule* l_pPluginModule=dynamic_cast<IPluginModule*>(l_pObject);
				if(!l_pPluginModule)
				{
					m_rKernel.getObjectFactory().releaseObject(l_pObject);
					m_rKernel.getKernelLog() << "[FAILED] Loading [" << rEntry.getName() << "]\n";
					return true;
				}

				if(!l_pPluginModule->load(rEntry.getName()))
				{
					m_rKernel.getObjectFactory().releaseObject(l_pObject);
					m_rKernel.getKernelLog() << "[FAILED] File [" << rEntry.getName() << "] is not a plugin module\n";
					return true;
				}

				if(!l_pPluginModule->initialize())
				{
					l_pPluginModule->uninitialize();
					l_pPluginModule->unload();
					m_rKernel.getObjectFactory().releaseObject(l_pObject);
					m_rKernel.getKernelLog() << "[FAILED] Module [" << rEntry.getName() << "] did not initialize correctly\n";
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
					m_rKernel.getKernelLog() << "[  OK  ] Added " << (uint32)m_rPluginObjectDesc[l_pPluginModule].size() << " 'plugin object descriptor(s)' from [" << rEntry.getName() << "]\n";
				}
				else
				{
					m_rKernel.getKernelLog() << "[  OK  ] No 'plugin object descriptor' found from [" << rEntry.getName() << "]\n";
				}

				return true;
			}

		protected:

			IKernel& m_rKernel;
			vector<IPluginModule*>& m_rPluginModule;
			map<IPluginModule*, vector<IPluginObjectDesc*> >& m_rPluginObjectDesc;
		};
	};
};

CPluginManager::CPluginManager(IKernel& rKernel)
	:TKernelObject<IPluginManager>(rKernel)
{
}

boolean CPluginManager::addPluginsFromFiles(
	const CString& rFileNameWildCard)
{
	getKernel().getKernelLog() << "[ INFO ] Adding [" << rFileNameWildCard << "]\n";

	boolean l_bResult;
	CEntryEnumeratorCallBack l_rCB(getKernel(), m_vPluginModule, m_vPluginObjectDesc);
	FS::IEntryEnumerator* l_pEntryEnumerator=FS::createEntryEnumerator(l_rCB);
	l_bResult=l_pEntryEnumerator->enumerate(rFileNameWildCard);
	l_pEntryEnumerator->release();
	return l_bResult;
}

boolean CPluginManager::enumeratePluginObjectDesc(
	IPluginManager::IPluginObjectDescEnum& rCallback) const
{
	getKernel().getKernelLog() << "[ INFO ] Enumerating 'plugin object descriptor(s)'\n";

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
	getKernel().getKernelLog() << "[ INFO ] Enumerating 'plugin object descriptor(s)' with base type " << rBaseClassIdentifier << "\n";

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

const IPluginObjectDesc* CPluginManager::getPluginObjectDescCreating(
	const CIdentifier& rClassIdentifier) const
{
	getKernel().getKernelLog() << "[ INFO ] Searching plugin object descriptor\n";

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

	getKernel().getKernelLog() << "[FAILED] Class identifier not found\n";
	return NULL;

}

IPluginObject* CPluginManager::createPluginObject(
	const CIdentifier& rClassIdentifier)
{
	getKernel().getKernelLog() << "[ INFO ] Creating plugin object\n";

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

	getKernel().getKernelLog() << "[FAILED] Class identifier not found\n";
	return NULL;
}

boolean CPluginManager::releasePluginObject(
	IPluginObject* pPluginObject)
{
	getKernel().getKernelLog() << "[ INFO ] Releasing plugin object\n";

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

	getKernel().getKernelLog() << "[FAILED] Plugin object not found\n";
	return false;
}
