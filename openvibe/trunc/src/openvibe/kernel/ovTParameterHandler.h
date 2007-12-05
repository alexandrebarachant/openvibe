#ifndef __OpenViBE_Kernel_TParameterHandler_H__
#define __OpenViBE_Kernel_TParameterHandler_H__

#include "ovIParameter.h"
#include "ovIConfigurable.h"

namespace OpenViBE
{
	namespace Kernel
	{
		template <typename T>
		class TParameterHandler
		{
		public:

			TParameterHandler(void)
				:m_pParameter(NULL)
			{
			}

			TParameterHandler(OpenViBE::Kernel::IParameter* pParameter)
				:m_pParameter(pParameter)
			{
			}

			~TParameterHandler(void)
			{
			}

			OpenViBE::boolean initialize(OpenViBE::Kernel::IParameter* pParameter)
			{
				if(m_pParameter)
				{
					return false;
				}

				m_pParameter=pParameter;
				return true;
			}

			OpenViBE::boolean uninitialize(void)
			{
				if(!m_pParameter)
				{
					return false;
				}

				m_pParameter=NULL;
				return true;
			}

			OpenViBE::boolean exists(void) const
			{
				return m_pParameter!=NULL;
			}

			operator T (void) const
			{
				if(!m_pParameter)
				{
					throw;
				}
				T t;
				m_pParameter->getValue(&t);
				return t;
			}

			T operator -> (void) const
			{
				if(!m_pParameter)
				{
					throw;
				}
				T t;
				m_pParameter->getValue(&t);
				return t;
			}

			TParameterHandler<T>& operator=(const T& t)
			{
				if(!m_pParameter)
				{
					throw;
				}
				m_pParameter->setValue(&t);
				return *this;
			}

		protected:

			OpenViBE::Kernel::IParameter* m_pParameter;
		};
	};
};

#endif // __OpenViBE_Kernel_TParameterHandler_H__
