#ifndef __OpenViBE_Kernel_IPluginManager_H__
#define __OpenViBE_Kernel_IPluginManager_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IPluginObjectDesc;

		class OV_API IPluginManager : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			class OV_API IPluginObjectDescEnum
			{
			public:
				virtual ~IPluginObjectDescEnum(void) { }
				virtual OpenViBE::boolean callback(
					OpenViBE::Kernel::IPluginObjectDesc& rPluginObjectDesc)=0;
			};

			virtual OpenViBE::boolean addPluginsFromFiles(
				const OpenViBE::CString& rFileNameWildCard)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_PluginManager)

		};
	};
};

#endif // __OpenViBE_Kernel_IPluginManager_H__
