#include "ovpCBoxAlgorithmStimulationArtefactFilter.h"

#include <list>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Stimulation;

#define _LOG_(lm, x) { lm << x; }
#define _OPTIONAL_LOG_(lm, x) /* _LOG_(lm, x); */

namespace
{
	class _AutoCast_
	{
	public:
		_AutoCast_(IBox& rBox, IConfigurationManager& rConfigurationManager, const uint32 ui32Index) : m_rConfigurationManager(rConfigurationManager) { rBox.getSettingValue(ui32Index, m_sSettingValue); }
		operator uint64 (void) { return m_rConfigurationManager.expandAsUInteger(m_sSettingValue); }
		operator int64 (void) { return m_rConfigurationManager.expandAsInteger(m_sSettingValue); }
		operator float64 (void) { return m_rConfigurationManager.expandAsFloat(m_sSettingValue); }
		operator boolean (void) { return m_rConfigurationManager.expandAsBoolean(m_sSettingValue); }
		operator const CString (void) { return m_sSettingValue; }
	protected:
		IConfigurationManager& m_rConfigurationManager;
		CString m_sSettingValue;
	};
};

boolean CBoxAlgorithmStimArtefactFilter::initialize(void)
{
	m_pStimulationEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
	m_pStimulationEncoder->initialize();
    op_pEncodedMemoryBuffer.initialize(m_pStimulationEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	ip_pStimulationSet.initialize(m_pStimulationEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));

	m_pStimulationDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pStimulationDecoder->initialize();
	ip_pMemoryBufferToDecode.initialize(m_pStimulationDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pStimulationSet.initialize(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
	
	m_oStimulationSet=new CStimulationSet();
	m_oReserveStimulationSet=new CStimulationSet();
	
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------

	IBox& l_rStaticBoxContext=this->getStaticBoxContext();	
	m_ui64MinimumDuration	=((float64)_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0))*(1LL<<32);
	m_ui64StimulationStart	=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 1));
	m_ui64StimulationEnd	=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, _AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 2));
	
	m_ui64startTimeStim=0;
	m_bSkip=false;
	m_bNeedChunkMemory=false;
	m_bCutChunkTime=false;
	m_ui64MemStartTime=m_ui64MemEndTime=0;
	
	if(m_ui64StimulationStart==0 || m_ui64StimulationEnd==0) {return false;}
			
	return true;
}

boolean CBoxAlgorithmStimArtefactFilter::uninitialize(void)
{
	delete m_oReserveStimulationSet;
	delete m_oStimulationSet;
	
	op_pStimulationSet.uninitialize();
	ip_pMemoryBufferToDecode.uninitialize();
	m_pStimulationDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoder);

	ip_pStimulationSet.uninitialize();
	op_pEncodedMemoryBuffer.uninitialize();
	m_pStimulationEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationEncoder);

	return true;
}

boolean CBoxAlgorithmStimArtefactFilter::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmStimArtefactFilter::process(void)
{
	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		ip_pMemoryBufferToDecode=l_rDynamicBoxContext.getInputChunk(0, i);
		op_pEncodedMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);
		m_pStimulationDecoder->process();

		uint64 l_ui64StartTime=l_rDynamicBoxContext.getInputChunkStartTime(0, i);
		uint64 l_ui64EndTime=l_rDynamicBoxContext.getInputChunkEndTime(0, i);
		if(m_ui64MemEndTime!=0) {m_ui64MemEndTime=l_ui64EndTime;}
					
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		  {
			ip_pStimulationSet=op_pStimulationSet;
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
		  }
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		  {
			m_oStimulationSet->clear();
			//
			for(uint32 j=0; j<op_pStimulationSet->getStimulationCount(); j++)
			  {
				if(op_pStimulationSet->getStimulationIdentifier(j) == m_ui64StimulationStart)
				  {
					//std::cout<<"has new start at "<<op_pStimulationSet->getStimulationDate(j)<<std::endl;
					if(!m_bSkip)
					  {
						m_ui64startTimeStim=op_pStimulationSet->getStimulationDate(j);
						m_ui64MemStartTime=m_ui64startTimeStim;
						m_ui64MemEndTime=l_ui64EndTime;
						l_ui64EndTime=m_ui64startTimeStim;
						m_bCutChunkTime=true;
						m_bNeedChunkMemory=true;
					  }
					//
					if(m_ui64StimulationEnd==m_ui64StimulationStart) {m_bSkip=!m_bSkip;}
				  }	
				//  
				//std::cout<<m_ui64startTimeStim<<" && "<<op_pStimulationSet->getStimulationDate(j)<<"<="<<m_ui64startTimeStim+m_ui64MinimumDuration<<std::endl;
				if(m_ui64startTimeStim!=0 && op_pStimulationSet->getStimulationDate(j)<=m_ui64startTimeStim+m_ui64MinimumDuration)
				  {
					//std::cout<<"retain Stimulation"<<std::endl;
					m_oReserveStimulationSet->appendStimulation(op_pStimulationSet->getStimulationIdentifier(j),op_pStimulationSet->getStimulationDate(j),op_pStimulationSet->getStimulationDuration(j));
					//
					if(op_pStimulationSet->getStimulationIdentifier(j) == m_ui64StimulationEnd && 
						(m_ui64StimulationEnd!=m_ui64StimulationStart || op_pStimulationSet->getStimulationDate(j)!=m_ui64startTimeStim))
					  {
						std::cout<<"end in minimum windows...will shunt"<<std::endl;
						//flush de la reserve avec les start et stop à 0
						for(uint32 k=0; k<m_oReserveStimulationSet->getStimulationCount(); k++)
						  {
							if(m_oReserveStimulationSet->getStimulationIdentifier(k)!=m_ui64StimulationStart && m_oReserveStimulationSet->getStimulationIdentifier(k)!=m_ui64StimulationEnd)
							  {m_oStimulationSet->appendStimulation(m_oReserveStimulationSet->getStimulationIdentifier(k),m_oReserveStimulationSet->getStimulationDate(k),m_oReserveStimulationSet->getStimulationDuration(k));}
							else
							  {m_oStimulationSet->appendStimulation(0,m_oReserveStimulationSet->getStimulationDate(k),m_oReserveStimulationSet->getStimulationDuration(k));}
							m_oReserveStimulationSet->removeStimulation(k);
							k--;
						  }
						//
						m_ui64startTimeStim=0;
					  }
				  }
				else
				  {
					m_bNeedChunkMemory=false;
					for(uint32 k=0; k<m_oReserveStimulationSet->getStimulationCount(); k++)
					  {
						m_oStimulationSet->appendStimulation(m_oReserveStimulationSet->getStimulationIdentifier(k),m_oReserveStimulationSet->getStimulationDate(k),m_oReserveStimulationSet->getStimulationDuration(k));
						m_oReserveStimulationSet->removeStimulation(k);
						k--;
					  }
					m_oStimulationSet->appendStimulation(op_pStimulationSet->getStimulationIdentifier(j),op_pStimulationSet->getStimulationDate(j),op_pStimulationSet->getStimulationDuration(j));
				  }
			
				  
			  }

			if(!m_bCutChunkTime && m_ui64MemStartTime!=0)
			  {
				l_ui64StartTime=m_ui64MemStartTime;
				l_ui64EndTime=m_ui64MemEndTime;
				m_bCutChunkTime=false;
			  }
			ip_pStimulationSet=m_oStimulationSet;
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
			
			if(!m_bNeedChunkMemory)
			 {
				m_ui64MemStartTime=m_ui64MemEndTime=0;
			 }

		  }
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		  {
			ip_pStimulationSet=op_pStimulationSet;
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeEnd);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
		  }
		  
		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	return true;
}

