#ifndef __OpenViBEToolkit_TFeatureVectorEncoder_H__
#define __OpenViBEToolkit_TFeatureVectorEncoder_H__

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#include "../../ovtk_base.h"

#include "ovtkTStreamedMatrixEncoder.h"

namespace OpenViBEToolkit
{
	template <class T>
	class TFeatureVectorEncoderLocal : public TStreamedMatrixEncoderLocal <T>
	{
	
	protected:

		OpenViBE::boolean initialize()
		{
			m_pCodec = &m_pBoxAlgorithm->getAlgorithmManager().getAlgorithm(m_pBoxAlgorithm->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_FeatureVectorStreamEncoder));
			m_pCodec->initialize();
			m_pInputMatrix.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_FeatureVectorStreamEncoder_InputParameterId_Matrix));
			m_pOutputMemoryBuffer.initialize(m_pCodec->getOutputParameter(OVP_GD_Algorithm_FeatureVectorStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

			return true;
		}

	public:

		using TStreamedMatrixEncoderLocal::initialize;
		using TStreamedMatrixEncoderLocal::uninitialize;
		using TStreamedMatrixEncoderLocal::m_pCodec;
		using TStreamedMatrixEncoderLocal::m_pBoxAlgorithm;
		using TStreamedMatrixEncoderLocal::m_pOutputMemoryBuffer;

	protected:
		OpenViBE::boolean encodeHeader(void)
		{
			return m_pCodec->process(OVP_GD_Algorithm_FeatureVectorStreamEncoder_InputTriggerId_EncodeHeader);
		}

		OpenViBE::boolean encodeBuffer(void)
		{
			return m_pCodec->process(OVP_GD_Algorithm_FeatureVectorStreamEncoder_InputTriggerId_EncodeBuffer);
		}

		OpenViBE::boolean encodeEnd(void)
		{
			return m_pCodec->process(OVP_GD_Algorithm_FeatureVectorStreamEncoder_InputTriggerId_EncodeEnd);
		}
	};

	template <class T>
	class TFeatureVectorEncoder : public TFeatureVectorEncoderLocal < TEncoder < T > >
	{
	};
};

#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#endif //__OpenViBEToolkit_TFeatureVectorEncoder_H__
