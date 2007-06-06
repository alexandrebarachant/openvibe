#include "ovkCScopeTester.h"

using namespace OpenViBE;
using namespace OpenViBE::Tools;
using namespace std;

CScopeTester::CScopeTester(void)
{
	std::cout << "## CScopeTester [" << m_sPrefix << "] enter\n";
}

CScopeTester::CScopeTester(const std::string& sPrefix)
	:m_sPrefix(sPrefix)
{
	std::cout << "## CScopeTester [" << m_sPrefix << "] enter\n";
}

CScopeTester::~CScopeTester(void)
{
	std::cout << "## CScopeTester [" << m_sPrefix << "] leave\n";
}
