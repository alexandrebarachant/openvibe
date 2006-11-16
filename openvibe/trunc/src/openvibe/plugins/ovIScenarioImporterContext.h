#ifndef __OpenViBE_Plugins_IScenarioImporterContext_H__
#define __OpenViBE_Plugins_IScenarioImporterContext_H__

#include "ovIPluginObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class OV_API IScenarioImporterContext : virtual public OpenViBE::Plugins::IPluginObjectContext
		{
		public:

			virtual OpenViBE::CString getFileName(void) const=0;
			virtual OpenViBE::Kernel::IScenario& getScenario(void) const=0;

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectContext, OV_ClassId_Plugins_ScenarioImporterContext);

		};
	};
};

#endif // __OpenViBE_Plugins_IScenarioImporterContext_H__
