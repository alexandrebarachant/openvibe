#ifndef __SamplePlugin_CScenarioImporter_H__
#define __SamplePlugin_CScenarioImporter_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CScenarioImporterXML : virtual public OpenViBE::Plugins::IScenarioImporter
		{
		public:

			virtual void release(void)
			{
				delete this;
			}

			virtual OpenViBE::boolean doImport(const OpenViBE::Plugins::IScenarioImporterContext& rScenarioImporterContext)
			{
				return false;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IScenarioImporter, OVP_ClassId_ScenarioImporterXML)
		};

		class CScenarioImporterXMLDesc : virtual public OpenViBE::Plugins::IScenarioImporterDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("sample behavior"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("sample behavior"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("A sample XML scenario importer"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getFileExtension(void) const       { return OpenViBE::CString("xml;XML"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_ScenarioImporterXML; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CScenarioImporterXML(); }

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IScenarioImporterDesc, OVP_ClassId_ScenarioImporterXMLDesc)
		};
	};
};

#endif // __SamplePlugin_CScenarioImporter_H__
