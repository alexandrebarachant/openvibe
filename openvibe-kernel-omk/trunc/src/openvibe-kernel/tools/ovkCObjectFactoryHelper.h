#ifndef __OpenViBEKernel_Tools_CKernelFactoryHelper_H__
#define __OpenViBEKernel_Tools_CKernelFactoryHelper_H__

#include <openvibe/ov_all.h>

namespace OpenViBE
{
	namespace Tools
	{
		class CObjectFactoryHelper
		{
		public:

			CObjectFactoryHelper(OpenViBE::Kernel::IObjectFactory& rObjectFactory)
				:m_rObjectFactory(rObjectFactory)
			{
			}

			template <class T> T createObject(const OpenViBE::CIdentifier& rClassIdentifier)
			{
				OpenViBE::IObject* l_pObject=m_rObjectFactory.createObject(rClassIdentifier);
				T l_tResult=dynamic_cast<T>(l_pObject);
				if(l_pObject && !l_tResult)
				{
					m_rObjectFactory.releaseObject(l_pObject);
				}
				return l_tResult;
			}

			template <class T> T createObject(const OpenViBE::CIdentifier& rClassIdentifier, OpenViBE::IObject& rUserData)
			{
				OpenViBE::IObject* l_pObject=m_rObjectFactory.createObject(rClassIdentifier, rUserData);
				T l_tResult=dynamic_cast<T>(l_pObject);
				if(l_pObject && !l_tResult)
				{
					m_rObjectFactory.releaseObject(l_pObject);
				}
				return l_tResult;
			}

			template <class T> T createObject(const OpenViBE::CIdentifier& rClassIdentifier, const OpenViBE::IObject& rUserData)
			{
				OpenViBE::IObject* l_pObject=m_rObjectFactory.createObject(rClassIdentifier, rUserData);
				T l_tResult=dynamic_cast<T>(l_pObject);
				if(l_pObject && !l_tResult)
				{
					m_rObjectFactory.releaseObject(l_pObject);
				}
				return l_tResult;
			}

			template <class T> OpenViBE::boolean releaseObject(T tObject)
			{
				return m_rObjectFactory.releaseObject(tObject);
			}

		protected:

			OpenViBE::Kernel::IObjectFactory& m_rObjectFactory;

		private:

			CObjectFactoryHelper(void);
		};
	};
};

#endif // __OpenViBEKernel_Tools_CKernelFactoryHelper_H__
