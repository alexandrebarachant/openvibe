#ifndef __OpenViBE_Plugins_IScenarioImporter_H__
#define __OpenViBE_Plugins_IScenarioImporter_H__

#include "ovIPluginObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class OV_API IScenarioImporter : virtual public OpenViBE::Plugins::IPluginObject
		{
		public:

			virtual OpenViBE::boolean doImport(
				const OpenViBE::CString& rFileName,
				OpenViBE::Kernel::IScenario& rScenario)=0;

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObject, OV_ClassId_Plugins_ScenarioImporter);
		};
	};
};

#endif // __OpenViBE_Plugins_IScenarioImporter_H__
