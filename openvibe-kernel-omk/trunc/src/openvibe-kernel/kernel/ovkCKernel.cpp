#include "ovkCKernel.h"

#include <string>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;

CKernel::CKernel(void)
	:m_oContext(*this)
{
}

void CKernel::release(void)
{
	delete this;
}
