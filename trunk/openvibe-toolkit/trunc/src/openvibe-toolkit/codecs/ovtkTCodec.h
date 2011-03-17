#ifndef __OpenViBEToolkit_TCodec_H__
#define __OpenViBEToolkit_TCodec_H__

#include <openvibe/ov_all.h>

namespace OpenViBEToolkit
{
	template <class T>
	class TCodec
	{
	protected:
		// The Box Algorithm that actually uses this Codec, so we can get the context.
		T* m_pBoxAlgorithm;

		OpenViBE::Kernel::IAlgorithmProxy* m_pCodec;

		//// const is for input
		//OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > m_pInputMemoryBuffer;
		//OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > m_pOutputMemoryBuffer;

	public:
		TCodec()
			:m_pBoxAlgorithm(NULL)
		{
		}

		OpenViBE::boolean initialize(T& rBoxAlgorithm)
		{
			if(m_pBoxAlgorithm == NULL)
			{
				m_pBoxAlgorithm = &rBoxAlgorithm;
			}
			else
			{
				return false;
			}

			return initialize();
		}

		virtual OpenViBE::boolean uninitialize(void) { return false;}

		/*virtual void setInputChunk(const OpenViBE::IMemoryBuffer * pInputChunkMemoryBuffer)
		{
			m_pInputMemoryBuffer = pInputChunkMemoryBuffer;
		}
		virtual void setOutputChunk(OpenViBE::IMemoryBuffer * pOutputChunkMemoryBuffer)
		{
			m_pOutputMemoryBuffer = pOutputChunkMemoryBuffer;
		}
		virtual OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > getInputMemoryBuffer()
		{
			return m_pInputMemoryBuffer;
		}
		virtual OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > getOutputMemoryBuffer()
		{
			return m_pOutputMemoryBuffer;
		}*/

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
		//virtual OpenViBE::boolean decode(void) 
		//{
		//	return m_pCodec->process();
		//}
		
	protected:
		virtual OpenViBE::boolean initialize() { return false;}

	};
};


#endif //__OpenViBEToolkit_TCodec_H__