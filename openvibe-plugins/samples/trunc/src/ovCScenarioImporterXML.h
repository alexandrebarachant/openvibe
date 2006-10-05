#ifndef __SamplePlugin_CScenarioImporter_H__
#define __SamplePlugin_CScenarioImporter_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

#define ClassIdentifier_SampleScenarioImporterDesc OpenViBE::CIdentifier(0x28DC180C, 0x4CCA7314)
#define ClassIdentifier_SampleScenarioImporter     OpenViBE::CIdentifier(0x440BF3AC, 0x2D960300)

namespace OpenViBESamples
{
	class CScenarioImporterXML : virtual public OpenViBE::Plugins::IScenarioImporter
	{
	public:
		virtual void release(void)
		{
			delete this;
		}
		virtual OpenViBE::boolean doImport(const OpenViBE::CString& rFileName, OpenViBE::Kernel::IScenario& rScenario)
		{
			return false;
		}
		_IsDerivedFromClass_Final_(OpenViBE::Plugins::IScenarioImporter, ClassIdentifier_SampleScenarioImporter)
	};

	class CScenarioImporterXMLDesc : virtual public OpenViBE::Plugins::IScenarioImporterDesc
	{
	public:
		virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("sample behavior"); }
		virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
		virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
		virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("sample behavior"); }
		virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("A sample XML scenario importer"); }
		virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
		virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

		virtual void release(void)
		{
		}
		virtual OpenViBE::CIdentifier getCreatedClass(void) const
		{
			return ClassIdentifier_SampleScenarioImporter;
		}
		virtual OpenViBE::Plugins::IPluginObject* create(void)
		{
			return new OpenViBESamples::CScenarioImporterXML();
		}
		virtual OpenViBE::CString getFileExtension(void) const
		{
			return OpenViBE::CString("xml;XML");
		}

		_IsDerivedFromClass_Final_(OpenViBE::Plugins::IScenarioImporterDesc, ClassIdentifier_SampleScenarioImporterDesc)
	};

};

#endif // __SamplePlugin_CScenarioImporter_H__
