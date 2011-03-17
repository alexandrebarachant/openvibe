#ifndef __OpenViBEToolkit_TChannelLocalisationEncoder_H__
#define __OpenViBEToolkit_TChannelLocalisationEncoder_H__

#include <openvibe/ov_all.h>

namespace OpenViBEToolkit
{
	template <class T>
	class TChannelLocalisationEncoder : public T
	{
	protected:
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > m_pInputLocalisationMatrix;
		OpenViBE::Kernel::TParameterHandler < OpenViBE::boolean > m_bInputDynamic;

	protected:
		OpenViBE::boolean initialize()
		{
			m_pCodec = &m_pBoxAlgorithm->getAlgorithmManager().getAlgorithm(m_pBoxAlgorithm->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_ChannelLocalisationStreamEncoder));
			m_pCodec->initialize();
			m_pInputLocalisationMatrix.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_ChannelLocalisationStreamEncoder_InputParameterId_Matrix));
			m_bInputDynamic.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_ChannelLocalisationStreamEncoder_InputParameterId_Dynamic));
			m_pOutputMemoryBuffer.initialize(m_pCodec->getOutputParameter(OVP_GD_Algorithm_ChannelLocalisationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
		
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

			m_pInputLocalisationMatrix.uninitialize();
			m_bInputDynamic.uninitialize();
			m_pOutputMemoryBuffer.uninitialize();
			m_pCodec->uninitialize();
			m_pBoxAlgorithm->getAlgorithmManager().releaseAlgorithm(*m_pCodec);
			m_pBoxAlgorithm = NULL;

			return true;
		}

		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > getInputLocalisationMatrix()
		{
			return m_pInputLocalisationMatrix;
		}
		OpenViBE::Kernel::TParameterHandler < OpenViBE::boolean > getInputDynamic()
		{
			return m_bInputDynamic;
		}
		
		OpenViBE::boolean encodeHeader(void) 
		{
			return m_pCodec->process(OVP_GD_Algorithm_ChannelLocalisationStreamEncoder_InputTriggerId_EncodeHeader);
		}
		OpenViBE::boolean encodeBuffer(void) 
		{
			return m_pCodec->process(OVP_GD_Algorithm_ChannelLocalisationStreamEncoder_InputTriggerId_EncodeBuffer);
		}
		OpenViBE::boolean encodeEnd(void) 
		{
			return m_pCodec->process(OVP_GD_Algorithm_ChannelLocalisationStreamEncoder_InputTriggerId_EncodeEnd);
		}
	};
};


#endif //__OpenViBEToolkit_TChannelLocalisationEncoder_H__