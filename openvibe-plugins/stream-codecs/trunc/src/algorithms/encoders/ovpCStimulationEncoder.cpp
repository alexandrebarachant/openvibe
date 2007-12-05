#include "ovpCStimulationEncoder.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

boolean CStimulationEncoder::initialize(void)
{
	CEBMLBaseEncoder::initialize();

	m_oStimulationSetHandle.initialize(getInputParameter(OVP_StimulationEncoder_StimulationSet_InParameterId));

	return true;
}

boolean CStimulationEncoder::uninitialize(void)
{
	m_oStimulationSetHandle.uninitialize();

	CEBMLBaseEncoder::uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

boolean CStimulationEncoder::processBuffer(void)
{
	IStimulationSet* l_pStimulationSet=m_oStimulationSetHandle;

	 m_pEBMLWriterHelper->openChild(OVTK_NodeId_Buffer);
	  m_pEBMLWriterHelper->openChild(OVTK_NodeId_Buffer_Stimulation);
	   m_pEBMLWriterHelper->openChild(OVTK_NodeId_Buffer_Stimulation_NumberOfStimulations);
	    m_pEBMLWriterHelper->setUIntegerAsChildData(l_pStimulationSet->getStimulationCount());
	   m_pEBMLWriterHelper->closeChild();
	   for(uint32 i=0; i<l_pStimulationSet->getStimulationCount(); i++)
	   {
	     m_pEBMLWriterHelper->openChild(OVTK_NodeId_Buffer_Stimulation_Stimulation);
	      m_pEBMLWriterHelper->openChild(OVTK_NodeId_Buffer_Stimulation_Stimulation_Identifier);
	       m_pEBMLWriterHelper->setUIntegerAsChildData(l_pStimulationSet->getStimulationIdentifier(i));
	      m_pEBMLWriterHelper->closeChild();
	      m_pEBMLWriterHelper->openChild(OVTK_NodeId_Buffer_Stimulation_Stimulation_Date);
	       m_pEBMLWriterHelper->setUIntegerAsChildData(l_pStimulationSet->getStimulationDate(i));
	      m_pEBMLWriterHelper->closeChild();
	      m_pEBMLWriterHelper->openChild(OVTK_NodeId_Buffer_Stimulation_Stimulation_Duration);
	       m_pEBMLWriterHelper->setUIntegerAsChildData(l_pStimulationSet->getStimulationDuration(i));
	      m_pEBMLWriterHelper->closeChild();
	     m_pEBMLWriterHelper->closeChild();
	    }
	  m_pEBMLWriterHelper->closeChild();
	 m_pEBMLWriterHelper->closeChild();

	return true;
}
