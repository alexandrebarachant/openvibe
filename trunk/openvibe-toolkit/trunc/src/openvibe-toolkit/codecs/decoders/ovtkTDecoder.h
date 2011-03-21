#ifndef __OpenViBEToolkit_TDecoder_H__
#define __OpenViBEToolkit_TDecoder_H__

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#include "../../ovtk_base.h"

namespace OpenViBEToolkit
{
	template <class T>
	class TDecoder : public TCodec < T >
	{
	protected:

		OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > m_pInputMemoryBuffer;

		using TCodec < T >::initialize;
		using TCodec < T >::m_pCodec;

	protected:

		virtual void setInputChunk(const OpenViBE::IMemoryBuffer * pInputChunkMemoryBuffer)
		{
			m_pInputMemoryBuffer = pInputChunkMemoryBuffer;
		}

		virtual OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* >& getInputMemoryBuffer()
		{
			return m_pInputMemoryBuffer;
		}

		virtual OpenViBE::boolean isOutputTriggerActive(OpenViBE::CIdentifier oTrigger)
		{
			return m_pCodec->isOutputTriggerActive(oTrigger);
		}

		virtual OpenViBE::boolean process(OpenViBE::CIdentifier oTrigger)
		{
			return m_pCodec->process(oTrigger);
		}
		virtual OpenViBE::boolean process(void)
		{
			return m_pCodec->process();
		}

	public:

		virtual OpenViBE::boolean decode(OpenViBE::uint32 ui32InputIndex, OpenViBE::uint32 ui32ChunkIndex)
		{
			this->setInputChunk(m_pBoxAlgorithm->getDynamicBoxContext().getInputChunk(ui32InputIndex, ui32ChunkIndex));
			if(! m_pCodec->process()) return false;
			m_pBoxAlgorithm->getDynamicBoxContext().markInputAsDeprecated(ui32InputIndex, ui32ChunkIndex);
		}

		virtual OpenViBE::boolean isHeaderReceived(void) { return false;}
		virtual OpenViBE::boolean isBufferReceived(void) { return false;}
		virtual OpenViBE::boolean isEndReceived(void) { return false;}

	
		virtual OpenViBE::boolean uninitialize(void) { return false;}
		virtual OpenViBE::boolean initialize(void) { return false;}

	};
};

#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#endif //__OpenViBEToolkit_TDecoder_H__
