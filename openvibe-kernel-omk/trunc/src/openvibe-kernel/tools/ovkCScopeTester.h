#ifndef __OpenViBEKernel_Tools_CScopeTester_H__
#define __OpenViBEKernel_Tools_CScopeTester_H__

#include "../ovk_base.h"

#include <openvibe/ov_all.h>

#include <iostream>
#include <string>

namespace OpenViBE
{
	namespace Tools
	{
		class CScopeTester : virtual OpenViBE::IObject
		{
		public:

			CScopeTester(void);
			CScopeTester(const std::string& sPrefix);
			virtual ~CScopeTester(void);

			_IsDerivedFromClass_Final_(IObject, OVK_ClassId_Tools_ScopeTester);

		protected:

			std::string m_sPrefix;
		};
	};
};

#endif // __OpenViBEKernel_Tools_CScopeTester_H__
