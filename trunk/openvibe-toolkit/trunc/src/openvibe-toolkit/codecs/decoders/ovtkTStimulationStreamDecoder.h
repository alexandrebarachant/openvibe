#ifndef __OpenViBEToolkit_TStimulationStreamDecoder_H__
#define __OpenViBEToolkit_TStimulationStreamDecoder_H__

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#include "../../ovtk_base.h"

#include "ovtkTDecoder.h"

namespace OpenViBEToolkit
{
	template <class T>
	class TStimulationStreamDecoderLocal : public T
	{

	protected:

		OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > m_pOutputStimulationSet;
		
		using T::m_pCodec;
		using T::m_pBoxAlgorithm;
		using T::m_pInputMemoryBuffer;

		OpenViBE::boolean initializeImpl()
		{
			m_pCodec = &m_pBoxAlgorithm->getAlgorithmManager().getAlgorithm(m_pBoxAlgorithm->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
			m_pCodec->initialize();
			m_pOutputStimulationSet.initialize(m_pCodec->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
			m_pInputMemoryBuffer.initialize(m_pCodec->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));

			return true;
		}

	public:
		using T::initialize;
		
		OpenViBE::boolean uninitialize(void)
		{
			if(m_pBoxAlgorithm == NULL || m_pCodec == NULL)
			{
				return false;
			}

			m_pOutputStimulationSet.uninitialize();
			m_pInputMemoryBuffer.uninitialize();
			m_pCodec->uninitialize();
			m_pBoxAlgorithm->getAlgorithmManager().releaseAlgorithm(*m_pCodec);
			m_pBoxAlgorithm = NULL;

			return true;
		}

		OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* >& getOutputStimulationSet()
		{
			return m_pOutputStimulationSet;
		}

		virtual OpenViBE::boolean isHeaderReceived()
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader);
		}

		virtual OpenViBE::boolean isBufferReceived()
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer);
		}

		virtual OpenViBE::boolean isEndReceived()
		{
			return m_pCodec->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd);
		}
	};

	template <class T>
	class TStimulationStreamDecoder : public TStimulationStreamDecoderLocal < TDecoder < T > >
	{
	private:
		using TStimulationStreamDecoderLocal < TDecoder < T > >::m_pBoxAlgorithm;
	public:
		using TStimulationStreamDecoderLocal < TDecoder < T > >::uninitialize;

		TStimulationStreamDecoder()
		{
		}
		TStimulationStreamDecoder(T& rBoxAlgorithm)
		{
			m_pBoxAlgorithm = NULL;
			initialize(rBoxAlgorithm);
		}
		~TStimulationStreamDecoder()
		{
			uninitialize();
		}
	};
};

#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#endif //__OpenViBEToolkit_TStimulationStreamDecoder_H__
