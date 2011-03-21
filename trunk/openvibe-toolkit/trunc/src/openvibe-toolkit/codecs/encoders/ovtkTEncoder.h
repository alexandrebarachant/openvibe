#ifndef __OpenViBEToolkit_TEncoder_H__
#define __OpenViBEToolkit_TEncoder_H__

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#include "../../ovtk_base.h"

namespace OpenViBEToolkit
{
	template <class T>
	class TEncoder : public TCodec < T >
	{
	protected:

		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > m_pOutputMemoryBuffer;

		using TCodec < T >::initialize;
		using TCodec < T >::m_pCodec;

		virtual void setOutputChunk(OpenViBE::IMemoryBuffer * pOutputChunkMemoryBuffer)
		{
			m_pOutputMemoryBuffer = pOutputChunkMemoryBuffer;
		}

		virtual OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* >& getOutputMemoryBuffer()
		{
			return m_pOutputMemoryBuffer;
		}

		virtual OpenViBE::boolean isOutputTriggerActive(OpenViBE::CIdentifier oTrigger)
		{
			return m_pCodec->isOutputTriggerActive(oTrigger);
		}

		virtual OpenViBE::boolean process(OpenViBE::CIdentifier &oTrigger)
		{
			return m_pCodec->process(oTrigger);
		}
		virtual OpenViBE::boolean process(void)
		{
			return m_pCodec->process();
		}

	
		virtual OpenViBE::boolean encodeHeader() { return false;}
		virtual OpenViBE::boolean encodeBuffer() { return false;}
		virtual OpenViBE::boolean encodeEnd() { return false;}

	public:
		OpenViBE::boolean encodeHeader(OpenViBE::uint32 ui32OutputIndex, OpenViBE::uint64 ui64StartTime, OpenViBE::uint64 ui64EndTime)
		{
			this->setOutputChunk(m_pBoxAlgorithm->getDynamicBoxContext().getOutputChunk(ui32OutputIndex));
			if(!this->encodeHeader()) return false;
			return m_pBoxAlgorithm->getDynamicBoxContext().markOutputAsReadyToSend(ui32OutputIndex, ui64StartTime, ui64EndTime);
		}

		OpenViBE::boolean encodeBuffer(OpenViBE::uint32 ui32OutputIndex, OpenViBE::uint64 ui64StartTime, OpenViBE::uint64 ui64EndTime)
		{
			this->setOutputChunk(m_pBoxAlgorithm->getDynamicBoxContext().getOutputChunk(ui32OutputIndex));
			if(!this->encodeBuffer()) return false;
			return m_pBoxAlgorithm->getDynamicBoxContext().markOutputAsReadyToSend(ui32OutputIndex, ui64StartTime, ui64EndTime);
		}

		OpenViBE::boolean encodeEnd(OpenViBE::uint32 ui32OutputIndex, OpenViBE::uint64 ui64StartTime, OpenViBE::uint64 ui64EndTime)
		{
			this->setOutputChunk(m_pBoxAlgorithm->getDynamicBoxContext().getOutputChunk(ui32OutputIndex));
			if(!this->encodeEnd()) return false;
			return m_pBoxAlgorithm->getDynamicBoxContext().markOutputAsReadyToSend(ui32OutputIndex, ui64StartTime, ui64EndTime);
		}

		virtual OpenViBE::boolean uninitialize(void) { return false;}
		virtual OpenViBE::boolean initialize(void) { return false;}

	};
};

#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#endif //__OpenViBEToolkit_TEncoder_H__
