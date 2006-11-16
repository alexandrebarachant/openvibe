#ifndef __OpenViBE_Plugins_IScenarioExporterContext_H__
#define __OpenViBE_Plugins_IScenarioExporterContext_H__

#include "ovIPluginObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class OV_API IScenarioExporterContext : virtual public OpenViBE::Plugins::IPluginObjectContext
		{
		public:

			virtual const OpenViBE::CString getFileName(void) const=0;
			virtual const OpenViBE::Kernel::IScenario& getScenario(void) const=0;

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectContext, OV_ClassId_Plugins_ScenarioExporterContext);

		};
	};
};

#endif // __OpenViBE_Plugins_IScenarioExporterContext_H__
