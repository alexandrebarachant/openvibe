#ifndef __OpenViBEToolkit_TDecoder_H__
#define __OpenViBEToolkit_TDecoder_H__

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#include "../../ovtk_base.h"

#include "../ovtkTCodec.h"

namespace OpenViBEToolkit
{
	template <class T>
	class TDecoder : public T
	{
	protected:

		OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > m_pInputMemoryBuffer;

		using T::m_pCodec;
		using T::m_pBoxAlgorithm;
		
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
		using T::initialize;
		
		virtual OpenViBE::boolean decode(OpenViBE::uint32 ui32InputIndex, OpenViBE::uint32 ui32ChunkIndex)
		{
			return m_pCodec->process();
			return true;
		}

		virtual OpenViBE::boolean isHeaderReceived(void) = 0;
		virtual OpenViBE::boolean isBufferReceived(void) = 0;
		virtual OpenViBE::boolean isEndReceived(void) = 0;

	};
};

#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#endif //__OpenViBEToolkit_TDecoder_H__
