#ifndef __OpenViBEToolkit_TCodec_H__
#define __OpenViBEToolkit_TCodec_H__

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#include "../ovtk_base.h"

namespace OpenViBEToolkit
{
	template <class T>
	class TCodec
	{
	protected:

		T* m_pBoxAlgorithm;

		OpenViBE::Kernel::IAlgorithmProxy* m_pCodec;

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

			return initializeImpl();
		}
		
		virtual OpenViBE::boolean uninitialize(void) = 0;

	protected:

		virtual OpenViBE::boolean initializeImpl() = 0;

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

		

	};
};

#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines

#endif //__OpenViBEToolkit_TCodec_H__
