#ifndef __OpenViBEToolkit_TChannelLocalisationDecoder_H__
#define __OpenViBEToolkit_TChannelLocalisationDecoder_H__

#include <openvibe/ov_all.h>

namespace OpenViBEToolkit
{
	template <class T>
	class TChannelLocalisationDecoder : public T
	{
	protected:
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_pOutputLocalisationMatrix;

	protected:
		OpenViBE::boolean initialize()
		{
			m_pCodec = &m_pBoxAlgorithm->getAlgorithmManager().getAlgorithm(m_pBoxAlgorithm->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_ChannelLocalisationStreamDecoder));
			m_pCodec->initialize();
			m_pInputMemoryBuffer.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_ChannelLocalisationStreamDecoder_InputParameterId_MemoryBufferToDecode));
			m_pOutputLocalisationMatrix.initialize(m_pCodec->getOutputParameter(OVP_GD_Algorithm_ChannelLocalisationStreamDecoder_OutputParameterId_Matrix));
		
			return true;
		}

	public:
		using T::initialize;

		OpenViBE::boolean uninitialize(void)
		{
			if(m_pBoxAlgorithm == NULL)
			{
					return false;
			}

			m_pOutputLocalisationMatrix.uninitialize();
			m_pInputMemoryBuffer.uninitialize();
			m_pCodec->uninitialize();
			m_pBoxAlgorithm->getAlgorithmManager().releaseAlgorithm(*m_pCodec);
			m_pBoxAlgorithm = NULL;

			return true;
		}
		
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > getOutputLocalisationMatrix()
		{
			return m_pOutputLocalisationMatrix;
		}

		virtual OpenViBE::boolean isHeaderReceived() 
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_ChannelLocalisationStreamDecoder_OutputTriggerId_ReceivedHeader);
		}
		virtual OpenViBE::boolean isBufferReceived() 
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_ChannelLocalisationStreamDecoder_OutputTriggerId_ReceivedBuffer);
		}
		virtual OpenViBE::boolean isEndReceived() 
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_ChannelLocalisationStreamDecoder_OutputTriggerId_ReceivedEnd);
		}
	};
};


#endif //__OpenViBEToolkit_TChannelLocalisationDecoder_H__