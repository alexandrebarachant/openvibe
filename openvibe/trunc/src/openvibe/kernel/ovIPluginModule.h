#ifndef __OpenViBE_Kernel_IPluginModule_H__
#define __OpenViBE_Kernel_IPluginModule_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IPluginObjectDesc;

		class OV_API IPluginModule : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::boolean load(
				const OpenViBE::CString& sName,
				OpenViBE::CString* pError=NULL)=0;
			virtual OpenViBE::boolean unload(
				CString* pError=NULL)=0;

			virtual OpenViBE::boolean initialize(void)=0;
			virtual OpenViBE::boolean getPluginObjectDescription(
				OpenViBE::uint32 ui32Index,
				OpenViBE::Kernel::IPluginObjectDesc*& rpPluginObjectDescription)=0;
			virtual OpenViBE::boolean uninitialize(void)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_PluginModule)
		};
	};
};

#endif // __OpenViBE_Kernel_IPluginModule_H__
