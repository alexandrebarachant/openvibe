#ifndef __SamplePlugin_CBoxAlgorithmProducer_H__
#define __SamplePlugin_CBoxAlgorithmProducer_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <ebml/IWriter.h>
#include <ebml/IWriterHelper.h>
#include <system/Memory.h>
#include <iostream>
#include <vector>

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CBoxAlgorithmProducer : virtual public OpenViBE::Plugins::IBoxAlgorithm, virtual public EBML::IWriterCallback
		{
		public:

			CBoxAlgorithmProducer(void)
				:m_pWriter(NULL)
				,m_pWriterHelper(NULL)
				,m_pBoxAlgorithmContext(NULL)
			{
			}

			virtual void release(void)
			{
				delete this;
			}

			virtual OpenViBE::uint64 getClockFrequency(
				OpenViBE::Kernel::IBoxAlgorithmContext& rContext)
			{
				return 1LL<<32;
			}

			virtual OpenViBE::boolean initialize(
				OpenViBE::Kernel::IBoxAlgorithmContext& rContext)
			{
				m_pWriter=EBML::createWriter(*this);
				m_pWriterHelper=EBML::createWriterHelper();

				return true;
			}

			virtual OpenViBE::boolean uninitialize(
				OpenViBE::Kernel::IBoxAlgorithmContext& rContext)
			{
				m_pWriterHelper->release();
				m_pWriter->release();

				m_pWriterHelper=NULL;
				m_pWriter=NULL;

				return true;
			}

			virtual OpenViBE::boolean processClock(
				OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::Kernel::IMessageClock& rMessageClock)
			{
				rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
				return true;
			}

			virtual OpenViBE::boolean process(OpenViBE::Kernel::IBoxAlgorithmContext& rBoxAlgorithmContext)
			{
				OpenViBE::Kernel::IBoxIO* l_pDynamicBoxContext=rBoxAlgorithmContext.getDynamicBoxContext();

				m_pBoxAlgorithmContext=&rBoxAlgorithmContext;

				m_pWriterHelper->connect(m_pWriter);
				m_pWriterHelper->openChild(0);
				m_pWriterHelper->setASCIIStringAsChildData("OpenViBE producer sent this !");
				m_pWriterHelper->closeChild();
				m_pWriterHelper->disconnect();
				l_pDynamicBoxContext->markOutputAsReadyToSend(0, 0, 0);

				m_pBoxAlgorithmContext=NULL;
				return true;
			}

			virtual void write(
				const void* pBuffer,
				const EBML::uint64 ui64BufferSize)
			{
				OpenViBE::Kernel::IBoxIO* l_pDynamicBoxContext=m_pBoxAlgorithmContext->getDynamicBoxContext();

				std::cout << "Producer : Sending magic data : "
			    	      << ui64BufferSize
				          << " byte(s)" << std::endl;

				l_pDynamicBoxContext->appendOutputChunkData(0, static_cast<const OpenViBE::uint8*>(pBuffer), ui64BufferSize);
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_BoxAlgorithmProducer)

		protected:

			EBML::IWriter* m_pWriter;
			EBML::IWriterHelper* m_pWriterHelper;

		public:

			OpenViBE::Kernel::IBoxAlgorithmContext* m_pBoxAlgorithmContext;
		};

		class CBoxAlgorithmProducerDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Producer"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Sample data producer"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This sample producer sends strings on its outputs and shows how such sending could be done in OpenViBE"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithmProducer; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CBoxAlgorithmProducer(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addOutput("an output", OpenViBE::CIdentifier(0x1234, 0x5678));

				return true;
			}

			virtual OpenViBE::CString getStockItemName(void) const
			{
				return OpenViBE::CString("gtk-go-down");
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithmProducerDesc)
		};
	};
};

#endif // __SamplePlugin_CBoxAlgorithmProducer_H__
