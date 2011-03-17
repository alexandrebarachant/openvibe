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

	public:

		using TCodec < T >::initialize;
		using TCodec < T >::m_pCodec;

		virtual OpenViBE::boolean uninitialize(void) { return false;}

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

		virtual OpenViBE::boolean decode(void)
		{
			return m_pCodec->process();
		}

		virtual OpenViBE::boolean isHeaderReceived(void) { return false;}
		virtual OpenViBE::boolean isBufferReceived(void) { return false;}
		virtual OpenViBE::boolean isEndReceived(void) { return false;}

	protected:

		virtual OpenViBE::boolean initialize(void) { return false;}

	};
};

#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#endif //__OpenViBEToolkit_TDecoder_H__
