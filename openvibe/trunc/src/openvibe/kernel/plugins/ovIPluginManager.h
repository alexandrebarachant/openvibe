#ifndef __OpenViBE_Kernel_Plugins_IPluginManager_H__
#define __OpenViBE_Kernel_Plugins_IPluginManager_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class IPluginObject;
		class IPluginObjectDesc;

		class IBoxAlgorithm;
		class IBoxAlgorithmDesc;

		class IAlgorithm;
		class IAlgorithmDesc;
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

			virtual OpenViBE::boolean canCreatePluginObject(
				const OpenViBE::CIdentifier& rClassIdentifier)=0;
			virtual const OpenViBE::Plugins::IPluginObjectDesc* getPluginObjectDescCreating(
				const OpenViBE::CIdentifier& rClassIdentifier) const=0;
			virtual OpenViBE::Plugins::IPluginObject* createPluginObject(
				const OpenViBE::CIdentifier& rClassIdentifier)=0;
			virtual OpenViBE::boolean releasePluginObject(
				OpenViBE::Plugins::IPluginObject* pPluginObject)=0;

			/**
			 */

			virtual OpenViBE::Plugins::IAlgorithm* createAlgorithm(
				const OpenViBE::CIdentifier& rClassIdentifier,
				const OpenViBE::Plugins::IAlgorithmDesc** ppAlgorithmDesc)=0;
			virtual OpenViBE::Plugins::IBoxAlgorithm* createBoxAlgorithm(
				const OpenViBE::CIdentifier& rClassIdentifier,
				const OpenViBE::Plugins::IBoxAlgorithmDesc** ppBoxAlgorithmDesc)=0;

			/**
			 */

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Plugins_PluginManager)

		};
	};
};

#endif // __OpenViBE_Kernel_Plugins_IPluginManager_H__
