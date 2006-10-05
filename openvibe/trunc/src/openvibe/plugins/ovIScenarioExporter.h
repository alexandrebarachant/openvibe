#ifndef __OpenViBE_Plugins_IScenarioExporter_H__
#define __OpenViBE_Plugins_IScenarioExporter_H__

#include "ovIPluginObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class OV_API IScenarioExporter : virtual public OpenViBE::Plugins::IPluginObject
		{
		public:

			virtual OpenViBE::boolean doExport(
				const OpenViBE::CString& rFileName,
				const OpenViBE::Kernel::IScenario& rScenario)=0;

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObject, OV_ClassId_Plugins_ScenarioExporter);
		};
	};
};

#endif // __OpenViBE_Plugins_IScenarioExporter_H__
