#include "ovpCBoxAlgorithmStimulationNumberFilter.h"

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

boolean CBoxAlgorithmNbStimFilter::initialize(void)
{
	m_pStimulationEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
	m_pStimulationEncoder->initialize();
    op_pEncodedMemoryBuffer.initialize(m_pStimulationEncoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	m_pStimulationDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pStimulationDecoder->initialize();
	ip_pMemoryBufferToDecode.initialize(m_pStimulationDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));

	iop_pStimulationSet.initialize(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
	m_pStimulationEncoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet)->setReferenceTarget(m_pStimulationDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------

	IBox& l_rStaticBoxContext=this->getStaticBoxContext();	
	m_i64KeepingCount		=_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 0);
	if(m_i64KeepingCount<=0) 
	  {
		_OPTIONAL_LOG_(this->getLogManager(), LogLevel_Warning << "Can't keep less than 1 Stimulation\n");
		return false;
	  }
	m_i64AvoidBetween		=_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 1);
	if(m_i64AvoidBetween<0) 
	  {
		_OPTIONAL_LOG_(this->getLogManager(), LogLevel_Warning << "Can't avoid less than 0 Stimulation\n");
		m_i64AvoidBetween=0;
	  }
	m_i64AvoidStart        =_AutoCast_(l_rStaticBoxContext, this->getConfigurationManager(), 2);
	if(m_i64AvoidStart<0) 
	  {
		_OPTIONAL_LOG_(this->getLogManager(), LogLevel_Warning << "Can't start before starting\n");
		m_i64AvoidStart=0;
	  }
	
	m_i64keepingCountCurrent=0;
	m_i64avoidBetweenCurrent=0;
	m_i64avoidStartCurrent=0;
			
	return true;
}

boolean CBoxAlgorithmNbStimFilter::uninitialize(void)
{
	iop_pStimulationSet.uninitialize();
	
	ip_pMemoryBufferToDecode.uninitialize();
	m_pStimulationDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationDecoder);

	op_pEncodedMemoryBuffer.uninitialize();
	m_pStimulationEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationEncoder);

	return true;
}

boolean CBoxAlgorithmNbStimFilter::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmNbStimFilter::process(void)
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
					
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		  {
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
		  }
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		  {
			for(uint32 j=0; j<iop_pStimulationSet->getStimulationCount(); j++)
			  {
				if(iop_pStimulationSet->getStimulationIdentifier(j) != 0)
				  {
					if(m_i64avoidStartCurrent<m_i64AvoidStart)
					  {
						m_i64avoidStartCurrent++; 
						iop_pStimulationSet->removeStimulation(j);
						j--;
						continue;
					  }
					if(m_i64keepingCountCurrent>=m_i64KeepingCount)
					  {
						if(m_i64avoidBetweenCurrent<m_i64AvoidBetween)
						  {
							m_i64avoidBetweenCurrent++; 
							iop_pStimulationSet->removeStimulation(j);
							j--;
							continue;
						  }
						else
						  {
							m_i64keepingCountCurrent=0; 
							m_i64avoidBetweenCurrent=0;
						  }
					  }
					if(m_i64keepingCountCurrent<m_i64KeepingCount)
					  {m_i64keepingCountCurrent++;}
					
				  }
			  }

			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);

		  }
		if(m_pStimulationDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		  {
			m_pStimulationEncoder->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeEnd);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_ui64StartTime, l_ui64EndTime);
		  }
		  
		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	return true;
}

