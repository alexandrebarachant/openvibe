#ifndef __OpenViBEToolkit_TStreamedMatrixDecoder_H__
#define __OpenViBEToolkit_TStreamedMatrixDecoder_H__

#include <openvibe/ov_all.h>

namespace OpenViBEToolkit
{
	template <class T>
	class TStreamedMatrixDecoder : public T
	{
	protected:
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_pOutputMatrix;

	protected:
		OpenViBE::boolean initialize()
		{
			m_pCodec = &m_pBoxAlgorithm->getAlgorithmManager().getAlgorithm(m_pBoxAlgorithm->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamDecoder));
			m_pCodec->initialize();
			m_pInputMemoryBuffer.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_InputParameterId_MemoryBufferToDecode));
			m_pOutputMatrix.initialize(m_pCodec->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputParameterId_Matrix));
		
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

			m_pOutputMatrix.uninitialize();
			m_pInputMemoryBuffer.uninitialize();
			m_pCodec->uninitialize();
			m_pBoxAlgorithm->getAlgorithmManager().releaseAlgorithm(*m_pCodec);
			m_pBoxAlgorithm = NULL;

			return true;
		}
		
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > getOutputMatrix()
		{
			return m_pOutputMatrix;
		}

		using T::isHeaderReceived;
		using T::isBufferReceived;
		using T::isEndReceived;

		virtual OpenViBE::boolean isHeaderReceived(void) 
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedHeader);
		}
		virtual OpenViBE::boolean isBufferReceived(void) 
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedBuffer);
		}
		virtual OpenViBE::boolean isEndReceived(void) 
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedEnd);
		}

	};
};


#endif //__OpenViBEToolkit_TStreamedMatrixDecoder_H__