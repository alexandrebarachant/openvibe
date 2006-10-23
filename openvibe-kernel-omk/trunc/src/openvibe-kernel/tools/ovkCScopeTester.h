#ifndef __OpenViBEKernel_Tools_CScopeTester_H__
#define __OpenViBEKernel_Tools_CScopeTester_H__

#include <iostream>
#include <string>

namespace OpenViBE
{
	namespace Tools
	{
		class CScopeTester
		{
		public:

			CScopeTester(void)
			{
				std::cout << "## CScopeTester [" << m_sPrefix << "] enter\n";
			}

			CScopeTester(const std::string& sPrefix)
				:m_sPrefix(sPrefix)
			{
				std::cout << "## CScopeTester [" << m_sPrefix << "] enter\n";
			}

			virtual ~CScopeTester(void)
			{
				std::cout << "## CScopeTester [" << m_sPrefix << "] leave\n";
			}

		protected:

			std::string m_sPrefix;
		};
	};
};

#endif // __OpenViBEKernel_Tools_CScopeTester_H__
