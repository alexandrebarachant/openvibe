#include "ovkCScenario.h"
#include "ovkCBoxProto.h"

#include <iostream>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

//___________________________________________________________________//
//                                                                   //

CScenario::CScenario(IKernel& rKernel)
	:TKernelObject<IScenario>(rKernel)
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
	getKernel().getKernelLog() << "[ INFO ] Loading scneario\n";

	return false; // TODO
}

boolean CScenario::load(
	const CString& sFileName,
	const CIdentifier& rLoaderIdentifier)
{
	getKernel().getKernelLog() << "[ INFO ] Loading scneario with specific importer\n";

	return false; // TODO
}

boolean CScenario::save(
	const CString& sFileName)
{
	getKernel().getKernelLog() << "[ INFO ] Saving scneario\n";

	return false; // TODO
}

boolean CScenario::save(
	const CString& sFileName,
	const CIdentifier& rSaverIdentifier)
{
	getKernel().getKernelLog() << "[ INFO ] Saving scneario with specific exporter\n";

	IScenarioExporter* l_pScenarioExporter=dynamic_cast<IScenarioExporter*>(getKernel().getPluginManager().createPluginObject(rSaverIdentifier));
	if(!l_pScenarioExporter)
	{
		getKernel().getKernelLog() << "[FAILED] Exporter not found\n";
		return false;
	}
	l_pScenarioExporter->doExport(sFileName, *this);
	getKernel().getPluginManager().releasePluginObject(l_pScenarioExporter);
	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CScenario::enumerateBoxes(
	IScenario::IBoxEnum& rCallback) const
{
	// getKernel().getKernelLog() << "[ INFO ] Enumerating scenario boxes\n";

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
	getKernel().getKernelLog() << "[ INFO ] Getting const box details from scenario\n";

	map<CIdentifier, IBox*>::const_iterator itBox;
	itBox=m_vBox.find(rBoxIdentifier);
	if(itBox==m_vBox.end())
	{
		getKernel().getKernelLog() << "[FAILED] The box does not exist\n";
		return NULL;
	}
	return itBox->second;
}

IBox* CScenario::getBoxDetails(
	const CIdentifier& rBoxIdentifier)
{
	getKernel().getKernelLog() << "[ INFO ] Getting box details from scenario\n";

	map<CIdentifier, IBox*>::const_iterator itBox;
	itBox=m_vBox.find(rBoxIdentifier);
	if(itBox==m_vBox.end())
	{
		getKernel().getKernelLog() << "[FAILED] The box does not exist\n";
		return NULL;
	}
	return itBox->second;
}

boolean CScenario::addBox(
	const CIdentifier& rBoxAlgorithmIdentifier,
	CIdentifier& rBoxIdentifier)
{
	getKernel().getKernelLog() << "[ INFO ] Adding new box in scenario\n";

	const IBoxAlgorithmDesc* l_pBoxAlgorithmDesc=dynamic_cast<const IBoxAlgorithmDesc*>(getKernel().getPluginManager().getPluginObjectDescCreating(rBoxAlgorithmIdentifier));
	if(!l_pBoxAlgorithmDesc)
	{
		getKernel().getKernelLog() << "[FAILED] Algorithm descriptor not found\n";
		return false;
	}

	IBox* l_pBox=OpenViBE::Tools::CObjectFactoryHelper(getKernel().getObjectFactory()).createObject<IBox*>(OV_ClassId_Kernel_Box);

	rBoxIdentifier=getUnusedIdentifier();
	l_pBox->setName(l_pBoxAlgorithmDesc->getName());
	l_pBox->setIdentifier(rBoxIdentifier);
	l_pBox->setAlgorithmClassIdentifier(rBoxAlgorithmIdentifier);

	CBoxProto l_oBoxProto(getKernel(), *l_pBox);
	l_pBoxAlgorithmDesc->getBoxPrototype(l_oBoxProto);

	m_vBox[rBoxIdentifier]=l_pBox;
	return true;
}

boolean CScenario::removeBox(
	const CIdentifier& rBoxIdentifier)
{
	getKernel().getKernelLog() << "[ INFO ] Removing box from scenario\n";

	// Finds the box according to its identifier
	map<CIdentifier, IBox*>::iterator itBox;
	itBox=m_vBox.find(rBoxIdentifier);
	if(itBox==m_vBox.end())
	{
		// The box does not exist !
		getKernel().getKernelLog() << "[FAILED] The box does not exist\n";
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
			// Deletes link -> box relations
			map<CIdentifier, vector<IBox*> >::iterator itLinkBox;
			itLinkBox=m_vLinkBox.find((*itLink)->getIdentifier());
			if(itLinkBox!=m_vLinkBox.end())
			{
				m_vLinkBox.erase(itLinkBox);
			}

			// Deletes link itself
			OpenViBE::Tools::CObjectFactoryHelper(getKernel().getObjectFactory()).releaseObject(*itLink);
		}

		// Deletes box -> link relation
		m_vBoxLink.erase(itBoxLink);
	}

	// Deletes the box itself
	OpenViBE::Tools::CObjectFactoryHelper(getKernel().getObjectFactory()).releaseObject(itBox->second);

	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CScenario::enumerateLinks(
	IScenario::ILinkEnum& rCallback) const
{
	// getKernel().getKernelLog() << "[ INFO ] Enumerating scenario links\n";

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
	// getKernel().getKernelLog() << "[ INFO ] Enumerating scenario links from specific box\n";

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
	// getKernel().getKernelLog() << "[ INFO ] Enumerating scenario links from specific box output\n";

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
	// getKernel().getKernelLog() << "[ INFO ] Enumerating scenario links to specific box\n";

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
		}
	}
	return true;
}

const ILink* CScenario::getLinkDetails(
	const CIdentifier& rLinkIdentifier) const
{
	getKernel().getKernelLog() << "[ INFO ] Retrieving const link details from scenario\n";

	map<CIdentifier, ILink*>::const_iterator itLink;
	itLink=m_vLink.find(rLinkIdentifier);
	if(itLink==m_vLink.end())
	{
		getKernel().getKernelLog() << "[FAILED] The link does not exist\n";
		return NULL;
	}
	return itLink->second;
}

ILink* CScenario::getLinkDetails(
	const CIdentifier& rLinkIdentifier)
{
	getKernel().getKernelLog() << "[ INFO ] Retrieving link details from scenario\n";

	map<CIdentifier, ILink*>::const_iterator itLink;
	itLink=m_vLink.find(rLinkIdentifier);
	if(itLink==m_vLink.end())
	{
		getKernel().getKernelLog() << "[FAILED] The link does not exist\n";
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
	getKernel().getKernelLog() << "[ INFO ] Connecting boxes\n";

	map<CIdentifier, IBox*>::const_iterator itBox1;
	map<CIdentifier, IBox*>::const_iterator itBox2;
	itBox1=m_vBox.find(rSourceBoxIdentifier);
	itBox2=m_vBox.find(rTargetBoxIdentifier);
	if(itBox1==m_vBox.end() || itBox2==m_vBox.end())
	{
		getKernel().getKernelLog() << "[FAILED] At least one of the boxes does not exist\n";
		return false;
	}
	IBox* l_pSourceBox=itBox1->second;
	IBox* l_pTargetBox=itBox2->second;
	if(ui32SourceBoxOutputIndex >= l_pSourceBox->getOutputCount())
	{
		getKernel().getKernelLog() << "[FAILED] Wrong output index\n";
		return false;
	}
	if(ui32TargetBoxInputIndex >= l_pTargetBox->getInputCount())
	{
		getKernel().getKernelLog() << "[FAILED] Wrong input index\n";
		return false;
	}

	rLinkIdentifier=getUnusedIdentifier();

	ILink* l_pLink=OpenViBE::Tools::CObjectFactoryHelper(getKernel().getObjectFactory()).createObject<ILink*>(OV_ClassId_Kernel_Link);
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
	getKernel().getKernelLog() << "[ INFO ] Disconnecting boxes\n";

	return false; // TODO
}

boolean CScenario::disconnect(
	const CIdentifier& rLinkIdentifier)
{
	getKernel().getKernelLog() << "[ INFO ] Disconnecting boxes\n";

	// Finds the link according to its identifier
	map<CIdentifier, ILink*>::iterator itLink;
	itLink=m_vLink.find(rLinkIdentifier);
	if(itLink==m_vLink.end())
	{
		// The link does not exist !
		getKernel().getKernelLog() << "[FAILED] The link does not exist\n";
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
	OpenViBE::Tools::CObjectFactoryHelper(getKernel().getObjectFactory()).releaseObject(itLink->second);

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
