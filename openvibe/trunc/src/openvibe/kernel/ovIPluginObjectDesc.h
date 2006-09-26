#ifndef __OpenViBE_Kernel_IPluginObjectDesc_H__
#define __OpenViBE_Kernel_IPluginObjectDesc_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IPluginObjectDesc : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::CIdentifier getCreatedClass(void)=0;
			// virtual OpenViBE::Kernel::IPluginObject* create(void)=0;

			virtual OpenViBE::CString getName(void)=0;
			virtual OpenViBE::CString getAuthorName(void)=0;
			virtual OpenViBE::CString getAuthorCompanyName(void)=0;
			virtual OpenViBE::CString getShortDescription(void)=0;
			virtual OpenViBE::CString getDetailedDescription(void)=0;
			virtual OpenViBE::CString getCategory(void)=0;
			virtual OpenViBE::CString getVersion(void)=0;

			// _IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassIdentifier_PluginObjectDesc)
		};
	};
};

#endif // __OpenViBE_Kernel_IPluginObjectDesc_H__
