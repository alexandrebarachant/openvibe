#ifndef __SamplePlugin_CScenarioExporter_H__
#define __SamplePlugin_CScenarioExporter_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

#include <xml/IWriter.h>

#include <iostream>
#include <fstream>

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CScenarioExporterXML
			: virtual public OpenViBE::Plugins::IScenarioExporter
			, virtual public XML::IWriterCallBack
			, virtual public OpenViBE::Kernel::IAttributable::IAttributeEnum
			, virtual public OpenViBE::Kernel::IScenario::IBoxEnum
			, virtual public OpenViBE::Kernel::IScenario::ILinkEnum
		{
		public:

			CScenarioExporterXML(void)
				:m_pWriter(NULL)
			{
			}

			virtual void release(void)
			{
				delete this;
			}

			// IWriterCallBack callback !!!
			virtual void write(const char* sString)
			{
				m_oFile << sString;
			}

			// IAttributable::IAttributeEnum callback !!!
			virtual OpenViBE::boolean callback(const OpenViBE::Kernel::IAttributable& rAttributable, const OpenViBE::CIdentifier& rAttributeIdentifier, const OpenViBE::CString& rAttributeValue)
			{
				m_pWriter->openChild("Attribute");
				 m_pWriter->openChild("Identifier");
				  m_pWriter->setChildData(rAttributeIdentifier.toString());
				 m_pWriter->closeChild();
				 m_pWriter->openChild("Value");
				  m_pWriter->setChildData(rAttributeValue);
				 m_pWriter->closeChild();
				m_pWriter->closeChild();
				return true;
			}

			// IScenario::IBoxEnum callback !!!
			virtual OpenViBE::boolean callback(const OpenViBE::Kernel::IScenario& rScenario, OpenViBE::Kernel::IBox& rBox)
			{
				m_pWriter->openChild("Box");
				 m_pWriter->openChild("Identifier");
				  m_pWriter->setChildData(rBox.getIdentifier().toString());
				 m_pWriter->closeChild();
				 m_pWriter->openChild("Name");
				  m_pWriter->setChildData(rBox.getName());
				 m_pWriter->closeChild();
				 m_pWriter->openChild("AlgorithmClassIdentifier");
				  m_pWriter->setChildData(rBox.getAlgorithmClassIdentifier().toString());
				 m_pWriter->closeChild();

				if(rBox.getInputCount()!=0)
				{
					m_pWriter->openChild("Inputs");
					for(OpenViBE::uint32 i=0; i<rBox.getInputCount(); i++)
					{
						OpenViBE::CIdentifier l_oInputTypeIdentifier;
						OpenViBE::CString l_sInputName;
						rBox.getInputType(i, l_oInputTypeIdentifier);
						rBox.getInputName(i, l_sInputName);
						m_pWriter->openChild("Input");
						 m_pWriter->openChild("TypeIdentifier");
						  m_pWriter->setChildData(l_oInputTypeIdentifier.toString());
						 m_pWriter->closeChild();
						 m_pWriter->openChild("Name");
						  m_pWriter->setChildData(l_sInputName);
						 m_pWriter->closeChild();
						m_pWriter->closeChild();
					}
					m_pWriter->closeChild();
				}

				if(rBox.getOutputCount()!=0)
				{
					m_pWriter->openChild("Outputs");
					for(OpenViBE::uint32 i=0; i<rBox.getOutputCount(); i++)
					{
						OpenViBE::CIdentifier l_oOutputTypeIdentifier;
						OpenViBE::CString l_sOutputName;
						rBox.getOutputType(i, l_oOutputTypeIdentifier);
						rBox.getOutputName(i, l_sOutputName);
						m_pWriter->openChild("Output");
						 m_pWriter->openChild("TypeIdentifier");
						  m_pWriter->setChildData(l_oOutputTypeIdentifier.toString());
						 m_pWriter->closeChild();
						 m_pWriter->openChild("Name");
						  m_pWriter->setChildData(l_sOutputName);
						 m_pWriter->closeChild();
						m_pWriter->closeChild();
					}
					m_pWriter->closeChild();
				}

				if(rBox.getSettingCount()!=0)
				{
					m_pWriter->openChild("Settings");
					for(OpenViBE::uint32 i=0; i<rBox.getSettingCount(); i++)
					{
						OpenViBE::CIdentifier l_oSettingTypeIdentifier;
						OpenViBE::CString l_sSettingName;
						OpenViBE::CString l_sDefaultValue;
						OpenViBE::CString l_sValue;
						rBox.getSettingType(i, l_oSettingTypeIdentifier);
						rBox.getSettingName(i, l_sSettingName);
						rBox.getSettingDefaultValue(i, l_sDefaultValue);
						rBox.getSettingValue(i, l_sValue);
						m_pWriter->openChild("Setting");
						 m_pWriter->openChild("TypeIdentifier");
						  m_pWriter->setChildData(l_oSettingTypeIdentifier.toString());
						 m_pWriter->closeChild();
						 m_pWriter->openChild("Name");
						  m_pWriter->setChildData(l_sSettingName);
						 m_pWriter->closeChild();
						 m_pWriter->openChild("DefaultValue");
						  m_pWriter->setChildData(l_sDefaultValue);
						 m_pWriter->closeChild();
						 m_pWriter->openChild("Value");
						  m_pWriter->setChildData(l_sValue);
						 m_pWriter->closeChild();
						m_pWriter->closeChild();
					}
					m_pWriter->closeChild();
				}

				if(rBox.hasAttributes())
				{
					m_pWriter->openChild("Attributes");
					 rBox.enumerateAttributes(*this);
					m_pWriter->closeChild();
				}

				m_pWriter->closeChild();

				return true;
			}

			// IScenario::ILinkEnum callback !!!
			virtual OpenViBE::boolean callback(const OpenViBE::Kernel::IScenario& rScenario, OpenViBE::Kernel::ILink& rLink)
			{
				OpenViBE::CIdentifier l_oSourceBoxIdentifier;
				OpenViBE::CIdentifier l_oTargetBoxIdentifier;
				OpenViBE::uint32 l_ui32SourceBoxOutputIndex=0xffffffff;
				OpenViBE::uint32 l_ui32TargetBoxInputIndex=0xffffffff;

				rLink.getSource(l_oSourceBoxIdentifier, l_ui32SourceBoxOutputIndex);
				rLink.getTarget(l_oTargetBoxIdentifier, l_ui32TargetBoxInputIndex);

				char l_sSourceBoxOutputIndex[1024];
				char l_sTargetBoxInputIndex[1024];
				sprintf(l_sSourceBoxOutputIndex, "%i", l_ui32SourceBoxOutputIndex);
				sprintf(l_sTargetBoxInputIndex, "%i", l_ui32TargetBoxInputIndex);

				m_pWriter->openChild("Link");
				 m_pWriter->openChild("Identifier");
				  m_pWriter->setChildData(rLink.getIdentifier().toString());
				 m_pWriter->closeChild();
				 m_pWriter->openChild("Source");
				  m_pWriter->openChild("BoxIdentifier");
				   m_pWriter->setChildData(l_oSourceBoxIdentifier.toString());
				  m_pWriter->closeChild();
				  m_pWriter->openChild("BoxOutputIndex");
				   m_pWriter->setChildData(l_sSourceBoxOutputIndex);
				  m_pWriter->closeChild();
				 m_pWriter->closeChild();
				 m_pWriter->openChild("Target");
				  m_pWriter->openChild("BoxIdentifier");
				   m_pWriter->setChildData(l_oTargetBoxIdentifier.toString());
				  m_pWriter->closeChild();
				  m_pWriter->openChild("BoxInputIndex");
				   m_pWriter->setChildData(l_sTargetBoxInputIndex);
				  m_pWriter->closeChild();
				 m_pWriter->closeChild();

				if(rLink.hasAttributes())
				{
					m_pWriter->openChild("Attributes");
					 rLink.enumerateAttributes(*this);
					m_pWriter->closeChild();
				}

				m_pWriter->closeChild();

				return true;
			}

			virtual OpenViBE::boolean doExport(const OpenViBE::Plugins::IScenarioExporterContext& rScenarioExporterContext)
			{
				m_pWriter=XML::createWriter(*this);
				if(!m_pWriter)
				{
					return false;
				}

				const OpenViBE::Kernel::IScenario& l_rScenario=rScenarioExporterContext.getScenario();

				m_oFile.open(rScenarioExporterContext.getFileName());

				m_pWriter->openChild("OpenViBE-Scenario");
				 m_pWriter->openChild("Boxes");
				  l_rScenario.enumerateBoxes(*this);
				 m_pWriter->closeChild();
				 m_pWriter->openChild("Links");
				  l_rScenario.enumerateLinks(*this);
				 m_pWriter->closeChild();

				if(l_rScenario.hasAttributes())
				{
					m_pWriter->openChild("Attributes");
					 l_rScenario.enumerateAttributes(*this);
					m_pWriter->closeChild();
				}

				m_pWriter->closeChild();
				m_pWriter->release();
				m_pWriter=NULL;

				m_oFile.close();
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IScenarioExporter, OVP_ClassId_ScenarioExporterXML)

		protected:

			XML::IWriter* m_pWriter;
			std::ofstream m_oFile;
		};

		class CScenarioExporterXMLDesc : virtual public OpenViBE::Plugins::IScenarioExporterDesc
		{
		public:
			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("XML Scenario exporter"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("A sample XML scenario exporter"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This scenario exporter uses simple XML format to output the scenario"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getFileExtension(void) const       { return OpenViBE::CString("xml;XML"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_ScenarioExporterXML; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CScenarioExporterXML(); }

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IScenarioExporterDesc, OVP_ClassId_ScenarioExporterXMLDesc)
		};
	};
};

#endif // __SamplePlugin_CScenarioExporter_H__
