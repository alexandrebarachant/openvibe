#include "kernel/ovkCKernel.h"

#include <openvibe/ov_all.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;

namespace OpenViBE
{
	namespace Kernel
	{
		class CKernelDesc : virtual public IKernelDesc
		{
		public:

			virtual void release(void) { }

			virtual IKernel* create(void)
			{
				return new CKernel();
			}

			virtual CString getName(void)                { return CString("OpenViBE Kernel Implementation [Based on OpenMASK]"); }
			virtual CString getAuthorName(void)          { return CString("Yann Renard"); }
			virtual CString getAuthorCompanyName(void)   { return CString("INRIA/IRISA"); }
			virtual CString getShortDescription(void)    { return CString("OpenViBE Kernel Implementation [Based on OpenMASK]"); }
			virtual CString getDetailedDescription(void) { return CString("OpenViBE Kernel Implementation [Based on OpenMASK]"); }
			virtual CString getVersion(void)             { return CString("0.1"); }

			_IsDerivedFromClass_Final_(IKernelDesc, OVK_ClassId_KernelDesc);
		};
	};
};

static CKernelDesc gst_oKernelDesc;

#include "system/Time.h"

extern "C"
{

OVK_API boolean onInitialize(void)
{
	return true;
}

OVK_API boolean onGetKernelDesc(IKernelDesc*& rpKernelDesc)
{
	rpKernelDesc=&gst_oKernelDesc;
	return true;
}

OVK_API boolean onUninitialize(void)
{
	return true;
}

};
