#include "ovpCBoxAlgorithmStimulationBasedEpoching.h"
#include <stdio.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

using namespace std;

boolean CBoxAlgorithmStimulationBasedEpoching::initialize(void)
{
	m_pStimulationStreamDecoder=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pSignalStreamDecoder=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
	m_pSignalStreamEncoder=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));

	m_pStimulationStreamDecoder->initialize();
	m_pSignalStreamDecoder->initialize();
	m_pSignalStreamEncoder->initialize();

	CString l_sSettingValue;

	float64 l_f64EpochDuration;
	getStaticBoxContext().getSettingValue(0, l_sSettingValue);
	if(sscanf(l_sSettingValue.toASCIIString(), "%lf", &l_f64EpochDuration)==0)
	{
		getLogManager() << LogLevel_Error << "Epoch duration could not be parsed as float64\n";
		return false;
	}
	getLogManager() << LogLevel_Debug << "Epoch duration : " << l_f64EpochDuration << "\n";
	m_ui64EpochDuration=(int64)(l_f64EpochDuration*(1LL<<32)); // $$$ Casted in (int64) because of Ubuntu 7.10 crash !

	float64 l_f64EpochOffset;
	getStaticBoxContext().getSettingValue(1, l_sSettingValue);
	if(sscanf(l_sSettingValue.toASCIIString(), "%lf", &l_f64EpochOffset)==0)
	{
		getLogManager() << LogLevel_Error << "Epoch offset could not be parsed as float64\n";
		return false;
	}
	getLogManager() << LogLevel_Debug << "Epoch offset : " << l_f64EpochOffset << "\n";
	m_i64EpochOffset=(int64)(l_f64EpochOffset*(1LL<<32));

	for(uint32 i=2; i<getStaticBoxContext().getSettingCount(); i++)
	{
		getStaticBoxContext().getSettingValue(i, l_sSettingValue);
		uint64 l_ui64StimulationId=getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, l_sSettingValue);
		getLogManager() << LogLevel_Debug << "Stimulation Id : " << l_ui64StimulationId << " with name " << l_sSettingValue << "\n";
		m_vStimulationId[l_ui64StimulationId]=true;
	}

	m_ui64LastStimulationInputStartTime=0;
	m_ui64LastStimulationInputEndTime=0;

	m_ui64SamplingRate.initialize(m_pSignalStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));
	m_pInputStimulationSet.initialize(m_pStimulationStreamDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
	m_pInputSignal.initialize(m_pSignalStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	m_pOutputSignal.initialize(m_pSignalStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));

	getLogManager() << LogLevel_Debug << "Parameters existence : " << m_ui64SamplingRate.exists() << m_pInputStimulationSet.exists() << m_pInputSignal.exists() << m_pOutputSignal.exists() << "\n";

	m_pSignalStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate)->setReferenceTarget(m_pSignalStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));

	m_pOutputSignalDescription=new CMatrix();

	return true;
}

boolean CBoxAlgorithmStimulationBasedEpoching::uninitialize(void)
{
	delete m_pOutputSignalDescription;
	m_pOutputSignalDescription=NULL;

	m_pSignalStreamEncoder->uninitialize();
	m_pSignalStreamDecoder->uninitialize();
	m_pStimulationStreamDecoder->uninitialize();

	getAlgorithmManager().releaseAlgorithm(*m_pSignalStreamEncoder);
	getAlgorithmManager().releaseAlgorithm(*m_pSignalStreamDecoder);
	getAlgorithmManager().releaseAlgorithm(*m_pStimulationStreamDecoder);

	m_vStimulationId.clear();

	std::vector < SStimulationBasedEpoching >::iterator itStimulationBasedEpoching;
	for(itStimulationBasedEpoching=m_vStimulationBasedEpoching.begin(); itStimulationBasedEpoching!=m_vStimulationBasedEpoching.end(); itStimulationBasedEpoching++)
	{
		getAlgorithmManager().releaseAlgorithm(*itStimulationBasedEpoching->m_pEpocher);
	}

	return true;
}

boolean CBoxAlgorithmStimulationBasedEpoching::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmStimulationBasedEpoching::process(void)
{
	// IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();
	uint32 i, j, k;

	// Stimulation input parsing
	for(i=0; i<l_rDynamicBoxContext.getInputChunkCount(1); i++)
	{
		TParameterHandler < const IMemoryBuffer* > l_pStimulationMemoryBuffer(m_pStimulationStreamDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
		l_pStimulationMemoryBuffer=l_rDynamicBoxContext.getInputChunk(1, i);

		m_pStimulationStreamDecoder->process();
		if(m_pStimulationStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			for(j=0; j<m_pInputStimulationSet->getStimulationCount(); j++)
			{
				if(m_vStimulationId.find(m_pInputStimulationSet->getStimulationIdentifier(j))!=m_vStimulationId.end())
				{
					if((int64)m_pInputStimulationSet->getStimulationDate(j)+m_i64EpochOffset>=0)
					{
						SStimulationBasedEpoching l_oEpocher;
						l_oEpocher.m_pEpocher=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_StimulationBasedEpoching));
						l_oEpocher.m_pEpocher->initialize();
						l_oEpocher.m_ui64StimulationTime=m_pInputStimulationSet->getStimulationDate(j);
						l_oEpocher.m_ui64StartTime=m_pInputStimulationSet->getStimulationDate(j)+m_i64EpochOffset;
						l_oEpocher.m_ui64EndTime=m_pInputStimulationSet->getStimulationDate(j)+m_i64EpochOffset+m_ui64EpochDuration;
						l_oEpocher.m_bNeedsReset=true;
						m_vStimulationBasedEpoching.push_back(l_oEpocher);
						getLogManager() << LogLevel_Trace << "Created new epocher at time "
							<< l_oEpocher.m_ui64StimulationTime << ":"
							<< l_oEpocher.m_ui64StartTime << ":"
							<< l_oEpocher.m_ui64EndTime << "\n";
					}
					else
					{
						getLogManager() << LogLevel_Warning << "Skipped epocher that should have started at a negative time\n";
					}
				}
			}
		}
		m_ui64LastStimulationInputStartTime=l_rDynamicBoxContext.getInputChunkStartTime(1, i);
		m_ui64LastStimulationInputEndTime=l_rDynamicBoxContext.getInputChunkEndTime(1, i);
		l_rDynamicBoxContext.markInputAsDeprecated(1, i);
	}

	// Signal input parsing
	for(i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		if((int64)l_rDynamicBoxContext.getInputChunkEndTime(0, i) <= (int64)m_ui64LastStimulationInputEndTime + m_i64EpochOffset) // preserve enough history
		{
			TParameterHandler < const IMemoryBuffer* > l_pInputSignalMemoryBuffer(m_pSignalStreamDecoder->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
			TParameterHandler < IMemoryBuffer* > l_pOutputSignalMemoryBuffer(m_pSignalStreamEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
			l_pInputSignalMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
			l_pOutputSignalMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);

			m_pSignalStreamDecoder->process();

			if(m_pSignalStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
			{
				m_pOutputSignalDescription->setDimensionCount(2);
				m_pOutputSignalDescription->setDimensionSize(0, m_pInputSignal->getDimensionSize(0));
				m_pOutputSignalDescription->setDimensionSize(1, (m_ui64SamplingRate*(m_ui64EpochDuration>>16))>>16);
				for(k=0; k<m_pInputSignal->getDimensionSize(0); k++)
				{
					m_pOutputSignalDescription->setDimensionLabel(0, k, m_pInputSignal->getDimensionLabel(0, k));
				}
				m_pOutputSignal.setReferenceTarget(m_pOutputSignalDescription);
				m_pSignalStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
				l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_rDynamicBoxContext.getInputChunkStartTime(0, i), l_rDynamicBoxContext.getInputChunkEndTime(0, i));
			}

			if(m_pSignalStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
			{
				vector < SStimulationBasedEpoching >::iterator j;
				for(j=m_vStimulationBasedEpoching.begin(); j!=m_vStimulationBasedEpoching.end(); )
				{
					SStimulationBasedEpoching& l_oEpocher=*j;

					TParameterHandler < IMatrix* > l_pEpocherInputSignal(l_oEpocher.m_pEpocher->getInputParameter(OVP_Algorithm_StimulationBasedEpoching_InputParameterId_InputSignal));
					TParameterHandler < IMatrix* > l_pEpocherOutputSignal(l_oEpocher.m_pEpocher->getOutputParameter(OVP_Algorithm_StimulationBasedEpoching_OutputParameterId_OutputSignal));
					l_pEpocherInputSignal.setReferenceTarget(m_pInputSignal);
					m_pOutputSignal.setReferenceTarget(l_pEpocherOutputSignal);

					if(l_oEpocher.m_bNeedsReset)
					{
						OpenViBEToolkit::Tools::Matrix::copyDescription(*l_pEpocherOutputSignal, *m_pOutputSignalDescription);
						TParameterHandler < int64 > l_ui64OffsetSampleCount(l_oEpocher.m_pEpocher->getInputParameter(OVP_Algorithm_StimulationBasedEpoching_InputParameterId_OffsetSampleCount));
						l_ui64OffsetSampleCount=(m_ui64SamplingRate*((l_oEpocher.m_ui64StartTime-l_rDynamicBoxContext.getInputChunkStartTime(0, i))>>16))>>16;
						l_oEpocher.m_pEpocher->process(OVP_Algorithm_StimulationBasedEpoching_InputTriggerId_Reset);
						l_oEpocher.m_bNeedsReset=false;
					}

					l_oEpocher.m_pEpocher->process(OVP_Algorithm_StimulationBasedEpoching_InputTriggerId_PerformEpoching);

					if(l_oEpocher.m_pEpocher->isOutputTriggerActive(OVP_Algorithm_StimulationBasedEpoching_OutputTriggerId_EpochingDone))
					{
						m_pSignalStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
						l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_oEpocher.m_ui64StartTime, l_oEpocher.m_ui64EndTime);

						getAlgorithmManager().releaseAlgorithm(*l_oEpocher.m_pEpocher);
						j=m_vStimulationBasedEpoching.erase(j);
					}
					else
					{
						j++;
					}
				}
			}

			if(m_pSignalStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedEnd))
			{
				m_pSignalStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeEnd);
				l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_rDynamicBoxContext.getInputChunkStartTime(0, i), l_rDynamicBoxContext.getInputChunkEndTime(0, i));
			}

			l_rDynamicBoxContext.markInputAsDeprecated(0, i);
		}
	}

	return true;
}
