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
		class CBoxAlgorithmConsumer : virtual public OpenViBE::Plugins::IBoxAlgorithm, virtual public EBML::IReaderCallback
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
				OpenViBE::Kernel::IBoxAlgorithmContext& rContext)
			{
				m_pReader=EBML::createReader(*this);
				m_pReaderHelper=EBML::createReaderHelper();

				return true;
			}

			virtual OpenViBE::boolean uninitialize(
				OpenViBE::Kernel::IBoxAlgorithmContext& rContext)
			{
				m_pReaderHelper->release();
				m_pReader->release();

				m_pReaderHelper=NULL;
				m_pReader=NULL;

				return true;
			}

			virtual OpenViBE::boolean processInput(
				OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::uint32 ui32InputIndex)
			{
				rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
				return true;
			}

			virtual OpenViBE::boolean process(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
			{
				OpenViBE::Kernel::IBoxIO* l_pDynamicBoxContext=rBoxAlgorithmContext.getDynamicBoxContext();

				OpenViBE::uint64 l_ui64StartTime=0;
				OpenViBE::uint64 l_ui64EndTime=0;
				OpenViBE::uint64 l_ui64ChunkSize=0;
				const OpenViBE::uint8* l_pChunkBuffer=NULL;

				for(OpenViBE::uint32 j=0; j<l_pDynamicBoxContext->getInputChunkCount(0); j++)
				{
					l_pDynamicBoxContext->getInputChunk(0, j, l_ui64StartTime, l_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer);
					m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
					l_pDynamicBoxContext->markInputAsDeprecated(0, j);
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
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("an input", OpenViBE::CIdentifier(0x1234, 0x5678));

				return true;
			}

			virtual OpenViBE::CString getStockItemName(void) const
			{
				return OpenViBE::CString("gtk-go-up");
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithmConsumerDesc)
		};
	};
};

#endif // __SamplePlugin_CBoxAlgorithmConsumer_H__
