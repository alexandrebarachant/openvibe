#ifndef __OpenViBE_Kernel_IScenarioExporterContext_H__
#define __OpenViBE_Kernel_IScenarioExporterContext_H__

#include "ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IScenarioExporterContext : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual const OpenViBE::CString getFileName(void) const=0;
			virtual const OpenViBE::Kernel::IScenario& getScenario(void) const=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_ScenarioExporterContext);

		};
	};
};

#endif // __OpenViBE_Kernel_IScenarioExporterContext_H__
