#ifndef __OpenViBEToolkit_TStimulationStreamEncoder_H__
#define __OpenViBEToolkit_TStimulationStreamEncoder_H__

#include <openvibe/ov_all.h>

namespace OpenViBEToolkit
{
	template <class T>
	class TStimulationStreamEncoder : public T
	{
	protected:
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > m_pInputStimulationSet;

	protected:
		OpenViBE::boolean initialize()
		{
			m_pCodec = &m_pBoxAlgorithm->getAlgorithmManager().getAlgorithm(m_pBoxAlgorithm->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamEncoder));
			m_pCodec->initialize();
			m_pInputStimulationSet.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_InputParameterId_StimulationSet));
			m_pOutputMemoryBuffer.initialize(m_pCodec->getOutputParameter(OVP_GD_Algorithm_StimulationStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
		
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

			m_pInputStimulationSet.uninitialize();
			m_pOutputMemoryBuffer.uninitialize();
			m_pCodec->uninitialize();
			m_pBoxAlgorithm->getAlgorithmManager().releaseAlgorithm(*m_pCodec);
			m_pBoxAlgorithm = NULL;

			return true;
		}

		OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > getInputStimulationSet()
		{
			return m_pInputStimulationSet;
		}

		OpenViBE::boolean encodeHeader(void) 
		{
			return m_pCodec->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeHeader);
		}
		OpenViBE::boolean encodeBuffer(void) 
		{
			return m_pCodec->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeBuffer);
		}
		OpenViBE::boolean encodeEnd(void) 
		{
			return m_pCodec->process(OVP_GD_Algorithm_StimulationStreamEncoder_InputTriggerId_EncodeEnd);
		}
		
	};
};


#endif //__OpenViBEToolkit_TStimulationStreamEncoder_H__