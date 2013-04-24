#if defined TARGET_HAS_ThirdPartyVRPN

#include "ovpCBoxAlgorithmVRPNAnalogClient.h"
#include "openvibe/ovITimeArithmetics.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::VRPN;

#define boolean OpenViBE::boolean

namespace
{
	static void VRPN_CALLBACK vrpn_analog_cb(void* pUserData, const vrpn_ANALOGCB a)
	{
		OpenViBEPlugins::VRPN::CBoxAlgorithmVRPNAnalogClient* l_pClient=static_cast < OpenViBEPlugins::VRPN::CBoxAlgorithmVRPNAnalogClient* > (pUserData);
		l_pClient->setAnalog(a.num_channel, a.channel);
	}
}

uint64 CBoxAlgorithmVRPNAnalogClient::getClockFrequency(void)
{
	return 128LL<<32; // the box clock frequency
}

boolean CBoxAlgorithmVRPNAnalogClient::initialize(void)
{
	m_pVRPNAnalogRemote=NULL;

	m_sPeripheralName=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 0);
	m_ui64SamplingRate=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 1);
	m_ui32ChannelCount=uint32((uint64)FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 2));
	m_ui32SampleCountPerSentBlock=uint32((uint64)FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 3));

	m_pStreamEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));
	m_pStreamEncoder->initialize();

	ip_pMatrix.initialize(m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
	ip_ui64SamplingRate.initialize(m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate));
	op_pMemoryBuffer.initialize(m_pStreamEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	m_bFirstStart = true;
	m_ui64LastChunkEndTime=0;
	m_ui64ChunkDuration=(uint64(m_ui32SampleCountPerSentBlock)<<32)/m_ui64SamplingRate;

	m_vLastSample.resize(m_ui32ChannelCount);
	m_dSampleBuffer.clear();

	return true;
}

boolean CBoxAlgorithmVRPNAnalogClient::uninitialize(void)
{
	if(m_pVRPNAnalogRemote)
	{
		delete m_pVRPNAnalogRemote;
		m_pVRPNAnalogRemote=NULL;
	}

	if(m_pStreamEncoder)
	{
		m_pStreamEncoder->uninitialize();
		this->getAlgorithmManager().releaseAlgorithm(*m_pStreamEncoder);
		m_pStreamEncoder=NULL;
	}

	m_dSampleBuffer.clear();
	m_vLastSample.clear();

	return true;
}

boolean CBoxAlgorithmVRPNAnalogClient::processClock(IMessageClock& rMessageClock)
{
	this->getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmVRPNAnalogClient::process(void)
{
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	if(m_bFirstStart)
	{
		ip_ui64SamplingRate=m_ui64SamplingRate;
		ip_pMatrix->setDimensionCount(2);
		ip_pMatrix->setDimensionSize(0, m_ui32ChannelCount);
		ip_pMatrix->setDimensionSize(1, m_ui32SampleCountPerSentBlock);

		// @TODO do labels ?

		op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);
		m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
		l_rDynamicBoxContext.markOutputAsReadyToSend(0, 0, 0);

		m_vLastSample.resize(m_ui32ChannelCount);
		for(uint32 i=0;i<m_ui32ChannelCount;i++) {
			m_vLastSample[i] = 0;
		}

		m_bFirstStart = false;
	}

	if(!m_pVRPNAnalogRemote) {
		m_pVRPNAnalogRemote=new vrpn_Analog_Remote(m_sPeripheralName.toASCIIString());
		m_pVRPNAnalogRemote->register_change_handler(this, &vrpn_analog_cb);
	}

	m_pVRPNAnalogRemote->mainloop();

	const uint64 l_ui64CurrentTime=this->getPlayerContext().getCurrentTime();

	if( (l_ui64CurrentTime-m_ui64LastChunkEndTime) >= m_ui64ChunkDuration ) 
	{
		// Time to send a chunk. Copy our current sample buffer to the output matrix.
		uint32 l_ui32SampleIndex=0;
		OpenViBE::float64 *l_pOutputBuffer = ip_pMatrix->getBuffer();

		while(m_dSampleBuffer.size()>0) 
		{
			const std::vector< OpenViBE::float64 > &l_vTempSample = m_dSampleBuffer.front();

			for(uint32 j=0; j<m_ui32ChannelCount; j++) {
				l_pOutputBuffer[j*m_ui32SampleCountPerSentBlock + l_ui32SampleIndex ] = l_vTempSample[j];
			}

			m_dSampleBuffer.pop_front();
			l_ui32SampleIndex++;
		}
		// If the buffer didn't have enough data from callbacks, pad with the last sample
		while(l_ui32SampleIndex<m_ui32SampleCountPerSentBlock) 
		{
			for(uint32 j=0; j<m_ui32ChannelCount; j++) {
				l_pOutputBuffer[j*m_ui32SampleCountPerSentBlock + l_ui32SampleIndex ] = m_vLastSample[j];
			}
			l_ui32SampleIndex++;
		}

		op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);
		m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
		l_rDynamicBoxContext.markOutputAsReadyToSend(0, m_ui64LastChunkEndTime, m_ui64LastChunkEndTime + m_ui64ChunkDuration);
		m_ui64LastChunkEndTime += m_ui64ChunkDuration;
#if defined _DEBUG
		const float64 l_f64TimeDiff = OpenViBE::ITimeArithmetics::timeToSeconds(m_ui64LastChunkEndTime)
			- OpenViBE::ITimeArithmetics::timeToSeconds(l_ui64CurrentTime);
		if(std::abs(l_f64TimeDiff) > 1) {
			this->getLogManager() << LogLevel_Warning 
				<< "Time difference detected: " << l_f64TimeDiff << " secs.\n";
		}
#endif		
	}

	return true;
}

void CBoxAlgorithmVRPNAnalogClient::setAnalog(uint32 ui32AnalogCount, const float64* pAnalog)
{
	// Count how many samples are encoded in pAnalog
	const uint32 l_ui32SampleCount = ui32AnalogCount / m_ui32ChannelCount;
	for(uint32 i=0;i<l_ui32SampleCount;i++) 
	{
		// Append the sample to the buffer
		for(uint32 j=0; j<m_ui32ChannelCount; j++) 
		{
			m_vLastSample[j] = pAnalog[j*l_ui32SampleCount+i];
		}
		m_dSampleBuffer.push_back(m_vLastSample);

		// Drop the oldest sample if our buffer got full. This means that VRPN is sending data faster than we consume.
		if(m_dSampleBuffer.size()>m_ui32SampleCountPerSentBlock) 
		{
			m_dSampleBuffer.pop_front();
		}
	}
}

#endif // TARGET_HAS_ThirdPartyVRPN
