#ifndef __OpenViBE_Kernel_IPluginManager_H__
#define __OpenViBE_Kernel_IPluginManager_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class IPluginObject;
		class IPluginObjectDesc;
	};

	namespace Kernel
	{
		class IPluginModule;

		class OV_API IPluginManager : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			class OV_API IPluginObjectDescEnum
			{
			public:
				virtual ~IPluginObjectDescEnum(void) { }
				virtual OpenViBE::boolean callback(
					const OpenViBE::Kernel::IPluginModule& rPluginModule,
					const OpenViBE::Plugins::IPluginObjectDesc& rPluginObjectDesc)=0;
			};

			virtual OpenViBE::boolean addPluginsFromFiles(
				const OpenViBE::CString& rFileNameWildCard)=0;

			virtual OpenViBE::boolean enumeratePluginObjectDesc(
				OpenViBE::Kernel::IPluginManager::IPluginObjectDescEnum& rCallback) const=0;
			virtual OpenViBE::boolean enumeratePluginObjectDesc(
				OpenViBE::Kernel::IPluginManager::IPluginObjectDescEnum& rCallback,
				const OpenViBE::CIdentifier& rBaseClassIdentifier) const=0;

			virtual const OpenViBE::Plugins::IPluginObjectDesc* getPluginObjectDescCreating(
				const OpenViBE::CIdentifier& rClassIdentifier) const=0;
			virtual OpenViBE::Plugins::IPluginObject* createPluginObject(
				const OpenViBE::CIdentifier& rClassIdentifier)=0;
			virtual OpenViBE::boolean releasePluginObject(
				OpenViBE::Plugins::IPluginObject* pPluginObject)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_PluginManager)

		};
	};
};

#endif // __OpenViBE_Kernel_IPluginManager_H__
