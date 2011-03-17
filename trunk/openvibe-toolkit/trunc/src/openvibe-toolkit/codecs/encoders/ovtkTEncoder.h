#ifndef __OpenViBEToolkit_TEncoder_H__
#define __OpenViBEToolkit_TEncoder_H__

#include <openvibe/ov_all.h>

namespace OpenViBEToolkit
{
	template <class T>
	class TEncoder : public TCodec < T >
	{
	protected:
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > m_pOutputMemoryBuffer;

	public:
		using TCodec::initialize;

		virtual OpenViBE::boolean uninitialize(void) { return false;}

		
		virtual void setOutputChunk(OpenViBE::IMemoryBuffer * pOutputChunkMemoryBuffer)
		{
			m_pOutputMemoryBuffer = pOutputChunkMemoryBuffer;
		}
		virtual OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > getOutputMemoryBuffer()
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

		virtual OpenViBE::boolean encodeHeader(void) { return false;}
		virtual OpenViBE::boolean encodeBuffer(void) { return false;}
		virtual OpenViBE::boolean encodeEnd(void) { return false;}

	protected:
		virtual OpenViBE::boolean initialize(void) { return false;}

	};
};


#endif //__OpenViBEToolkit_TEncoder_H__