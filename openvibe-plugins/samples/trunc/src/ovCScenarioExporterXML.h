#ifndef __SamplePlugin_CScenarioExporter_H__
#define __SamplePlugin_CScenarioExporter_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

#include <xml/IWriter.h>

#include <iostream>
#include <fstream>
#include <string.h>

#define ClassIdentifier_SampleScenarioExporterXMLDesc OpenViBE::CIdentifier(0x5B2B5C67, 0x78EA7539)
#define ClassIdentifier_SampleScenarioExporterXML     OpenViBE::CIdentifier(0x77075B3B, 0x3D632492)

namespace OpenViBESamples
{
	class CScenarioExporterXML : virtual public OpenViBE::Plugins::IScenarioExporter
	{
	public:

		class CScenarioExporterXMLWriterCB : virtual public XML::IWriterCallBack
		{
		public:
			CScenarioExporterXMLWriterCB(const OpenViBE::CString& sFileName) : m_oFile(sFileName) { }
			virtual ~CScenarioExporterXMLWriterCB(void) { m_oFile.close(); }
			virtual void write(const char* sString) { m_oFile << sString; }
		protected:
			std::ofstream m_oFile;
		};

		class CSenarioExporterXMLBoxEnumCB : virtual public OpenViBE::Kernel::IScenario::IBoxEnum
		{
		public:
			CSenarioExporterXMLBoxEnumCB(XML::IWriter* pWriter)
				:m_pWriter(pWriter)
			{
			}

			virtual OpenViBE::boolean callback(const OpenViBE::Kernel::IScenario& rScenario, const OpenViBE::Kernel::IBox& rBox)
			{
				m_pWriter->openChild("Box");
				 m_pWriter->openChild("Identifier");
				  m_pWriter->setChildData(rBox.getIdentifier().toString());
				 m_pWriter->closeChild();
				 m_pWriter->openChild("AlgorithmClassIdentifier");
				  m_pWriter->setChildData(rBox.getAlgorithmClassIdentifier().toString());
				 m_pWriter->closeChild();
#if 0
				 m_pWriter->openChild("BehaviorClassIdentifier");
				  m_pWriter->setChildData(rBox.getBehaviorClassIdentifier().toString());
				 m_pWriter->closeChild();
#endif

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

				m_pWriter->closeChild();

				return true;
			}

		protected:

			XML::IWriter* m_pWriter;
		};

		class CSenarioExporterXMLLinkEnumCB : virtual public OpenViBE::Kernel::IScenario::ILinkEnum
		{
		public:
			CSenarioExporterXMLLinkEnumCB(XML::IWriter* pWriter)
				:m_pWriter(pWriter)
			{
			}

			virtual OpenViBE::boolean callback(const OpenViBE::Kernel::IScenario& rScenario, const OpenViBE::Kernel::ILink& rLink)
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
				m_pWriter->closeChild();

				return true;
			}

		protected:

			XML::IWriter* m_pWriter;
		};

		virtual void release(void)
		{
			delete this;
		}
		virtual OpenViBE::boolean doExport(const OpenViBE::CString& rFileName, const OpenViBE::Kernel::IScenario& rScenario)
		{
			CScenarioExporterXMLWriterCB cb(rFileName);
			XML::IWriter* l_pWriter=XML::createWriter(cb);
			if(!l_pWriter)
			{
				return false;
			}

			CSenarioExporterXMLBoxEnumCB bcb(l_pWriter);
			CSenarioExporterXMLLinkEnumCB lcb(l_pWriter);

			l_pWriter->openChild("OpenViBE");
			 l_pWriter->openChild("Scenario");
			  l_pWriter->openChild("Boxes");

				rScenario.enumerateBoxes(bcb);

			  l_pWriter->closeChild();
			  l_pWriter->openChild("Links");

				rScenario.enumerateLinks(lcb);

			  l_pWriter->closeChild();
			 l_pWriter->closeChild();
			l_pWriter->closeChild();

			l_pWriter->release();
			return true;
		}

		_IsDerivedFromClass_Final_(OpenViBE::Plugins::IScenarioExporter, ClassIdentifier_SampleScenarioExporterXML)
	};

	class CScenarioExporterXMLDesc : virtual public OpenViBE::Plugins::IScenarioExporterDesc
	{
	public:
		virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("sample behavior"); }
		virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
		virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
		virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("sample behavior"); }
		virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("A sample XML scenario exporter"); }
		virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
		virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

		virtual void release(void)
		{
		}
		virtual OpenViBE::CIdentifier getCreatedClass(void) const
		{
			return ClassIdentifier_SampleScenarioExporterXML;
		}
		virtual OpenViBE::Plugins::IPluginObject* create(void)
		{
			return new OpenViBESamples::CScenarioExporterXML();
		}
		virtual OpenViBE::CString getFileExtension(void) const
		{
			return OpenViBE::CString("xml;XML");
		}

		_IsDerivedFromClass_Final_(OpenViBE::Plugins::IScenarioExporterDesc, ClassIdentifier_SampleScenarioExporterXMLDesc)
	};

};

#endif // __SamplePlugin_CScenarioExporter_H__
