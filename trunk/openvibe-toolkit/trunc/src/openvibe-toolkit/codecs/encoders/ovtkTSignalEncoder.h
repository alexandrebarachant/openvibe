#ifndef __OpenViBEToolkit_TSignalEncoder_H__
#define __OpenViBEToolkit_TSignalEncoder_H__

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#include "../../ovtk_base.h"

#include "ovtkTStreamedMatrixEncoder.h"

namespace OpenViBEToolkit
{
	template <class T>
	class TSignalEncoderLocal : public TStreamedMatrixEncoderLocal <T>
	{
	protected:

		OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > m_pInputSamplingRate;

	protected:

		OpenViBE::boolean initialize()
		{
			m_pCodec = &m_pBoxAlgorithm->getAlgorithmManager().getAlgorithm(m_pBoxAlgorithm->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SignalStreamEncoder));
			m_pCodec->initialize();
			m_pInputMatrix.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
			m_pInputSamplingRate.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate));
			m_pOutputMemoryBuffer.initialize(m_pCodec->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

			return true;
		}

	public:

		using TStreamedMatrixEncoderLocal::initialize;
		using TStreamedMatrixEncoderLocal::m_pCodec;
		using TStreamedMatrixEncoderLocal::m_pBoxAlgorithm;
		using TStreamedMatrixEncoderLocal::m_pOutputMemoryBuffer;

		OpenViBE::boolean uninitialize(void)
		{
			if(m_pBoxAlgorithm == NULL || m_pCodec == NULL)
			{
				return false;
			}

			m_pInputMatrix.uninitialize();
			m_pInputSamplingRate.uninitialize();
			m_pOutputMemoryBuffer.uninitialize();
			m_pCodec->uninitialize();
			m_pBoxAlgorithm->getAlgorithmManager().releaseAlgorithm(*m_pCodec);
			m_pBoxAlgorithm = NULL;

			return true;
		}

		OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 >& getInputSamplingRate()
		{
			return m_pInputSamplingRate;
		}

	protected:
		OpenViBE::boolean encodeHeader(void)
		{
			return process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
		}

		OpenViBE::boolean encodeBuffer(void)
		{
			return process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
		}

		OpenViBE::boolean encodeEnd(void)
		{
			return process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeEnd);
		}
	};

	template <class T>
	class TSignalEncoder : public TSignalEncoderLocal < TEncoder < T > >
	{
	};
};

#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#endif //__OpenViBEToolkit_TSignalEncoder_H__
