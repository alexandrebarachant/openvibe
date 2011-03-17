#ifndef __OpenViBEToolkit_TFeatureVectorDecoder_H__
#define __OpenViBEToolkit_TFeatureVectorDecoder_H__

#include <openvibe/ov_all.h>

namespace OpenViBEToolkit
{
	template <class T>
	class TFeatureVectorDecoder : public T
	{
	protected:
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_pOutputVector;

	protected:
		OpenViBE::boolean initialize()
		{
			m_pCodec = &m_pBoxAlgorithm->getAlgorithmManager().getAlgorithm(m_pBoxAlgorithm->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_FeatureVectorStreamDecoder));
			m_pCodec->initialize();
			m_pInputMemoryBuffer.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_FeatureVectorStreamDecoder_InputParameterId_MemoryBufferToDecode));
			m_pOutputVector.initialize(m_pCodec->getOutputParameter(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputParameterId_Matrix));
		
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

			m_pOutputVector.uninitialize();
			m_pInputMemoryBuffer.uninitialize();
			m_pCodec->uninitialize();
			m_pBoxAlgorithm->getAlgorithmManager().releaseAlgorithm(*m_pCodec);
			m_pBoxAlgorithm = NULL;

			return true;
		}
		
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > getOutputVector()
		{
			return m_pOutputVector;
		}

		virtual OpenViBE::boolean isHeaderReceived() 
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputTriggerId_ReceivedHeader);
		}
		virtual OpenViBE::boolean isBufferReceived() 
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputTriggerId_ReceivedBuffer);
		}
		virtual OpenViBE::boolean isEndReceived() 
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputTriggerId_ReceivedEnd);
		}
	};
};


#endif //__OpenViBEToolkit_TFeatureVectorDecoder_H__