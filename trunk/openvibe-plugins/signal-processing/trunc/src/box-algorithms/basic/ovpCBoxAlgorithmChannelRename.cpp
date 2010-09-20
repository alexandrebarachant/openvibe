#include "ovpCBoxAlgorithmChannelRename.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessing;

namespace
{
	bool split(const std::string& sString, std::vector < std::string >& vPart, unsigned char c)
	{
		std::string::size_type l_uiStart=-1;
		vPart.clear();
		do
		{
			std::string l_sSwap;
			std::string::size_type l_uiNext=sString.find(c, l_uiStart+1);
			if(l_uiNext==std::string::npos)
			{
				if(l_uiStart+1 < sString.length())
				{
					l_sSwap=sString.substr(l_uiStart+1, sString.length()-l_uiStart-1);
				}
			}
			else
			{
				l_sSwap=sString.substr(l_uiStart+1, l_uiNext-l_uiStart-1);
			}
			vPart.push_back(l_sSwap);
			l_uiStart=l_uiNext;
		}
		while(l_uiStart!=std::string::npos);

		return true;
	}
}

boolean CBoxAlgorithmChannelRename::initialize(void)
{
	CString   l_sSettingValue=FSettingValueAutoCast(*this->getBoxAlgorithmContext(), 0);
	split(l_sSettingValue.toASCIIString(), m_vChannelName, OV_Value_EnumeratedStringSeparator);

	m_pStreamDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamDecoder));
	m_pStreamDecoder->initialize();

	m_pStreamEncoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));
	m_pStreamEncoder->initialize();

	ip_pMemoryBuffer.initialize(m_pStreamDecoder->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
	op_pMemoryBuffer.initialize(m_pStreamEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
	ip_pMatrix.initialize(m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
	op_pMatrix.initialize(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));

	m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate)->setReferenceTarget(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));

	return true;
}

boolean CBoxAlgorithmChannelRename::uninitialize(void)
{
	op_pMatrix.uninitialize();
	ip_pMatrix.uninitialize();
	op_pMemoryBuffer.uninitialize();
	ip_pMemoryBuffer.uninitialize();

	m_pStreamEncoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStreamEncoder);
	m_pStreamEncoder=NULL;

	m_pStreamDecoder->uninitialize();
	this->getAlgorithmManager().releaseAlgorithm(*m_pStreamDecoder);
	m_pStreamDecoder=NULL;

	return true;
}

boolean CBoxAlgorithmChannelRename::processInput(uint32 ui32InputIndex)
{
	this->getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CBoxAlgorithmChannelRename::process(void)
{
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
		op_pMemoryBuffer=l_rDynamicBoxContext.getOutputChunk(0);
		m_pStreamDecoder->process();
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
			OpenViBEToolkit::Tools::Matrix::copyDescription(*ip_pMatrix, *op_pMatrix);
			for(uint32 j=0; j<ip_pMatrix->getDimensionSize(0) && j<m_vChannelName.size(); j++)
			{
				ip_pMatrix->setDimensionLabel(0, j, m_vChannelName[j].c_str());
			}
			m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			OpenViBEToolkit::Tools::Matrix::copyContent(*ip_pMatrix, *op_pMatrix);
			m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
			m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeEnd);
		}

		l_rDynamicBoxContext.markOutputAsReadyToSend(0, l_rDynamicBoxContext.getInputChunkStartTime(0, i), l_rDynamicBoxContext.getInputChunkEndTime(0, i));
		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	return true;
}
