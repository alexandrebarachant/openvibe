#ifndef __SamplePlugin_CScenarioImporter_H__
#define __SamplePlugin_CScenarioImporter_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

#include <xml/IReader.h>

#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <string>

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CScenarioImporterXML
			: virtual public OpenViBE::Plugins::IScenarioImporter
			, virtual public XML::IReaderCallBack
		{
		public:

			#define _default_and_copy_constructor_1_(c,m1)                   c(void) { } c(const c& r) : m1(r.m1) { }
			#define _default_and_copy_constructor_2_(c,m1,m2)                c(void) { } c(const c& r) : m1(r.m1), m2(r.m2) { }
			#define _default_and_copy_constructor_3_(c,m1,m2,m3)             c(void) { } c(const c& r) : m1(r.m1), m2(r.m2), m3(r.m3) { }
			#define _default_and_copy_constructor_4_(c,m1,m2,m3,m4)          c(void) { } c(const c& r) : m1(r.m1), m2(r.m2), m3(r.m3), m4(r.m4) { }
			#define _default_and_copy_constructor_5_(c,m1,m2,m3,m4,m5)       c(void) { } c(const c& r) : m1(r.m1), m2(r.m2), m3(r.m3), m4(r.m4), m5(r.m5) { }
			#define _default_and_copy_constructor_6_(c,m1,m2,m3,m4,m5,m6)    c(void) { } c(const c& r) : m1(r.m1), m2(r.m2), m3(r.m3), m4(r.m4), m5(r.m5), m6(r.m6) { }
			#define _default_and_copy_constructor_7_(c,m1,m2,m3,m4,m5,m6,m7) c(void) { } c(const c& r) : m1(r.m1), m2(r.m2), m3(r.m3), m4(r.m4), m5(r.m5), m6(r.m6), m7(r.m7) { }

			typedef struct _SInput
			{
				_default_and_copy_constructor_2_(_SInput, m_sTypeIdentifier, m_sName);
				std::string m_sTypeIdentifier;
				std::string m_sName;
			} SInput;
			typedef struct _SOutput
			{
				_default_and_copy_constructor_2_(_SOutput, m_sTypeIdentifier, m_sName);
				std::string m_sTypeIdentifier;
				std::string m_sName;
			} SOutput;
			typedef struct _SSetting
			{
				_default_and_copy_constructor_4_(_SSetting, m_sTypeIdentifier, m_sName, m_sDefaultValue, m_sValue);
				std::string m_sTypeIdentifier;
				std::string m_sName;
				std::string m_sDefaultValue;
				std::string m_sValue;
			} SSetting;
			typedef struct _SAttribute
			{
				_default_and_copy_constructor_2_(_SAttribute, m_sIdentifier, m_sValue);
				std::string m_sIdentifier;
				std::string m_sValue;
			} SAttribute;
			typedef struct _SBox
			{
				_default_and_copy_constructor_7_(_SBox, m_sIdentifier, m_sAlgorithmClassIdentifier, m_sName, m_vInput, m_vOutput, m_vSetting, m_vAttribute);
				std::string m_sIdentifier;
				std::string m_sAlgorithmClassIdentifier;
				std::string m_sName;
				std::vector<SInput> m_vInput;
				std::vector<SOutput> m_vOutput;
				std::vector<SSetting> m_vSetting;
				std::vector<SAttribute> m_vAttribute;
			} SBox;
			typedef struct _SLinkSource
			{
				_default_and_copy_constructor_2_(_SLinkSource, m_sBoxIdentifier, m_sBoxOutputIndex);
				std::string m_sBoxIdentifier;
				std::string m_sBoxOutputIndex;
			} SLinkSource;
			typedef struct _SLinkTarget
			{
				_default_and_copy_constructor_2_(_SLinkTarget, m_sBoxIdentifier, m_sBoxInputIndex);
				std::string m_sBoxIdentifier;
				std::string m_sBoxInputIndex;
			} SLinkTarget;
			typedef struct _SLink
			{
				_default_and_copy_constructor_4_(_SLink, m_sIdentifier, m_oLinkSource, m_oLinkTarget, m_vAttribute);
				std::string m_sIdentifier;
				SLinkSource m_oLinkSource;
				SLinkTarget m_oLinkTarget;
				std::vector<SAttribute> m_vAttribute;
			} SLink;
			typedef struct _SScenario
			{
				_default_and_copy_constructor_3_(_SScenario, m_vBox, m_vLink, m_vAttribute);
				std::vector<SBox> m_vBox;
				std::vector<SLink> m_vLink;
				std::vector<SAttribute> m_vAttribute;
			} SScenario;
			enum
			{
				Status_ParsingNothing,
				Status_ParsingScenario,
				Status_ParsingScenarioAttribute,
				Status_ParsingBox,
				Status_ParsingBoxInput,
				Status_ParsingBoxOutput,
				Status_ParsingBoxSetting,
				Status_ParsingBoxAttribute,
				Status_ParsingLink,
				Status_ParsingLinkSource,
				Status_ParsingLinkTarget,
				Status_ParsingLinkAttribute,
			};

			#undef _default_and_copy_constructor_1_
			#undef _default_and_copy_constructor_2_
			#undef _default_and_copy_constructor_3_
			#undef _default_and_copy_constructor_4_
			#undef _default_and_copy_constructor_5_
			#undef _default_and_copy_constructor_6_
			#undef _default_and_copy_constructor_7_

			class _AutoCaster_
			{
			public:
				_AutoCaster_(const std::string& sValue) : m_sValue(sValue) { }
				operator OpenViBE::CString (void) { return OpenViBE::CString(m_sValue.c_str()); }
				operator OpenViBE::CIdentifier (void) { OpenViBE::CIdentifier l_oResult; l_oResult.fromString(m_sValue.c_str()); return l_oResult; }
				operator OpenViBE::uint32 (void) { return atoi(m_sValue.c_str()); }
			protected:
				const std::string& m_sValue;
			};

			CScenarioImporterXML(void)
				:m_ui32Status(Status_ParsingNothing)
			{
			}

			virtual void openChild(const char* sName, const char** sAttributeName, const char** sAttributeValue, XML::uint64 ui64AttributeCount)
			{
				m_vNodes.push(sName);

				std::string& l_sTop=m_vNodes.top();
				if(false) { }
				else if(l_sTop=="OpenViBE-Scenario" && m_ui32Status==Status_ParsingNothing)  { m_ui32Status=Status_ParsingScenario; }
				else if(l_sTop=="Box"               && m_ui32Status==Status_ParsingScenario) { m_ui32Status=Status_ParsingBox;               m_oScenario.m_vBox.push_back(SBox()); }
				else if(l_sTop=="Input"             && m_ui32Status==Status_ParsingBox)      { m_ui32Status=Status_ParsingBoxInput;          m_oScenario.m_vBox.back().m_vInput.push_back(SInput());  }
				else if(l_sTop=="Output"            && m_ui32Status==Status_ParsingBox)      { m_ui32Status=Status_ParsingBoxOutput;         m_oScenario.m_vBox.back().m_vOutput.push_back(SOutput()); }
				else if(l_sTop=="Setting"           && m_ui32Status==Status_ParsingBox)      { m_ui32Status=Status_ParsingBoxSetting;        m_oScenario.m_vBox.back().m_vSetting.push_back(SSetting()); }
				else if(l_sTop=="Attribute"         && m_ui32Status==Status_ParsingBox)      { m_ui32Status=Status_ParsingBoxAttribute;      m_oScenario.m_vBox.back().m_vAttribute.push_back(SAttribute()); }
				else if(l_sTop=="Link"              && m_ui32Status==Status_ParsingScenario) { m_ui32Status=Status_ParsingLink;              m_oScenario.m_vLink.push_back(SLink()); }
				else if(l_sTop=="Source"            && m_ui32Status==Status_ParsingLink)     { m_ui32Status=Status_ParsingLinkSource; }
				else if(l_sTop=="Target"            && m_ui32Status==Status_ParsingLink)     { m_ui32Status=Status_ParsingLinkTarget; }
				else if(l_sTop=="Attribute"         && m_ui32Status==Status_ParsingLink)     { m_ui32Status=Status_ParsingLinkAttribute;     m_oScenario.m_vLink.back().m_vAttribute.push_back(SAttribute()); }
				else if(l_sTop=="Attribute"         && m_ui32Status==Status_ParsingScenario) { m_ui32Status=Status_ParsingScenarioAttribute; m_oScenario.m_vAttribute.push_back(SAttribute()); }

				// std::cout << std::string(m_vNodes.size(), '\t') << m_ui32Status << std::endl;
			}

			virtual void processChildData(const char* sData)
			{
				std::string& l_sTop=m_vNodes.top();
				switch(m_ui32Status)
				{
					case Status_ParsingBox:
						if(l_sTop=="Identifier")               m_oScenario.m_vBox.back().m_sIdentifier+=sData;
						if(l_sTop=="AlgorithmClassIdentifier") m_oScenario.m_vBox.back().m_sAlgorithmClassIdentifier+=sData;
						if(l_sTop=="Name")                     m_oScenario.m_vBox.back().m_sName+=sData;
						break;
					case Status_ParsingBoxInput:
						if(l_sTop=="TypeIdentifier")           m_oScenario.m_vBox.back().m_vInput.back().m_sTypeIdentifier+=sData;
						if(l_sTop=="Name")                     m_oScenario.m_vBox.back().m_vInput.back().m_sName+=sData;
						break;
					case Status_ParsingBoxOutput:
						if(l_sTop=="TypeIdentifier")           m_oScenario.m_vBox.back().m_vOutput.back().m_sTypeIdentifier+=sData;
						if(l_sTop=="Name")                     m_oScenario.m_vBox.back().m_vOutput.back().m_sName+=sData;
						break;
					case Status_ParsingBoxSetting:
						if(l_sTop=="TypeIdentifier")           m_oScenario.m_vBox.back().m_vSetting.back().m_sTypeIdentifier+=sData;
						if(l_sTop=="Name")                     m_oScenario.m_vBox.back().m_vSetting.back().m_sName+=sData;
						if(l_sTop=="DefaultValue")             m_oScenario.m_vBox.back().m_vSetting.back().m_sDefaultValue+=sData;
						if(l_sTop=="Value")                    m_oScenario.m_vBox.back().m_vSetting.back().m_sValue+=sData;
						break;
					case Status_ParsingBoxAttribute:
						if(l_sTop=="Identifier")               m_oScenario.m_vBox.back().m_vAttribute.back().m_sIdentifier+=sData;
						if(l_sTop=="Value")                    m_oScenario.m_vBox.back().m_vAttribute.back().m_sValue+=sData;
						break;

					case Status_ParsingLink:
						if(l_sTop=="Identifier")               m_oScenario.m_vLink.back().m_sIdentifier+=sData;
						break;
					case Status_ParsingLinkSource:
						if(l_sTop=="BoxIdentifier")           m_oScenario.m_vLink.back().m_oLinkSource.m_sBoxIdentifier+=sData;
						if(l_sTop=="BoxOutputIndex")          m_oScenario.m_vLink.back().m_oLinkSource.m_sBoxOutputIndex+=sData;
						break;
					case Status_ParsingLinkTarget:
						if(l_sTop=="BoxIdentifier")           m_oScenario.m_vLink.back().m_oLinkTarget.m_sBoxIdentifier+=sData;
						if(l_sTop=="BoxInputIndex")           m_oScenario.m_vLink.back().m_oLinkTarget.m_sBoxInputIndex+=sData;
						break;
					case Status_ParsingLinkAttribute:
						if(l_sTop=="Identifier")               m_oScenario.m_vLink.back().m_vAttribute.back().m_sIdentifier+=sData;
						if(l_sTop=="Value")                    m_oScenario.m_vLink.back().m_vAttribute.back().m_sValue+=sData;
						break;

					case Status_ParsingScenarioAttribute:
						if(l_sTop=="Identifier")               m_oScenario.m_vAttribute.back().m_sIdentifier+=sData;
						if(l_sTop=="Value")                    m_oScenario.m_vAttribute.back().m_sValue+=sData;
						break;
				}
			}

			virtual void closeChild(void)
			{
				std::string& l_sTop=m_vNodes.top();
				if(false) { }
				else if(l_sTop=="OpenViBE-Scenario" && m_ui32Status==Status_ParsingScenario)          m_ui32Status=Status_ParsingNothing;
				else if(l_sTop=="Box"               && m_ui32Status==Status_ParsingBox)               m_ui32Status=Status_ParsingScenario;
				else if(l_sTop=="Input"             && m_ui32Status==Status_ParsingBoxInput)          m_ui32Status=Status_ParsingBox;
				else if(l_sTop=="Output"            && m_ui32Status==Status_ParsingBoxOutput)         m_ui32Status=Status_ParsingBox;
				else if(l_sTop=="Setting"           && m_ui32Status==Status_ParsingBoxSetting)        m_ui32Status=Status_ParsingBox;
				else if(l_sTop=="Attribute"         && m_ui32Status==Status_ParsingBoxAttribute)      m_ui32Status=Status_ParsingBox;
				else if(l_sTop=="Link"              && m_ui32Status==Status_ParsingLink)              m_ui32Status=Status_ParsingScenario;
				else if(l_sTop=="Source"            && m_ui32Status==Status_ParsingLinkSource)        m_ui32Status=Status_ParsingLink;
				else if(l_sTop=="Target"            && m_ui32Status==Status_ParsingLinkTarget)        m_ui32Status=Status_ParsingLink;
				else if(l_sTop=="Attribute"         && m_ui32Status==Status_ParsingLinkAttribute)     m_ui32Status=Status_ParsingLink;
				else if(l_sTop=="Attribute"         && m_ui32Status==Status_ParsingScenarioAttribute) m_ui32Status=Status_ParsingScenario;

				m_vNodes.pop();
			}

			virtual void release(void)
			{
				delete this;
			}

			virtual OpenViBE::boolean doImport(const OpenViBE::Plugins::IScenarioImporterContext& rScenarioImporterContext)
			{
				XML::IReader* l_pReader=XML::createReader(*this);
				if(!l_pReader)
				{
					return false;
				}

				OpenViBE::Kernel::IScenario& l_rScenario=rScenarioImporterContext.getScenario();

				// 1. Open scenario file
				// 2. Loop until end of file, reading it
				//    and sending what is read to the XML parser
				// 3. Close the scenario file
				std::ifstream l_oFile(rScenarioImporterContext.getFileName());
				char l_sBuffer[1024];
				size_t l_iBufferLen=0;
				size_t l_iFileLen;
				l_oFile.seekg(0, std::ios::end);
				l_iFileLen=l_oFile.tellg();
				l_oFile.seekg(0, std::ios::beg);
				while(l_iFileLen)
				{
					l_iBufferLen=(l_iFileLen>sizeof(l_sBuffer)?sizeof(l_sBuffer):l_iFileLen);
					l_oFile.read(l_sBuffer, l_iBufferLen);
					l_iFileLen-=l_iBufferLen;
					l_pReader->processData(l_sBuffer, l_iBufferLen);
				}
				l_oFile.close();

				// Now build the scenario according to what has been loaded
				std::vector<SBox>::iterator b;
				std::vector<SInput>::iterator i;
				std::vector<SOutput>::iterator o;
				std::vector<SSetting>::iterator s;
				std::vector<SLink>::iterator l;
				std::vector<SAttribute>::iterator a;
				for(b=m_oScenario.m_vBox.begin(); b!=m_oScenario.m_vBox.end(); b++)
				{
					OpenViBE::Kernel::IBox* l_pBox=NULL;
					OpenViBE::CIdentifier l_oNewBoxIdentifier;
					OpenViBE::CIdentifier l_oBoxIdentifier;
					l_oBoxIdentifier.fromString(_AutoCaster_(b->m_sIdentifier));

					l_rScenario.addBox(l_oNewBoxIdentifier);
					l_pBox=l_rScenario.getBoxDetails(l_oNewBoxIdentifier);
					l_pBox->setName(_AutoCaster_(b->m_sName));
					l_pBox->setAlgorithmClassIdentifier(_AutoCaster_(b->m_sIdentifier));

					for(i=b->m_vInput.begin(); i!=b->m_vInput.end(); i++)
					{
						l_pBox->addInput(
							_AutoCaster_(i->m_sName),
							_AutoCaster_(i->m_sTypeIdentifier));
					}
					for(o=b->m_vOutput.begin(); o!=b->m_vOutput.end(); o++)
					{
						l_pBox->addOutput(
							_AutoCaster_(o->m_sName),
							_AutoCaster_(o->m_sTypeIdentifier));
					}
					for(s=b->m_vSetting.begin(); s!=b->m_vSetting.end(); s++)
					{
						l_pBox->addSetting(
							_AutoCaster_(s->m_sName),
							_AutoCaster_(s->m_sTypeIdentifier),
							_AutoCaster_(s->m_sDefaultValue));
						l_pBox->setSettingValue(
							l_pBox->getSettingCount()-1,
							_AutoCaster_(s->m_sValue));
					}
					for(a=b->m_vAttribute.begin(); a!=b->m_vAttribute.end(); a++)
					{
						l_pBox->addAttribute(
							_AutoCaster_(a->m_sIdentifier),
							_AutoCaster_(a->m_sValue));
					}
					m_vBoxIdMapping[l_oBoxIdentifier]=l_oNewBoxIdentifier;
				}
				for(l=m_oScenario.m_vLink.begin(); l!=m_oScenario.m_vLink.end(); l++)
				{
					OpenViBE::Kernel::ILink* l_pLink=NULL;
					OpenViBE::CIdentifier l_oNewLinkIdentifier;
					OpenViBE::CIdentifier l_oLinkIdentifier;
					l_oLinkIdentifier.fromString(_AutoCaster_(l->m_sIdentifier));

					l_rScenario.connect(
						m_vBoxIdMapping[_AutoCaster_(l->m_oLinkSource.m_sBoxIdentifier)],
						_AutoCaster_(l->m_oLinkSource.m_sBoxOutputIndex),
						m_vBoxIdMapping[_AutoCaster_(l->m_oLinkTarget.m_sBoxIdentifier)],
						_AutoCaster_(l->m_oLinkTarget.m_sBoxInputIndex),
						l_oNewLinkIdentifier);
					l_pLink=l_rScenario.getLinkDetails(l_oNewLinkIdentifier);
					for(a=l->m_vAttribute.begin(); a!=l->m_vAttribute.end(); a++)
					{
						l_pLink->addAttribute(
							_AutoCaster_(a->m_sIdentifier),
							_AutoCaster_(a->m_sValue));
					}
					m_vLinkIdMapping[l_oLinkIdentifier]=l_oNewLinkIdentifier;
				}
				for(a=m_oScenario.m_vAttribute.begin(); a!=m_oScenario.m_vAttribute.end(); a++)
				{
					l_rScenario.addAttribute(
						_AutoCaster_(a->m_sIdentifier),
						_AutoCaster_(a->m_sValue));
				}

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IScenarioImporter, OVP_ClassId_ScenarioImporterXML)

		protected:

			std::map<OpenViBE::CIdentifier, OpenViBE::CIdentifier> m_vBoxIdMapping;
			std::map<OpenViBE::CIdentifier, OpenViBE::CIdentifier> m_vLinkIdMapping;
			std::stack<std::string> m_vNodes;
			OpenViBE::uint32 m_ui32Status;
			SScenario m_oScenario;
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
