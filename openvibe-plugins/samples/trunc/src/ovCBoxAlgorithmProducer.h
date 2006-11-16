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
		class CBoxAlgorithmProducer;

		class CBoxAlgorithmProducerCB : virtual public EBML::IWriterCallBack
		{
		public:

			CBoxAlgorithmProducerCB(CBoxAlgorithmProducer& rParent, OpenViBE::uint32 ui32OutputIndex);
			virtual void write(const void* pBuffer, const EBML::uint64 ui64BufferSize);

		public:

			OpenViBE::uint64 m_ui64CurrentChunkSize;

		protected:

			CBoxAlgorithmProducer& m_rParent;
			OpenViBE::uint32 m_ui32OutputIndex;
		};

		class CBoxAlgorithmProducer : virtual public OpenViBE::Plugins::IBoxAlgorithm
		{
		public:

			CBoxAlgorithmProducer(void)
				:m_pEBMLWriterHelper(NULL)
				,m_pBoxAlgorithmContext(NULL)
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
				for(OpenViBE::uint32 i=0; i<l_pStaticBoxContext->getOutputCount(); i++)
				{
					CBoxAlgorithmProducerCB* l_pWriterCallBack=new CBoxAlgorithmProducerCB(*this, i);
					EBML::IWriter* l_pWriter=EBML::createWriter(*l_pWriterCallBack);

					m_vEBMLWriterCallBack.push_back(l_pWriterCallBack);
					m_vEBMLWriter.push_back(l_pWriter);
				}

				m_pEBMLWriterHelper=EBML::createWriterHelper();

				return true;
			}

			virtual OpenViBE::boolean uninitialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				m_pEBMLWriterHelper->release();
				m_pEBMLWriterHelper=NULL;

				std::vector < EBML::IWriter* > ::iterator i;
				for(i=m_vEBMLWriter.begin(); i!=m_vEBMLWriter.end(); i++)
				{
					(*i)->release();
				}
				m_vEBMLWriter.clear();

				std::vector < CBoxAlgorithmProducerCB* > ::iterator j;
				for(j=m_vEBMLWriterCallBack.begin(); j!=m_vEBMLWriterCallBack.end(); j++)
				{
					delete *j;
				}
				m_vEBMLWriterCallBack.clear();

				return true;
			}

			virtual OpenViBE::boolean processClock(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::CMessageClock& rMessageClock)
			{
				rBoxAlgorithmContext.markAlgorithmAsReadyToProcess();
				return true;
			}

			virtual OpenViBE::boolean process(OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext)
			{
				OpenViBE::Plugins::IStaticBoxContext* l_pStaticBoxContext=rBoxAlgorithmContext.getStaticBoxContext();
				m_pBoxAlgorithmContext=&rBoxAlgorithmContext;
				for(OpenViBE::uint32 i=0; i<l_pStaticBoxContext->getOutputCount(); i++)
				{
					m_vEBMLWriterCallBack[i]->m_ui64CurrentChunkSize=0;
					m_pEBMLWriterHelper->connect(m_vEBMLWriter[i]);
					m_pEBMLWriterHelper->openChild(1); // $$$
					m_pEBMLWriterHelper->setASCIIStringAsChildData("OpenViBE producer sent this !");
					m_pEBMLWriterHelper->closeChild();
					m_pEBMLWriterHelper->disconnect();
				}
				m_pBoxAlgorithmContext=NULL;
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_BoxAlgorithmProducer)

		public:

			OpenViBE::Plugins::IBoxAlgorithmContext* m_pBoxAlgorithmContext;

		protected:

			std::vector < CBoxAlgorithmProducerCB* > m_vEBMLWriterCallBack;
			std::vector < EBML::IWriter* > m_vEBMLWriter;
			EBML::IWriterHelper* m_pEBMLWriterHelper;
		};

		CBoxAlgorithmProducerCB::CBoxAlgorithmProducerCB(
			CBoxAlgorithmProducer& rParent,
			OpenViBE::uint32 ui32OutputIndex)
			:m_rParent(rParent)
			,m_ui32OutputIndex(ui32OutputIndex)
		{
		}

		void CBoxAlgorithmProducerCB::write(
			const void* pBuffer,
			const EBML::uint64 ui64BufferSize)
		{
			std::cout << "Producer : Sending magic data on output ["
			          << m_ui32OutputIndex
			          << "] : "
			          << ui64BufferSize
			          << " byte(s)" << std::endl;

			OpenViBE::Plugins::IDynamicBoxContext* l_pDynamicBoxContext=m_rParent.m_pBoxAlgorithmContext->getDynamicBoxContext();
			l_pDynamicBoxContext->setOutputChunkSize(m_ui32OutputIndex, m_ui64CurrentChunkSize+ui64BufferSize, false);
			System::Memory::copy(l_pDynamicBoxContext->getOutputChunkBuffer(m_ui32OutputIndex)+m_ui64CurrentChunkSize, pBuffer, ui64BufferSize);
			m_ui64CurrentChunkSize+=ui64BufferSize;
			l_pDynamicBoxContext->markOutputAsReadyToSend(m_ui32OutputIndex, 0); // $$$
		}

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
			virtual OpenViBE::uint32 getClockFrequency(const OpenViBE::Plugins::IStaticBoxContext& rStaticBoxContext) const { return 1; }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithmProducer; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CBoxAlgorithmProducer(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addOutput("some production 1", OV_TypeId_String);
				rPrototype.addOutput("some production 2", OV_TypeId_String);
				rPrototype.addOutput("some production 3", OV_TypeId_String);

				rPrototype.addSetting("activation frequency", OV_TypeId_Integer, "1");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithmProducerDesc)
		};
	};
};

#endif // __SamplePlugin_CBoxAlgorithmProducer_H__
