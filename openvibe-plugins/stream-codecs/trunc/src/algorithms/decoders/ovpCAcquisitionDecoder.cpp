#include "ovpCAcquisitionDecoder.h"

#include <system/Memory.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::StreamCodecs;

CAcquisitionDecoder::CAcquisitionDecoder(void)
	:m_ui32ChannelIndex(0)
	,m_ui32SampleCountPerBuffer(0)
{
}

// ________________________________________________________________________________________________________________
//

boolean CAcquisitionDecoder::initialize(void)
{
	CEBMLBaseDecoder::initialize();

	m_oExperimentIdHandle   .initialize(getOutputParameter(OVP_AcquisitionDecoder_ExperimentId_OutParameterId));
	m_oSubjectAgeHandle     .initialize(getOutputParameter(OVP_AcquisitionDecoder_SubjectAge_OutParameterId));
	m_oSubjectGenderHandle  .initialize(getOutputParameter(OVP_AcquisitionDecoder_SubjectGender_OutParameterId));
	m_oSamplingRateHandle   .initialize(getOutputParameter(OVP_AcquisitionDecoder_SamplingRate_OutParameterId));
	m_oChannelLocationHandle.initialize(getOutputParameter(OVP_AcquisitionDecoder_ChannelLocation_OutParameterId));
	m_oSignalHandle         .initialize(getOutputParameter(OVP_AcquisitionDecoder_Signal_OutParameterId));
	m_oStimulationSetHandle .initialize(getOutputParameter(OVP_AcquisitionDecoder_StimulationSet_OutParameterId));

	m_oChannelLocationHandle->setDimensionCount(1);

	m_oSignalHandle->setDimensionCount(2);

	return true;
}

boolean CAcquisitionDecoder::uninitialize(void)
{
	m_oStimulationSetHandle .uninitialize();
	m_oSignalHandle         .uninitialize();
	m_oChannelLocationHandle.uninitialize();
	m_oSamplingRateHandle   .uninitialize();
	m_oSubjectGenderHandle  .uninitialize();
	m_oSubjectAgeHandle     .uninitialize();
	m_oExperimentIdHandle   .uninitialize();

	CEBMLBaseDecoder::uninitialize();

	return true;
}

// ________________________________________________________________________________________________________________
//

EBML::boolean CAcquisitionDecoder::isMasterChild(const EBML::CIdentifier& rIdentifier)
{
	     if(rIdentifier==OVTK_NodeId_Acquisition_Header)                 return true;
	else if(rIdentifier==OVTK_NodeId_Acquisition_AcquisitionInformation) return true;
	else if(rIdentifier==OVTK_NodeId_Acquisition_ExperimentId)           return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_SubjectAge)             return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_SubjectGender)          return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_ChannelCount)           return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_SamplingFrequency)      return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_GainFactors)            return true;
	else if(rIdentifier==OVTK_NodeId_Acquisition_GainFactor)             return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_ChannelNames)           return true;
	else if(rIdentifier==OVTK_NodeId_Acquisition_ChannelName)            return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_ChannelLocations)       return true;
	else if(rIdentifier==OVTK_NodeId_Acquisition_ChannelLocation)        return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_Buffer)                 return true;
	else if(rIdentifier==OVTK_NodeId_Acquisition_Samples)                return true;
	else if(rIdentifier==OVTK_NodeId_Acquisition_SamplesPerChannelCount) return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_SampleBlock)            return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_Stimulations)           return true;
	else if(rIdentifier==OVTK_NodeId_Acquisition_StimulationsCount)      return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_Stimulation)            return true;
	else if(rIdentifier==OVTK_NodeId_Acquisition_StimulationSampleIndex) return false;
	else if(rIdentifier==OVTK_NodeId_Acquisition_StimulationIdentifier)  return false;
	return CEBMLBaseDecoder::isMasterChild(rIdentifier);
}

void CAcquisitionDecoder::openChild(const EBML::CIdentifier& rIdentifier)
{
	m_vNodes.push(rIdentifier);

	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop==OVTK_NodeId_Acquisition_Header)
	 ||(l_rTop==OVTK_NodeId_Acquisition_AcquisitionInformation)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ExperimentId)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SubjectAge)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SubjectGender)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelCount)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SamplingFrequency)
	 ||(l_rTop==OVTK_NodeId_Acquisition_GainFactors)
	 ||(l_rTop==OVTK_NodeId_Acquisition_GainFactor)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelNames)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelName)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelLocations)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelLocation)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Buffer)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Samples)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SamplesPerChannelCount)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SampleBlock)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Stimulations)
	 ||(l_rTop==OVTK_NodeId_Acquisition_StimulationsCount)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Stimulation)
	 ||(l_rTop==OVTK_NodeId_Acquisition_StimulationSampleIndex)
	 ||(l_rTop==OVTK_NodeId_Acquisition_StimulationIdentifier))
	{
		if(l_rTop==OVTK_NodeId_Acquisition_Stimulations)
		{
			m_vStimulationIdentifier.clear();
			m_vStimulationSampleIndex.clear();
		}
	}
	else
	{
		CEBMLBaseDecoder::openChild(rIdentifier);
	}
}

void CAcquisitionDecoder::processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop==OVTK_NodeId_Acquisition_Header)
	 ||(l_rTop==OVTK_NodeId_Acquisition_AcquisitionInformation)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ExperimentId)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SubjectAge)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SubjectGender)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelCount)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SamplingFrequency)
	 ||(l_rTop==OVTK_NodeId_Acquisition_GainFactors)
	 ||(l_rTop==OVTK_NodeId_Acquisition_GainFactor)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelNames)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelName)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelLocations)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelLocation)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Buffer)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Samples)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SamplesPerChannelCount)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SampleBlock)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Stimulations)
	 ||(l_rTop==OVTK_NodeId_Acquisition_StimulationsCount)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Stimulation)
	 ||(l_rTop==OVTK_NodeId_Acquisition_StimulationSampleIndex)
	 ||(l_rTop==OVTK_NodeId_Acquisition_StimulationIdentifier))
	{
		if(l_rTop==OVTK_NodeId_Acquisition_ExperimentId)           { m_oExperimentIdHandle=m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Acquisition_SubjectAge)             { m_oSubjectAgeHandle=m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Acquisition_SubjectGender)          { m_oSubjectGenderHandle=m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Acquisition_ChannelCount)           { m_oSignalHandle->setDimensionSize(0, (uint32)m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize)); }
		if(l_rTop==OVTK_NodeId_Acquisition_SamplingFrequency)      { m_oSamplingRateHandle=m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Acquisition_GainFactors)            { m_ui32ChannelIndex=0; }
		if(l_rTop==OVTK_NodeId_Acquisition_GainFactor)             { m_vChannelGainFactor.push_back(m_pEBMLReaderHelper->getFloatFromChildData(pBuffer, ui64BufferSize)); }
		if(l_rTop==OVTK_NodeId_Acquisition_ChannelNames)           { m_ui32ChannelIndex=0; }
		if(l_rTop==OVTK_NodeId_Acquisition_ChannelName)            { m_oSignalHandle->setDimensionLabel(0, m_ui32ChannelIndex++, m_pEBMLReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize)); }
		if(l_rTop==OVTK_NodeId_Acquisition_ChannelLocations)       { m_ui32ChannelIndex=0; }
		if(l_rTop==OVTK_NodeId_Acquisition_ChannelLocation)
		{
			const float64* l_pLocation=(const float64*) pBuffer;
			m_vChannelLocation.push_back(l_pLocation[0]);
			m_vChannelLocation.push_back(l_pLocation[1]);
			m_vChannelLocation.push_back(l_pLocation[2]);
		}
		if(l_rTop==OVTK_NodeId_Acquisition_SamplesPerChannelCount) { m_oSignalHandle->setDimensionSize(1, (uint32)m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize)); }
		if(l_rTop==OVTK_NodeId_Acquisition_SampleBlock)            { System::Memory::copy(m_oSignalHandle->getBuffer()+(m_ui32ChannelIndex++)*m_oSignalHandle->getDimensionSize(1), pBuffer, ui64BufferSize); }
		if(l_rTop==OVTK_NodeId_Acquisition_StimulationIdentifier)  { m_vStimulationIdentifier.push_back(m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize)); }
		if(l_rTop==OVTK_NodeId_Acquisition_StimulationSampleIndex) { m_vStimulationSampleIndex.push_back(m_pEBMLReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize)); }
	}
	else
	{
		CEBMLBaseDecoder::processChildData(pBuffer, ui64BufferSize);
	}
}

void CAcquisitionDecoder::closeChild(void)
{
	EBML::CIdentifier& l_rTop=m_vNodes.top();

	if((l_rTop==OVTK_NodeId_Acquisition_Header)
	 ||(l_rTop==OVTK_NodeId_Acquisition_AcquisitionInformation)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ExperimentId)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SubjectAge)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SubjectGender)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelCount)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SamplingFrequency)
	 ||(l_rTop==OVTK_NodeId_Acquisition_GainFactors)
	 ||(l_rTop==OVTK_NodeId_Acquisition_GainFactor)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelNames)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelName)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelLocations)
	 ||(l_rTop==OVTK_NodeId_Acquisition_ChannelLocation)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Buffer)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Samples)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SamplesPerChannelCount)
	 ||(l_rTop==OVTK_NodeId_Acquisition_SampleBlock)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Stimulations)
	 ||(l_rTop==OVTK_NodeId_Acquisition_StimulationsCount)
	 ||(l_rTop==OVTK_NodeId_Acquisition_Stimulation)
	 ||(l_rTop==OVTK_NodeId_Acquisition_StimulationSampleIndex)
	 ||(l_rTop==OVTK_NodeId_Acquisition_StimulationIdentifier))
	{
		if(l_rTop==OVTK_NodeId_Acquisition_Stimulations)
		{
			if(m_vStimulationIdentifier.size() == m_vStimulationSampleIndex.size() && m_vStimulationIdentifier.size() != 0)
			{
				size_t l_iStimulationCount=m_vStimulationIdentifier.size();
				m_oStimulationSetHandle->setStimulationCount(l_iStimulationCount);
				for(size_t i=0; i<l_iStimulationCount; i++)
				{
					m_oStimulationSetHandle->setStimulationIdentifier(i, m_vStimulationIdentifier[i]);
					m_oStimulationSetHandle->setStimulationDate(i, (m_vStimulationSampleIndex[i]<<32)/m_oSamplingRateHandle);
					m_oStimulationSetHandle->setStimulationDuration(i, 0);
				}
			}
		}
	}
	else
	{
		CEBMLBaseDecoder::closeChild();
	}

	m_vNodes.pop();
}
