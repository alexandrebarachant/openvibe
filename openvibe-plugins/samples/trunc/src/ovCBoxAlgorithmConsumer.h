#ifndef __SamplePlugin_CBoxAlgorithmConsumer_H__
#define __SamplePlugin_CBoxAlgorithmConsumer_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <ebml/IReader.h>
#include <ebml/IReaderHelper.h>
#include <iostream>
#include <vector>

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CBoxAlgorithmConsumerCB : virtual public EBML::IReaderCallBack
		{
		public:

			CBoxAlgorithmConsumerCB(OpenViBE::uint32 ui32InputIndex)
				:m_ui32InputIndex(ui32InputIndex)
				,m_pReaderHelper(NULL)
			{
				m_pReaderHelper=EBML::createReaderHelper();
			}

			virtual ~CBoxAlgorithmConsumerCB(void)
			{
				m_pReaderHelper->release();
			}

			virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier)
			{
				return false;
			}

			virtual void openChild(const EBML::CIdentifier& rIdentifier)
			{
			}

			virtual void processChildData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
			{
				std::cout << "Consumer : Received magic phrase ["
				          << m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize)
				          << "] from input ["
				          << m_ui32InputIndex
				          << "]" << std::endl;
			}

			virtual void closeChild(void)
			{
			}

		public:

			OpenViBE::uint32 m_ui32InputIndex;
			EBML::IReaderHelper* m_pReaderHelper;
		};

		class CBoxAlgorithmConsumer : virtual public OpenViBE::Plugins::IBoxAlgorithm
		{
		public:

			virtual void release(void)
			{
				delete this;
			}

			virtual OpenViBE::boolean initialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				const OpenViBE::Plugins::IStaticBoxContext* l_pStaticBoxContext=rContext.getStaticBoxContext();
				for(OpenViBE::uint32 i=0; i<l_pStaticBoxContext->getInputCount(); i++)
				{
					EBML::IReaderCallBack* l_pReaderCallBack=new CBoxAlgorithmConsumerCB(i);
					EBML::IReader* l_pReader=EBML::createReader(*l_pReaderCallBack);

					m_vEBMLReaderCallBack.push_back(l_pReaderCallBack);
					m_vEBMLReader.push_back(l_pReader);
				}
				return true;
			}

			virtual OpenViBE::boolean uninitialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				std::vector < EBML::IReader* > ::iterator i;
				for(i=m_vEBMLReader.begin(); i!=m_vEBMLReader.end(); i++)
				{
					(*i)->release();
				}
				m_vEBMLReader.clear();

				std::vector < EBML::IReaderCallBack* > ::iterator j;
				for(j=m_vEBMLReaderCallBack.begin(); j!=m_vEBMLReaderCallBack.end(); j++)
				{
					delete *j;
				}
				m_vEBMLReaderCallBack.clear();

				return true;
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
				OpenViBE::Plugins::IStaticBoxContext* l_pStaticBoxContext=rBoxAlgorithmContext.getStaticBoxContext();
				OpenViBE::Plugins::IDynamicBoxContext* l_pDynamicBoxContext=rBoxAlgorithmContext.getDynamicBoxContext();
				for(OpenViBE::uint32 i=0; i<l_pStaticBoxContext->getInputCount(); i++)
				{
					for(OpenViBE::uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(i); j++)
					{
						OpenViBE::uint64 l_ui64StartTime;
						OpenViBE::uint64 l_ui64EndTime;
						OpenViBE::uint64 l_ui64ChunkSize;
						const OpenViBE::uint8* l_pBuffer;
						l_pDynamicBoxContext->getInputChunk(i, j, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pBuffer);
						l_pDynamicBoxContext->markInputAsDeprecated(i, j);
						m_vEBMLReader[i]->processData(l_pBuffer, l_ui64ChunkSize);
					}
				}
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_BoxAlgorithmConsumer)

		protected:

			std::vector < EBML::IReader* > m_vEBMLReader;
			std::vector < EBML::IReaderCallBack* > m_vEBMLReaderCallBack;
		};

		class CBoxAlgorithmConsumerDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Consumer"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Sample data consumer"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This sample consumer receives strings on its inputs and shows how such receiving could be done in OpenViBE"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::uint32 getClockFrequency(const OpenViBE::Plugins::IStaticBoxContext& rStaticBoxContext) const { return 0; }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithmConsumer; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CBoxAlgorithmConsumer(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("a sample input 1", OpenViBE::CIdentifier(0,1));
				rPrototype.addInput("a sample input 2", OpenViBE::CIdentifier(1,0));
				rPrototype.addInput("a sample input 3", OpenViBE::CIdentifier(1,1));

				rPrototype.addOutput("an output 1", OpenViBE::CIdentifier(0,1));
				rPrototype.addOutput("an output 2", OpenViBE::CIdentifier(1,0));
				rPrototype.addOutput("an output 3", OpenViBE::CIdentifier(1,1));

				rPrototype.addSetting("a setting 1", OpenViBE::CIdentifier(0,1), "default value 1");
				rPrototype.addSetting("a setting 2", OpenViBE::CIdentifier(1,0), "default value 2");
				rPrototype.addSetting("a setting 3", OpenViBE::CIdentifier(1,1), "default value 3");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithmConsumerDesc)
		};
	};
};

#endif // __SamplePlugin_CBoxAlgorithmConsumer_H__
