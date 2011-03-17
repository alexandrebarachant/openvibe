#ifndef __OpenViBEToolkit_TFeatureVectorEncoder_H__
#define __OpenViBEToolkit_TFeatureVectorEncoder_H__

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#include "../../ovtk_base.h"

#include "ovtkTEncoder.h"

namespace OpenViBEToolkit
{
	template <class T>
	class TFeatureVectorEncoderLocal : public T
	{
	protected:

		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_pInputVector;

	protected:

		OpenViBE::boolean initialize()
		{
			m_pCodec = &m_pBoxAlgorithm->getAlgorithmManager().getAlgorithm(m_pBoxAlgorithm->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_FeatureVectorStreamEncoder));
			m_pCodec->initialize();
			m_pInputVector.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_FeatureVectorStreamEncoder_InputParameterId_Matrix));
			m_pOutputMemoryBuffer.initialize(m_pCodec->getOutputParameter(OVP_GD_Algorithm_FeatureVectorStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

			return true;
		}

	public:

		using T::initialize;
		using T::m_pCodec;
		using T::m_pBoxAlgorithm;
		using T::m_pOutputMemoryBuffer;

		OpenViBE::boolean uninitialize(void)
		{
			if(m_pBoxAlgorithm == NULL || m_pCodec == NULL)
			{
				return false;
			}

			m_pInputVector.uninitialize();
			m_pOutputMemoryBuffer.uninitialize();
			m_pCodec->uninitialize();
			m_pBoxAlgorithm->getAlgorithmManager().releaseAlgorithm(*m_pCodec);
			m_pBoxAlgorithm = NULL;

			return true;
		}

		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* >& getInputVector()
		{
			return m_pInputVector;
		}

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
