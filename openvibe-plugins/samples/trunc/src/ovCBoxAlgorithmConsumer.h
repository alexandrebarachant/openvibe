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
		class CBoxAlgorithmConsumer : virtual public OpenViBE::Plugins::IBoxAlgorithm, virtual public EBML::IReaderCallBack
		{
		public:

			CBoxAlgorithmConsumer(void)
				:m_pReader(NULL)
				,m_pReaderHelper(NULL)
			{
			}

			virtual void release(void)
			{
				delete this;
			}

			virtual OpenViBE::boolean initialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				const OpenViBE::Plugins::IStaticBoxContext* l_pStaticBoxContext=rContext.getStaticBoxContext();

				m_pReader=EBML::createReader(*this);
				m_pReaderHelper=EBML::createReaderHelper();

				return true;
			}

			virtual OpenViBE::boolean uninitialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				m_pReaderHelper->release();
				m_pReader->release();

				m_pReaderHelper=NULL;
				m_pReader=NULL;

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

				for(OpenViBE::uint32 i=0; i<l_pDynamicBoxContext->getInputChunkCount(0); i++)
				{
					OpenViBE::uint64 l_ui64StartTime;
					OpenViBE::uint64 l_ui64EndTime;
					OpenViBE::uint64 l_ui64ChunkSize;
					const OpenViBE::uint8* l_pBuffer;
					l_pDynamicBoxContext->getInputChunk(0, i, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pBuffer);
					l_pDynamicBoxContext->markInputAsDeprecated(0, i);
					m_pReader->processData(l_pBuffer, l_ui64ChunkSize);
				}

				return true;
			}

			virtual EBML::boolean isMasterChild(
				const EBML::CIdentifier& rIdentifier)
			{
				return false;
			}

			virtual void openChild(
				const EBML::CIdentifier& rIdentifier)
			{
			}

			virtual void processChildData(
				const void* pBuffer,
				const EBML::uint64 ui64BufferSize)
			{
				std::cout << "Consumer : Received magic phrase ["
				          << m_pReaderHelper->getASCIIStringFromChildData(pBuffer, ui64BufferSize)
				          << "]" << std::endl;
			}

			virtual void closeChild(void)
			{
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_BoxAlgorithmConsumer)

		protected:

			EBML::IReader* m_pReader;
			EBML::IReaderHelper* m_pReaderHelper;
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

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithmConsumer; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CBoxAlgorithmConsumer(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("an input", OV_UndefinedIdentifier);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithmConsumerDesc)
		};
	};
};

#endif // __SamplePlugin_CBoxAlgorithmConsumer_H__
