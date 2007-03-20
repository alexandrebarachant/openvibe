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

boolean CScenario::load(
	const CString& sFileName)
{
	// TODO
	log() << LogLevel_Debug << "CScenario::load - Not implemented yet\n";

	return false;
}

boolean CScenario::load(
	const CString& sFileName,
	const CIdentifier& rLoaderIdentifier)
{
	log() << LogLevel_Info << "Loading scneario with specific importer\n";

	IScenarioImporter* l_pScenarioImporter=dynamic_cast<IScenarioImporter*>(getKernelContext().getPluginManager().createPluginObject(rLoaderIdentifier));
	if(!l_pScenarioImporter)
	{
		log() << LogLevel_Warning << "Importer not found\n";
		return false;
	}
	boolean l_bResult=l_pScenarioImporter->doImport(CScenarioImporterContext(getKernelContext(), sFileName, *this));
	getKernelContext().getPluginManager().releasePluginObject(l_pScenarioImporter);

	if(!l_bResult)
	{
		log() << LogLevel_Warning << "Import failed...\n";
		return false;
	}
	return true;
}

boolean CScenario::save(
	const CString& sFileName)
{
	// TODO
	log() << LogLevel_Debug << "CScenario::save - Not implemented yet\n";

	return false;
}

boolean CScenario::save(
	const CString& sFileName,
	const CIdentifier& rSaverIdentifier)
{
	log() << LogLevel_Info << "Saving scneario with specific exporter\n";

	IScenarioExporter* l_pScenarioExporter=dynamic_cast<IScenarioExporter*>(getKernelContext().getPluginManager().createPluginObject(rSaverIdentifier));
	if(!l_pScenarioExporter)
	{
		log() << LogLevel_Warning << "Exporter not found\n";
		return false;
	}
	boolean l_bResult=l_pScenarioExporter->doExport(CScenarioExporterContext(getKernelContext(), sFileName, *this));
	getKernelContext().getPluginManager().releasePluginObject(l_pScenarioExporter);

	if(!l_bResult)
	{
		log() << LogLevel_Warning << "Export failed...\n";
		return false;
	}
	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CScenario::enumerateBoxes(
	IScenario::IBoxEnum& rCallback) const
{
	// log() << LogLevel_Info << "Enumerating scenario boxes\n";

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
	log() << LogLevel_Debug << "Getting const box details from scenario\n";

	map<CIdentifier, IBox*>::const_iterator itBox;
	itBox=m_vBox.find(rBoxIdentifier);
	if(itBox==m_vBox.end())
	{
		log() << LogLevel_Warning << "The box does not exist\n";
		return NULL;
	}
	return itBox->second;
}

boolean CScenario::isBox(
	const CIdentifier& rIdentifier) const
{
	map<CIdentifier, IBox*>::const_iterator itBox;
	itBox=m_vBox.find(rIdentifier);
	return itBox!=m_vBox.end();
}

IBox* CScenario::getBoxDetails(
	const CIdentifier& rBoxIdentifier)
{
	log() << LogLevel_Debug << "Getting box details from scenario\n";

	map<CIdentifier, IBox*>::const_iterator itBox;
	itBox=m_vBox.find(rBoxIdentifier);
	if(itBox==m_vBox.end())
	{
		log() << LogLevel_Warning << "The box does not exist\n";
		return NULL;
	}
	return itBox->second;
}

boolean CScenario::addBox(
	CIdentifier& rBoxIdentifier)
{
	log() << LogLevel_Info << "Adding new empty box in scenario\n";

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
	log() << LogLevel_Info << "Adding new box in scenario\n";

	const IBoxAlgorithmDesc* l_pBoxAlgorithmDesc=dynamic_cast<const IBoxAlgorithmDesc*>(getKernelContext().getPluginManager().getPluginObjectDescCreating(rBoxAlgorithmIdentifier));
	if(!l_pBoxAlgorithmDesc)
	{
		log() << LogLevel_Warning << "Algorithm descriptor not found\n";
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
	log() << LogLevel_Info << "Removing box from scenario\n";

	// Finds the box according to its identifier
	map<CIdentifier, IBox*>::iterator itBox;
	itBox=m_vBox.find(rBoxIdentifier);
	if(itBox==m_vBox.end())
	{
		// The box does not exist !
		log() << LogLevel_Warning << "The box does not exist\n";
		return false;
	}

	// Found the box,
	// now find all the links that are used by this box
	map<CIdentifier, ILink*>::iterator itLink;
	for(itLink=m_vLink.begin(); itLink!=m_vLink.end(); )
	{
		if(itLink->second->getSourceBoxIdentifier()==rBoxIdentifier || itLink->second->getTargetBoxIdentifier()==rBoxIdentifier)
		{
			// Deletes this link
			OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).releaseObject(itLink->second);

			// Removes link from the link list
			m_vLink.erase(itLink);
		}
		itLink++;
	}

	// Deletes the box itself
	OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).releaseObject(itBox->second);

	// Removes box from the box list
	m_vBox.erase(itBox);

	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CScenario::enumerateLinks(
	IScenario::ILinkEnum& rCallback) const
{
	log() << LogLevel_Debug << "Enumerating scenario links\n";

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
	log() << LogLevel_Debug << "Enumerating scenario links from specific box\n";

	map<CIdentifier, ILink*>::const_iterator itLink;
	for(itLink=m_vLink.begin(); itLink!=m_vLink.end(); itLink++)
	{
		if(itLink->second->getSourceBoxIdentifier()==rBoxIdentifier)
		{
			if(!rCallback.callback(*this, *(itLink->second)))
			{
				return true;
			}
		}
	}

	return true;
}

boolean CScenario::enumerateLinksFromBoxOutput(
	IScenario::ILinkEnum& rCallback,
	const CIdentifier& rBoxIdentifier,
	const uint32 ui32OutputIndex) const
{
	log() << LogLevel_Debug << "Enumerating scenario links from specific box output\n";

	map<CIdentifier, ILink*>::const_iterator itLink;
	for(itLink=m_vLink.begin(); itLink!=m_vLink.end(); itLink++)
	{
		if(itLink->second->getSourceBoxIdentifier()==rBoxIdentifier)
		{
			if(itLink->second->getSourceBoxOutputIndex()==ui32OutputIndex)
			{
				if(!rCallback.callback(*this, *(itLink->second)))
				{
					return true;
				}
			}
		}
	}

	return true;
}

boolean CScenario::enumerateLinksToBox(
	IScenario::ILinkEnum& rCallback,
	const CIdentifier& rBoxIdentifier) const
{
	log() << LogLevel_Debug << "Enumerating scenario links to specific box\n";

	map<CIdentifier, ILink*>::const_iterator itLink;
	for(itLink=m_vLink.begin(); itLink!=m_vLink.end(); itLink++)
	{
		if(itLink->second->getTargetBoxIdentifier()==rBoxIdentifier)
		{
			if(!rCallback.callback(*this, *(itLink->second)))
			{
				return true;
			}
		}
	}

	return true;
}

boolean CScenario::isLink(
	const CIdentifier& rIdentifier) const
{
	map<CIdentifier, ILink*>::const_iterator itLink;
	itLink=m_vLink.find(rIdentifier);
	return itLink!=m_vLink.end();
}

const ILink* CScenario::getLinkDetails(
	const CIdentifier& rLinkIdentifier) const
{
	log() << LogLevel_Debug << "Retrieving const link details from scenario\n";

	map<CIdentifier, ILink*>::const_iterator itLink;
	itLink=m_vLink.find(rLinkIdentifier);
	if(itLink==m_vLink.end())
	{
		log() << LogLevel_Warning << "The link does not exist\n";
		return NULL;
	}
	return itLink->second;
}

ILink* CScenario::getLinkDetails(
	const CIdentifier& rLinkIdentifier)
{
	log() << LogLevel_Debug << "Retrieving link details from scenario\n";

	map<CIdentifier, ILink*>::const_iterator itLink;
	itLink=m_vLink.find(rLinkIdentifier);
	if(itLink==m_vLink.end())
	{
		log() << LogLevel_Warning << "The link does not exist\n";
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
	log() << LogLevel_Info << "Connecting boxes\n";

	map<CIdentifier, IBox*>::const_iterator itBox1;
	map<CIdentifier, IBox*>::const_iterator itBox2;
	itBox1=m_vBox.find(rSourceBoxIdentifier);
	itBox2=m_vBox.find(rTargetBoxIdentifier);
	if(itBox1==m_vBox.end() || itBox2==m_vBox.end())
	{
		log() << LogLevel_Warning << "At least one of the boxes does not exist\n";
		return false;
	}
	IBox* l_pSourceBox=itBox1->second;
	IBox* l_pTargetBox=itBox2->second;
	if(ui32SourceBoxOutputIndex >= l_pSourceBox->getOutputCount())
	{
		log() << LogLevel_Warning << "Wrong output index\n";
		return false;
	}
	if(ui32TargetBoxInputIndex >= l_pTargetBox->getInputCount())
	{
		log() << LogLevel_Warning << "Wrong input index\n";
		return false;
	}

	rLinkIdentifier=getUnusedIdentifier();

	ILink* l_pLink=OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).createObject<ILink*>(OV_ClassId_Kernel_Link);
	l_pLink->setIdentifier(rLinkIdentifier);
	l_pLink->setSource(rSourceBoxIdentifier, ui32SourceBoxOutputIndex);
	l_pLink->setTarget(rTargetBoxIdentifier, ui32TargetBoxInputIndex);

	m_vLink[l_pLink->getIdentifier()]=l_pLink;

	return true;
}

boolean CScenario::disconnect(
	const CIdentifier& rSourceBoxIdentifier,
	const uint32 ui32SourceBoxOutputIndex,
	const CIdentifier& rTargetBoxIdentifier,
	const uint32 ui32TargetBoxInputIndex)
{
	// TODO
	log() << LogLevel_Debug << "CScenario::disconnect (identifier based) - Not implemented yet\n";

	return false;
}

boolean CScenario::disconnect(
	const CIdentifier& rLinkIdentifier)
{
	log() << LogLevel_Info << "Disconnecting boxes\n";

	// Finds the link according to its identifier
	map<CIdentifier, ILink*>::iterator itLink;
	itLink=m_vLink.find(rLinkIdentifier);
	if(itLink==m_vLink.end())
	{
		// The link does not exist !
		log() << LogLevel_Warning << "The link does not exist\n";
		return false;
	}

	// Deletes the link itself
	OpenViBE::Tools::CObjectFactoryHelper(getKernelContext().getObjectFactory()).releaseObject(itLink->second);

	// Removes link from the link list
	m_vLink.erase(itLink);

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
