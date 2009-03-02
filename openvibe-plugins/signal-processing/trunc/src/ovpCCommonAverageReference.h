#ifndef __OpenViBEPlugins_SignalProcessing_CCommonAverageReference_H__
#define __OpenViBEPlugins_SignalProcessing_CCommonAverageReference_H__

#include "ovp_defines.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>
#include <ebml/IReaderHelper.h>
#include <ebml/IWriter.h>
#include <ebml/IWriterHelper.h>

#include <ebml/TReaderCallbackProxy.h>
#include <ebml/TWriterCallbackProxy.h>

#include <string>
#include <vector>

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{


		/**
		* The class to re-reference the data to Common Average Reference (CAR)
		*/
		class CCommonAverageReference : virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{



		public:

			CCommonAverageReference(void);

			virtual void release(void);

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);

			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_CommonAverageReference)

		public:
			void appendSignalOutputChunkData(const void* pBuffer,  EBML::uint64 ui64BufferSize)
			  {
			    appendOutputChunkData(0, pBuffer, ui64BufferSize);
			  }

			virtual void setChannelCount(const OpenViBE::uint32 ui32ChannelCount);
			virtual void setChannelName(const OpenViBE::uint32 ui32ChannelIndex, const char* sChannelName);
			virtual void setSampleCountPerBuffer(const OpenViBE::uint32 ui32SampleCountPerBuffer);
			virtual void setSamplingRate(const OpenViBE::uint32 ui32SamplingFrequency);
			virtual void setSampleBuffer(const OpenViBE::float64* pBuffer);

		public:

			// Needed to read the input
			EBML::IReader* m_pSignalReader;
			OpenViBEToolkit::IBoxAlgorithmSignalInputReaderCallback* m_pSignalReaderCallBack;
			OpenViBEToolkit::IBoxAlgorithmSignalInputReaderCallback::TCallbackProxy1<OpenViBEPlugins::SignalProcessing::CCommonAverageReference> m_oSignalReaderCallBackProxy;


			//start time and end time of the last arrived chunk
			OpenViBE::uint64 m_ui64LastChunkStartTime;
			OpenViBE::uint64 m_ui64LastChunkEndTime;

			// Needed to write on the plugin output
			EBML::IWriter* m_pSignalOutputWriter;
			EBML::TWriterCallbackProxy1<OpenViBEPlugins::SignalProcessing::CCommonAverageReference> m_oSignalOutputWriterCallbackProxy;
			OpenViBEToolkit::IBoxAlgorithmSignalOutputWriter* m_pSignalOutputWriterHelper;

			///number of channels
			OpenViBE::uint32 m_ui32ChannelCount;

			///number of samples per buffer
			OpenViBE::uint32 m_ui32SamplePerBufferCount;

			//the current input buffer being processed
			const OpenViBE::float64* m_pInputBuffer;

			//the current output buffer being generated
			OpenViBE::float64* m_pOutputBuffer;



		};

		/**
		* Description of the channel selection plugin
		*/
		class CCommonAverageReferenceDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Common average reference filter"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Fabien Lotte"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("IRISA/INSA Rennes"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Re-reference the signal to common average reference."); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Re-referencing the signal to common average reference consists in substracting to each sample the average value of the samples of all electrodes at this time"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/Filtering"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_CommonAverageReference; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessing::CCommonAverageReference(); }

			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Input signal", OV_TypeId_Signal);
				rPrototype.addOutput("Spatially filtered (common average re-referenced) signal", OV_TypeId_Signal);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_CommonAverageReferenceDesc)
		};
	}
}

#endif // __OpenViBEPlugins_SignalProcessing_CCommonAverageReference_H__

