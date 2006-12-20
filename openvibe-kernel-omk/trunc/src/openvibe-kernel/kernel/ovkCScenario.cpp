#include "ovkCScenario.h"
#include "ovkCBoxProto.h"
#include "ovkCScenarioExporterContext.h"
#include "ovkCScenarioImporterContext.h"

#include <iostream>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

//___________________________________________________________________//
//                                                                   //

CScenario::CScenario(const IKernelContext& rKernelContext)
	:TKernelObject<TAttributable<IScenario> >(rKernelContext)
{
}

//___________________________________________________________________//
//                                                                   //

#if 0
namespace OpenViBE
{
	namespace Kernel
	{
		class CScenarioExporterPluginObjectDescEnumCB : virtual public IPluginManager::IPluginObjectDescEnum
		{
		public:

			virtual ~CScenarioExporterDescEnumCB(const CString& rFileExtension)
				:m_rFileExtension(rFileExtension)
				,m_oExporterClassIdentifier(OV_UnknownIdentifier)
			{
			}

			virtual boolean callback(
				const IPluginModule& rPluginModule,
				const IPluginObjectDesc& rPluginObjectDesc)
			{
				const IScenarioExporterDesc& l_rScenarioExporterDesc=dynamic_cast<const IScenarioExporterDesc&>(rPluginObjectDesc);
				if(l_rScenarioExporterDesc.getFileExtension()==m_rFileExtension)
				{
					m_oExporterClassIdentifier=l_rScenarioExporterDesc.getCreatedClassIdentifier();
					return false;
				}
				return true;
			}

			CIdentifier m_oExporterClassIdentifier;

		portected:

			CString& m_rFileExtension;
		};
	};
};
#endif

boolean CScenario::load(
	const CString& sFileName)
{
	getKernelContext().getLogManager() << "[ INFO ] Loading scneario\n";

	return false; // TODO
}

boolean CScenario::load(
	const CString& sFileName,
	const CIdentifier& rLoaderIdentifier)
{
	getKernelContext().getLogManager() << "[ INFO ] Loading scneario with specific importer\n";

	IScenarioImporter* l_pScenarioImporter=dynamic_cast<IScenarioImporter*>(getKernelContext().getPluginManager().createPluginObject(rLoaderIdentifier));
	if(!l_pScenarioImporter)
	{
		getKernelContext().getLogManager() << "[FAILED] Importer not found\n";
		return false;
	}
	l_pScenarioImporter->doImport(CScenarioImporterContext(getKernelContext(), sFileName, *this));
	getKernelContext().getPluginManager().releasePluginObject(l_pScenarioImporter);
	return true;
}

boolean CScenario::save(
	const CString& sFileName)
{
	getKernelContext().getLogManager() << "[ INFO ] Saving scneario\n";

	return false; // TODO
}

boolean CScenario::save(
	const CString& sFileName,
	const CIdentifier& rSaverIdentifier)
{
	getKernelContext().getLogManager() << "[ INFO ] Saving scneario with specific exporter\n";

	IScenarioExporter* l_pScenarioExporter=dynamic_cast<IScenarioExporter*>(getKernelContext().getPluginManager().createPluginObject(rSaverIdentifier));
	if(!l_pScenarioExporter)
	{
		getKernelContext().getLogManager() << "[FAILED] Exporter not found\n";
		return false;
	}
	l_pScenarioExporter->doExport(CScenarioExporterContext(getKernelContext(), sFileName, *this));
	getKernelContext().getPluginManager().releasePluginObject(l_pScenarioExporter);
	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CScenario::enumerateBoxes(
	IScenario::IBoxEnum& rCallback) const
{
	// getKernelContext().getLogManager() << "[ INFO ] Enumerating scenario boxes\n";

	map<CIdentifier, IBox*>::const_iterator itBox=m_vBox.begin();
	while(itBox!=m_vBox.end())
	{
		if(!rCallback.callback(*this, *(itBox->second)))
		{
			return true;
		}
		itBox++;
	}
	return true;
}

const IBox* CScenario::getBoxDetails(
	const CIdentifier& rBoxIdentifier) const
{
	getKernelContext().getLogManager() << "[ INFO ] Getting const box details from scenario\n";

	map<CIdentifier, IBox*>::const_iterator itBox;
	itBox=m_vBox.find(rBoxIdentifier);
	if(itBox==m_vBox.end())
	{
		getKernelContext().getLogManager() << "[FAILED] The box does not exist\n";
		return NULL;
	}
	return itBox->second;
}

boolean CScenario::isBox(
	const CIdentifier& rIdentifier) const
{
	map<CIdentifier, IBox*>::const_iterator itBox;
	itBox=m_vBox.find(rIdentifier);
	return itBox==m_vBox.end();
}

IBox* CScenario::getBoxDetails(
	const CIdentifier& rBoxIdentifier)
{
	getKernelContext().getLogManager() << "[ INFO ] Getting box details from scenario\n";

	map<CIdentifier, IBox*>::const_iterator itBox;
	itBox=m_vBox.find(rBoxIdentifier);
	if(itBox==m_vBox.end())
	{
		getKernelContext().getLogManager() << "[FAILED] The box does not exist\n";
		return NULL;
	}
	return itBox->second;
}

boolean CScenario::addBox(
	CIdentifier& rBoxIdentifier)
{
	getKernelContext().getLogManager() << "[ INFO ] Adding new empty box in scenario\n";

	IBox* l_pBox=OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).createObject<IBox*>(OV_ClassId_Kernel_Box);
	rBoxIdentifier=getUnusedIdentifier();
	l_pBox->setIdentifier(rBoxIdentifier);

	m_vBox[rBoxIdentifier]=l_pBox;
	return true;
}

boolean CScenario::addBox(
	const CIdentifier& rBoxAlgorithmIdentifier,
	CIdentifier& rBoxIdentifier)
{
	getKernelContext().getLogManager() << "[ INFO ] Adding new box in scenario\n";

	const IBoxAlgorithmDesc* l_pBoxAlgorithmDesc=dynamic_cast<const IBoxAlgorithmDesc*>(getKernelContext().getPluginManager().getPluginObjectDescCreating(rBoxAlgorithmIdentifier));
	if(!l_pBoxAlgorithmDesc)
	{
		getKernelContext().getLogManager() << "[FAILED] Algorithm descriptor not found\n";
		return false;
	}

	IBox* l_pBox=OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).createObject<IBox*>(OV_ClassId_Kernel_Box);
	rBoxIdentifier=getUnusedIdentifier();
	l_pBox->setIdentifier(rBoxIdentifier);
	l_pBox->setAlgorithmClassIdentifier(rBoxAlgorithmIdentifier);
	l_pBox->setName(l_pBoxAlgorithmDesc->getName());

	CBoxProto l_oBoxProto(getKernelContext(), *l_pBox);
	l_pBoxAlgorithmDesc->getBoxPrototype(l_oBoxProto);

	m_vBox[rBoxIdentifier]=l_pBox;
	return true;
}

boolean CScenario::removeBox(
	const CIdentifier& rBoxIdentifier)
{
	getKernelContext().getLogManager() << "[ INFO ] Removing box from scenario\n";

	// Finds the box according to its identifier
	map<CIdentifier, IBox*>::iterator itBox;
	itBox=m_vBox.find(rBoxIdentifier);
	if(itBox==m_vBox.end())
	{
		// The box does not exist !
		getKernelContext().getLogManager() << "[FAILED] The box does not exist\n";
		return false;
	}

	// Found the box,
	// now find all the links that are used by this box
	map<CIdentifier, vector<ILink*> >::iterator itBoxLink;
	itBoxLink=m_vBoxLink.find(rBoxIdentifier);
	if(itBoxLink!=m_vBoxLink.end())
	{
		// Links found
		// Delete all the links
		vector<ILink*>::iterator itLink;
		for(itLink=itBoxLink->second.begin(); itLink!=itBoxLink->second.end(); itLink++)
		{
			// Deletes link reference
			map<CIdentifier, ILink*>::iterator itLinkRef;
			itLinkRef=m_vLink.find((*itLink)->getIdentifier());
			if(itLinkRef!=m_vLink.end())
			{
				m_vLink.erase(itLinkRef);
			}

			// Deletes link -> box relations
			map<CIdentifier, vector<IBox*> >::iterator itLinkBox;
			itLinkBox=m_vLinkBox.find((*itLink)->getIdentifier());
			if(itLinkBox!=m_vLinkBox.end())
			{
				m_vLinkBox.erase(itLinkBox);
			}

			// Deletes link itself
			OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).releaseObject(*itLink);
		}

		// Deletes box -> link relation
		m_vBoxLink.erase(itBoxLink);
	}

	// Deletes the box itself
	OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).releaseObject(itBox->second);
	m_vBox.erase(itBox);

	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CScenario::enumerateLinks(
	IScenario::ILinkEnum& rCallback) const
{
	// getKernelContext().getLogManager() << "[ INFO ] Enumerating scenario links\n";

	map<CIdentifier, ILink*>::const_iterator itLink=m_vLink.begin();
	while(itLink!=m_vLink.end())
	{
		if(!rCallback.callback(*this, *(itLink->second)))
		{
			return true;
		}
		itLink++;
	}
	return true;
}

boolean CScenario::enumerateLinksFromBox(
	IScenario::ILinkEnum& rCallback,
	const CIdentifier& rBoxIdentifier) const
{
	// getKernelContext().getLogManager() << "[ INFO ] Enumerating scenario links from specific box\n";

	map<CIdentifier, vector<ILink*> >::const_iterator itBoxLink;
	CIdentifier l_oBoxIdentifier;
	uint32 l_ui32Index;
	itBoxLink=m_vBoxLink.find(rBoxIdentifier);
	if(itBoxLink!=m_vBoxLink.end())
	{
		vector<ILink*>::const_iterator itLink=itBoxLink->second.begin();
		while(itLink!=itBoxLink->second.end())
		{
			if((*itLink)->getSource(l_oBoxIdentifier, l_ui32Index))
			{
				if(l_oBoxIdentifier==rBoxIdentifier)
				{
					if(!rCallback.callback(*this, *(*itLink)))
					{
						return true;
					}
				}
			}
			itLink++;
		}
	}
	return true;
}

boolean CScenario::enumerateLinksFromBoxOutput(
	IScenario::ILinkEnum& rCallback,
	const CIdentifier& rBoxIdentifier,
	const uint32 ui32OutputIndex) const
{
	// getKernelContext().getLogManager() << "[ INFO ] Enumerating scenario links from specific box output\n";

	map<CIdentifier, vector<ILink*> >::const_iterator itBoxLink;
	CIdentifier l_oBoxIdentifier;
	uint32 l_ui32Index;
	itBoxLink=m_vBoxLink.find(rBoxIdentifier);
	if(itBoxLink!=m_vBoxLink.end())
	{
		vector<ILink*>::const_iterator itLink=itBoxLink->second.begin();
		while(itLink!=itBoxLink->second.end())
		{
			if((*itLink)->getSource(l_oBoxIdentifier, l_ui32Index))
			{
				if(l_oBoxIdentifier==rBoxIdentifier && l_ui32Index==ui32OutputIndex)
				{
					if(!rCallback.callback(*this, *(*itLink)))
					{
						return true;
					}
				}
			}
			itLink++;
		}
	}
	return true;
}


boolean CScenario::enumerateLinksToBox(
	IScenario::ILinkEnum& rCallback,
	const CIdentifier& rBoxIdentifier) const
{
	// getKernelContext().getLogManager() << "[ INFO ] Enumerating scenario links to specific box\n";

	map<CIdentifier, vector<ILink*> >::const_iterator itBoxLink;
	CIdentifier l_oBoxIdentifier;
	uint32 l_ui32Index;
	itBoxLink=m_vBoxLink.find(rBoxIdentifier);
	if(itBoxLink!=m_vBoxLink.end())
	{
		vector<ILink*>::const_iterator itLink=itBoxLink->second.begin();
		while(itLink!=itBoxLink->second.end())
		{
			if((*itLink)->getTarget(l_oBoxIdentifier, l_ui32Index))
			{
				if(l_oBoxIdentifier==rBoxIdentifier)
				{
					if(!rCallback.callback(*this, *(*itLink)))
					{
						return true;
					}
				}
			}
			itLink++;
		}
	}
	return true;
}

boolean CScenario::isLink(
	const CIdentifier& rIdentifier) const
{
	map<CIdentifier, ILink*>::const_iterator itLink;
	itLink=m_vLink.find(rIdentifier);
	return itLink==m_vLink.end();
}

const ILink* CScenario::getLinkDetails(
	const CIdentifier& rLinkIdentifier) const
{
	getKernelContext().getLogManager() << "[ INFO ] Retrieving const link details from scenario\n";

	map<CIdentifier, ILink*>::const_iterator itLink;
	itLink=m_vLink.find(rLinkIdentifier);
	if(itLink==m_vLink.end())
	{
		getKernelContext().getLogManager() << "[FAILED] The link does not exist\n";
		return NULL;
	}
	return itLink->second;
}

ILink* CScenario::getLinkDetails(
	const CIdentifier& rLinkIdentifier)
{
	getKernelContext().getLogManager() << "[ INFO ] Retrieving link details from scenario\n";

	map<CIdentifier, ILink*>::const_iterator itLink;
	itLink=m_vLink.find(rLinkIdentifier);
	if(itLink==m_vLink.end())
	{
		getKernelContext().getLogManager() << "[FAILED] The link does not exist\n";
		return NULL;
	}
	return itLink->second;
}

boolean CScenario::connect(
	const CIdentifier& rSourceBoxIdentifier,
	const uint32 ui32SourceBoxOutputIndex,
	const CIdentifier& rTargetBoxIdentifier,
	const uint32 ui32TargetBoxInputIndex,
	CIdentifier& rLinkIdentifier)
{
	getKernelContext().getLogManager() << "[ INFO ] Connecting boxes\n";

	map<CIdentifier, IBox*>::const_iterator itBox1;
	map<CIdentifier, IBox*>::const_iterator itBox2;
	itBox1=m_vBox.find(rSourceBoxIdentifier);
	itBox2=m_vBox.find(rTargetBoxIdentifier);
	if(itBox1==m_vBox.end() || itBox2==m_vBox.end())
	{
		getKernelContext().getLogManager() << "[FAILED] At least one of the boxes does not exist\n";
		return false;
	}
	IBox* l_pSourceBox=itBox1->second;
	IBox* l_pTargetBox=itBox2->second;
	if(ui32SourceBoxOutputIndex >= l_pSourceBox->getOutputCount())
	{
		getKernelContext().getLogManager() << "[FAILED] Wrong output index\n";
		return false;
	}
	if(ui32TargetBoxInputIndex >= l_pTargetBox->getInputCount())
	{
		getKernelContext().getLogManager() << "[FAILED] Wrong input index\n";
		return false;
	}

	rLinkIdentifier=getUnusedIdentifier();

	ILink* l_pLink=OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).createObject<ILink*>(OV_ClassId_Kernel_Link);
	l_pLink->setIdentifier(rLinkIdentifier);
	l_pLink->setSource(rSourceBoxIdentifier, ui32SourceBoxOutputIndex);
	l_pLink->setTarget(rTargetBoxIdentifier, ui32TargetBoxInputIndex);

	rLinkIdentifier=l_pLink->getIdentifier();

	m_vLink[rLinkIdentifier]=l_pLink;
	m_vLinkBox[rLinkIdentifier].push_back(l_pSourceBox);
	m_vLinkBox[rLinkIdentifier].push_back(l_pTargetBox);
	m_vBoxLink[rSourceBoxIdentifier].push_back(l_pLink);
	m_vBoxLink[rTargetBoxIdentifier].push_back(l_pLink);

	return true;
}

boolean CScenario::disconnect(
	const CIdentifier& rSourceBoxIdentifier,
	const uint32 ui32SourceBoxOutputIndex,
	const CIdentifier& rTargetBoxIdentifier,
	const uint32 ui32TargetBoxInputIndex)
{
	getKernelContext().getLogManager() << "[ INFO ] Disconnecting boxes\n";

	return false; // TODO
}

boolean CScenario::disconnect(
	const CIdentifier& rLinkIdentifier)
{
	getKernelContext().getLogManager() << "[ INFO ] Disconnecting boxes\n";

	// Finds the link according to its identifier
	map<CIdentifier, ILink*>::iterator itLink;
	itLink=m_vLink.find(rLinkIdentifier);
	if(itLink==m_vLink.end())
	{
		// The link does not exist !
		getKernelContext().getLogManager() << "[FAILED] The link does not exist\n";
		return false;
	}

	// Found the link,
	// now find all the boxes that are used by this link
	map<CIdentifier, vector<IBox*> >::iterator itLinkBox;
	itLinkBox=m_vLinkBox.find(rLinkIdentifier);
	if(itLinkBox!=m_vLinkBox.end())
	{
		// Boxes found
		vector<IBox*>::iterator itBox;
		for(itBox=itLinkBox->second.begin(); itBox!=itLinkBox->second.end(); itBox++)
		{
			// Updates the box -> link relations
			map<CIdentifier, vector<ILink*> >::iterator itBoxLink;
			itBoxLink=m_vBoxLink.find((*itBox)->getIdentifier());
			if(itBoxLink!=m_vBoxLink.end())
			{
				vector<ILink*>::iterator it;
				for(it=itBoxLink->second.begin(); it!=itBoxLink->second.end(); )
				{
					if((*it)->getIdentifier()==rLinkIdentifier)
					{
						it=itBoxLink->second.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
		}

		// Deletes link -> box relation
		m_vLinkBox.erase(itLinkBox);
	}

	// Deletes the link itself
	m_vLink.erase(itLink);
	OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).releaseObject(itLink->second);

	return true;
}

//___________________________________________________________________//
//                                                                   //

CIdentifier CScenario::getUnusedIdentifier(void) const
{
	uint64 l_ui64Identifier=(((uint64)rand())<<32)+((uint64)rand());
	CIdentifier l_oResult;
	map<CIdentifier, IBox*>::const_iterator i;
	map<CIdentifier, ILink*>::const_iterator j;
	do
	{
		l_ui64Identifier++;
		l_oResult=CIdentifier(l_ui64Identifier);
		i=m_vBox.find(l_oResult);
		j=m_vLink.find(l_oResult);
	}
	while(i!=m_vBox.end() || j!= m_vLink.end() || l_oResult==OV_UndefinedIdentifier);
	return l_oResult;
}
