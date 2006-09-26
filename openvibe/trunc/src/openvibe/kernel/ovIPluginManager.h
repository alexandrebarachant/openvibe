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

			class IPluginObjectDescEnum
			{
			public:
				virtual ~IPluginObjectDescEnum(void) { }
				virtual OpenViBE::boolean callback(
					OpenViBE::Kernel::IPluginObjectDesc& rPluginObjectDesc)=0;
			};

			virtual OpenViBE::boolean addPluginsInDirectory(
				const OpenViBE::CString& rDirectoryName)=0;
			virtual OpenViBE::boolean addPluginsInFile(
				const OpenViBE::CString& rFileName)=0;

		_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassIdentifier_PluginManager)

		};
	};
};

#endif // __OpenViBE_Kernel_IPluginManager_H__
