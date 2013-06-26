#ifndef __OpenViBE_Plugins_IServerExtension_H__
#define __OpenViBE_Plugins_IServerExtension_H__

#include "ovIPluginObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IServerExtensionContext;
		class IServerExtensionDynamicContext;
	}

	namespace Plugins
	{
		class OV_API IServerExtension : public OpenViBE::Plugins::IPluginObject
		{
		public:
			virtual OpenViBE::boolean initialize(
					OpenViBE::Kernel::IServerExtensionContext& rServerExtensionContext) = 0;

			virtual OpenViBE::boolean uninitialize(
					OpenViBE::Kernel::IServerExtensionContext& rServerExtensionContext) = 0;

			virtual OpenViBE::boolean createHook(
					OpenViBE::Kernel::IServerExtensionContext& rServerExtensionContext) = 0;

			virtual OpenViBE::boolean startHook(
					OpenViBE::Kernel::IServerExtensionContext& rServerExtensionContext) = 0;

			virtual OpenViBE::boolean stopHook(
					OpenViBE::Kernel::IServerExtensionContext& rServerExtensionContext) = 0;

			virtual OpenViBE::boolean loopHook(
					OpenViBE::Kernel::IServerExtensionContext& rServerExtensionContext,
					OpenViBE::Kernel::IServerExtensionDynamicContext& rServerExtensionDynamicContext) = 0;

			virtual OpenViBE::boolean acceptNewConnectionHook(
					OpenViBE::Kernel::IServerExtensionContext& rServerExtensionContext) = 0;

		};
	}
}

#endif // __OpenViBE_Plugins_IServerExtension_H__
