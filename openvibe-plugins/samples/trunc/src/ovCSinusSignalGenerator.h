#ifndef __SamplePlugin_CSinusSignalGenerator_H__
#define __SamplePlugin_CSinusSignalGenerator_H__

#include "ovp_defines.h"
#include "ovTEBMLOutputHandler.h"

#include <openvibe/ov_all.h>

#include <iostream>
#include <math.h>

#define OpenViBEStream_Header                           EBML::CIdentifier(1)
 #define OpenViBEStream_StreamVersion                   EBML::CIdentifier(2)
 #define OpenViBEStream_SamplingRate                    EBML::CIdentifier(3)
 #define OpenViBEStream_StreamedMatrixHeader            EBML::CIdentifier(4)
  #define OpenViBEStream_StreamedMatrixDimmensionCount  EBML::CIdentifier(5)
  #define OpenViBEStream_StreamedMatrixDimmension       EBML::CIdentifier(6)
   #define OpenViBEStream_StreamedMatrixDimmensionSize  EBML::CIdentifier(7)
   #define OpenViBEStream_StreamedMatrixDimmensionLabel EBML::CIdentifier(8)
#define OpenViBEStream_Buffer                           EBML::CIdentifier(9)
 #define OpenViBEStream_StreamedMatrixBuffer            EBML::CIdentifier(10)
  #define OpenViBEStream_StreamedMatrixRawBuffer        EBML::CIdentifier(11)

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CSinusSignalGenerator : virtual public OpenViBE::Plugins::IBoxAlgorithm
		{
		public:

			CSinusSignalGenerator(void)
				:m_pSignalOutputHandler(NULL)
				,m_bHeaderSent(false)
				,m_ui32ChannelCount(0)
				,m_ui32SamplingFrequency(0)
				,m_ui32GeneratedEpochSampleCount(0)
				,m_pSampleBuffer(NULL)
				,m_ui32SentSampleCount(0)
				,m_pBoxAlgorithmContext(NULL)
			{
			}

			virtual void release(void)
			{
			}

			virtual OpenViBE::boolean initialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				m_pSignalOutputHandler=new Common::TEBMLOutputHandler<CSinusSignalGenerator>(*this, 0);
				m_bHeaderSent=false;

				// Parses box settings to try connecting to server
				OpenViBE::CString l_sChannelCount;
				OpenViBE::CString l_sSamplingFrequency;
				OpenViBE::CString l_sGeneratedEpochSampleCount;
				rContext.getStaticBoxContext()->getSettingValue(0, l_sChannelCount);
				rContext.getStaticBoxContext()->getSettingValue(1, l_sSamplingFrequency);
				rContext.getStaticBoxContext()->getSettingValue(2, l_sGeneratedEpochSampleCount);
				m_ui32ChannelCount=atoi(l_sChannelCount);
				m_ui32SamplingFrequency=atoi(l_sSamplingFrequency);
				m_ui32GeneratedEpochSampleCount=atoi(l_sGeneratedEpochSampleCount);

				// Allocates sample block
				m_pSampleBuffer=new OpenViBE::float32[m_ui32ChannelCount*m_ui32GeneratedEpochSampleCount];

				return true;
			}

			virtual OpenViBE::boolean uninitialize(
				const OpenViBE::Plugins::IBoxAlgorithmContext& rContext)
			{
				delete [] m_pSampleBuffer;
				m_pSampleBuffer=NULL;

				delete m_pSignalOutputHandler;
				m_pSignalOutputHandler=NULL;

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
				rBoxAlgorithmContext.getDynamicBoxContext()->setOutputChunkSize(0, 0);
				m_pBoxAlgorithmContext=&rBoxAlgorithmContext;

				OpenViBE::uint32 i,j;
				if(!m_bHeaderSent)
				{
					char m_sChannelName[1024];
					static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_Header);
					 static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_StreamVersion);
					  static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->setUIntegerAsChildData(1);
					 static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
					 static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_SamplingRate);
					  static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->setUIntegerAsChildData(m_ui32SamplingFrequency);
					 static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
					 static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_StreamedMatrixHeader);
					  static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_StreamedMatrixDimmensionCount);
					   static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->setUIntegerAsChildData(2);
					  static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
					  static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_StreamedMatrixDimmension);
					   static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_StreamedMatrixDimmensionSize);
					    static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->setUIntegerAsChildData(m_ui32ChannelCount);
					   static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
					   for(i=0; i<m_ui32ChannelCount; i++)
					   {
					    sprintf(m_sChannelName, "Channel %i", i);
					    static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_StreamedMatrixDimmensionLabel);
					    static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->setASCIIStringAsChildData(m_sChannelName);
					    static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
					   }
					   static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_StreamedMatrixDimmensionSize);
					    static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->setUIntegerAsChildData(m_ui32GeneratedEpochSampleCount);
					   static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
					  static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
					 static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
					static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
					m_bHeaderSent=true;
				}

				for(i=0; i<m_ui32ChannelCount; i++)
				{
					for(j=0; j<m_ui32GeneratedEpochSampleCount; j++)
					{
						m_pSampleBuffer[i*m_ui32GeneratedEpochSampleCount+j]=sinf((2.0f*3.141592654f*i*(j+m_ui32SentSampleCount))/m_ui32SamplingFrequency);
					}
				}
				m_ui32SentSampleCount+=m_ui32GeneratedEpochSampleCount;

				static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_Buffer);
				 static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_StreamedMatrixBuffer);
				  static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->openChild(OpenViBEStream_StreamedMatrixRawBuffer);
				   static_cast<EBML::IWriter*>(*m_pSignalOutputHandler)->setChildData(m_pSampleBuffer, m_ui32ChannelCount*m_ui32GeneratedEpochSampleCount*sizeof(OpenViBE::float32));
				  static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
				 static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();
				static_cast<EBML::IWriterHelper*>(*m_pSignalOutputHandler)->closeChild();

				m_pBoxAlgorithmContext=NULL;
				rBoxAlgorithmContext.getDynamicBoxContext()->markOutputAsReadyToSend(0, 0); // $$$

				return true;
			}

			virtual void write(OpenViBE::uint32 ui32OutputIndex, const void* pBuffer, const OpenViBE::uint64 ui64BufferSize)
			{
				OpenViBE::Plugins::IDynamicBoxContext* l_pDynamicBoxContext=m_pBoxAlgorithmContext->getDynamicBoxContext();
				l_pDynamicBoxContext->appendOutputChunkData(ui32OutputIndex, static_cast<const OpenViBE::uint8*>(pBuffer), ui64BufferSize);
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_SinusSignalGenerator)

		protected:

			Common::TEBMLOutputHandler<CSinusSignalGenerator>* m_pSignalOutputHandler;
			OpenViBE::boolean m_bHeaderSent;
			OpenViBE::uint32 m_ui32ChannelCount;
			OpenViBE::uint32 m_ui32SamplingFrequency;
			OpenViBE::uint32 m_ui32GeneratedEpochSampleCount;
			OpenViBE::float32* m_pSampleBuffer;
			OpenViBE::uint32 m_ui32SentSampleCount;
			OpenViBE::Plugins::IBoxAlgorithmContext* m_pBoxAlgorithmContext;
			
		};

		class CSinusSignalGeneratorDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Sinus oscilator"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Simple sinux signal generator"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_SinusSignalGenerator; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CSinusSignalGenerator(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addOutput("Generated signal", OV_TypeId_Signal);

				rPrototype.addSetting("Channel count",                OV_TypeId_Integer, "32");
				rPrototype.addSetting("Sampling frequency",           OV_TypeId_Integer, "256");
				rPrototype.addSetting("Generated epoch sample count", OV_TypeId_Integer, "256");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_EBMLStreamSpyDesc)
		};
	};
};

#endif // __SamplePlugin_CSinusSignalGenerator_H__
