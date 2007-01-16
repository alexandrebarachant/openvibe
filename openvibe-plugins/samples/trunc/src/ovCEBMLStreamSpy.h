#ifndef __SamplePlugin_CEBMLStreamSpy_H__
#define __SamplePlugin_CEBMLStreamSpy_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <ebml/IReader.h>

#include <iostream>
#include <stack>
#include <map>
#include <string>

#define ConsoleColor_FG_Highlight "\033[01m"
#define ConsoleColor_FG_Downlight "\033[00m"

#define ConsoleColor_FG_Black     "\033[30m"
#define ConsoleColor_FG_Red       "\033[31m"
#define ConsoleColor_FG_Green     "\033[32m"
#define ConsoleColor_FG_Yellow    "\033[33m"
#define ConsoleColor_FG_Blue      "\033[34m"
#define ConsoleColor_FG_Magenta   "\033[35m"
#define ConsoleColor_FG_Cyan      "\033[36m"
#define ConsoleColor_FG_White     "\033[37m"

#define ConsoleColor_BG_Black     "\033[40m"
#define ConsoleColor_BG_Red       "\033[41m"
#define ConsoleColor_BG_Green     "\033[42m"
#define ConsoleColor_BG_Yellow    "\033[43m"
#define ConsoleColor_BG_Blue      "\033[44m"
#define ConsoleColor_BG_Magenta   "\033[45m"
#define ConsoleColor_BG_Cyan      "\033[46m"
#define ConsoleColor_BG_White     "\033[47m"

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CEBMLStreamSpy : virtual public OpenViBE::Plugins::IBoxAlgorithm, virtual public EBML::IReaderCallBack
		{
		public:

			CEBMLStreamSpy(void)
				:m_pReader(NULL)
				,m_pReaderHelper(NULL)
			{
			}

			virtual void release(void)
			{
			}

			virtual OpenViBE::boolean initialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				m_pReader=EBML::createReader(*this);
				m_pReaderHelper=EBML::createReaderHelper();

				OpenViBE::CString l_sFileName;
				rContext.getStaticBoxContext()->getSettingValue(0, l_sFileName);

				std::ifstream l_oFile(l_sFileName);
				while(!l_oFile.eof())
				{
					EBML::uint64 l_ui64Identifier;
					std::string l_sName;
					std::string l_sType;

					l_oFile>>l_sName;
					l_oFile>>l_ui64Identifier;
					l_oFile>>l_sType;

					m_vName[l_ui64Identifier]=l_sName;
					m_vType[l_ui64Identifier]=l_sType;
				}

				return true;
			}

			virtual OpenViBE::boolean uninitialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				m_pReaderHelper->release();
				m_pReaderHelper=NULL;
				m_pReader->release();
				m_pReader=NULL;

				return true;
			}

			virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier)
			{
				std::map<EBML::CIdentifier, std::string>::iterator n;
				std::map<EBML::CIdentifier, std::string>::iterator t;
				n=m_vName.find(rIdentifier);
				t=m_vType.find(rIdentifier);
				if(n!=m_vName.end() && t!=m_vType.end())
				{
					return (t->second=="master");
				}
				return false;
			}

			virtual void openChild(const EBML::CIdentifier& rIdentifier)
			{
				std::map<EBML::CIdentifier, std::string>::iterator n;
				std::map<EBML::CIdentifier, std::string>::iterator t;
				n=m_vName.find(rIdentifier);
				t=m_vType.find(rIdentifier);

				std::cout << std::endl;
				for(size_t i=0; i<m_vNodes.size(); i++)
				{
					std::cout << "  ";
				}
				std::cout
					<< "Opened EBML node [id:"
					<< ConsoleColor_FG_Blue << (EBML::uint64)rIdentifier << ConsoleColor_FG_Black
					<< "]-[name:"
					<< ConsoleColor_FG_Blue << (n!=m_vName.end()?n->second:"unknown") << ConsoleColor_FG_Black
					<< "]";

				m_vNodes.push(rIdentifier);
			}

			virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
			{
				std::map<EBML::CIdentifier, std::string>::iterator n;
				std::map<EBML::CIdentifier, std::string>::iterator t;
				n=m_vName.find(m_vNodes.top());
				t=m_vType.find(m_vNodes.top());

				if(t!=m_vType.end())
				{
					if(t->second=="uinteger")
						std::cout
							<< "-[type:"
							<< ConsoleColor_FG_Blue << t->second << ConsoleColor_FG_Black
							<< "]-[value:"
							<< ConsoleColor_FG_Blue << m_pReaderHelper->getUIntegerFromChildData(pBuffer, ui64BufferSize) << ConsoleColor_FG_Black
							<< "]";
					else if(t->second=="integer")
						std::cout
							<< "-[type:"
							<< ConsoleColor_FG_Blue << t->second << ConsoleColor_FG_Black
							<< "]-[value:"
							<< ConsoleColor_FG_Blue << m_pReaderHelper->getSIntegerFromChildData(pBuffer, ui64BufferSize) << ConsoleColor_FG_Black
							<< "]";
					else if(t->second=="string")
						std::cout
							<< "-[type:"
							<< ConsoleColor_FG_Blue << t->second << ConsoleColor_FG_Black
							<< "]-[value:"
							<< ConsoleColor_FG_Blue << m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize) << ConsoleColor_FG_Black
							<< "]";
					else if(t->second=="binary")
						std::cout
							<< "-[type:"
							<< ConsoleColor_FG_Blue << t->second << ConsoleColor_FG_Black
							<< "]-[bytes:"
							<< ConsoleColor_FG_Blue << ui64BufferSize << ConsoleColor_FG_Black
							<< "]";
					else
						std::cout
							<< "-[type:"
							<< ConsoleColor_FG_Red << "unknown" << ConsoleColor_FG_Black
							<< "]-[bytes:"
							<< ConsoleColor_FG_Blue << ui64BufferSize << ConsoleColor_FG_Black
							<< "]";
				}
			}

			virtual void closeChild(void)
			{
				m_vNodes.pop();
			}

			virtual OpenViBE::boolean processInput(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::uint32 ui32InputIndex)
			{
				rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
				return true;
			}

			virtual OpenViBE::boolean process(OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext)
			{
				OpenViBE::Plugins::IDynamicBoxContext* l_pDynamicBoxContext=rBoxAlgorithmContext.getDynamicBoxContext();
				for(OpenViBE::uint32 i=0; i<l_pDynamicBoxContext->getInputChunkCount(0); i++)
				{
					OpenViBE::uint64 l_ui64StartTime;
					OpenViBE::uint64 l_ui64EndTime;
					OpenViBE::uint64 l_ui64ChunkSize;
					const OpenViBE::uint8* l_pChunkBuffer;
					l_pDynamicBoxContext->getInputChunk(0, i, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer);
					l_pDynamicBoxContext->markInputAsDeprecated(0, i);
					m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
				}
				std::cout << std::endl;
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_EBMLStreamSpy)

		protected:

			std::stack<EBML::CIdentifier> m_vNodes;
			std::map<EBML::CIdentifier, std::string> m_vName;
			std::map<EBML::CIdentifier, std::string> m_vType;
			EBML::IReader* m_pReader;
			EBML::IReaderHelper* m_pReaderHelper;
		};

		class CEBMLStreamSpyDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("EBML stream spy"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("EBML stream tree viewer"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This sample EBML stream analyzer prints the EBML tree structure to the console"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_EBMLStreamSpy; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CEBMLStreamSpy(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Spied EBML stream", OV_UndefinedIdentifier);

				rPrototype.addSetting("EBML nodes description", OV_TypeId_String, "../share/ebml_stream_spy.cfg");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_EBMLStreamSpyDesc)
		};
	};
};

#endif // __SamplePlugin_CEBMLStreamSpy_H__
