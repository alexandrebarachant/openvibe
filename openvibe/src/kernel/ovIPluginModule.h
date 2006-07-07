#ifndef __OpenViBE_Kernel_IPluginModule_H__
#define __OpenViBE_Kernel_IPluginModule_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class IPluginObjectDesc;
	}

	namespace Kernel
	{
		class IPluginModule : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::boolean load(
				const std::string& sName)=0;
			virtual OpenViBE::boolean unload(void)=0;
			virtual OpenViBE::boolean getPluginObjectDescription(
				OpenViBE::uint32 ui32Index,
				OpenViBE::Plugins::IPluginObjectDesc*& rpPluginObjectDescription)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassIdentifier_PluginModule)
		};
	};
};

#endif // __OpenViBE_Kernel_IPluginModule_H__
