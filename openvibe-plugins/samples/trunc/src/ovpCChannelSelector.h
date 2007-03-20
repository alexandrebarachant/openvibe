#ifndef __SamplePlugin_CChannelSelector_H__
#define __SamplePlugin_CChannelSelector_H__

#include "ovp_defines.h"
#include <openvibe-toolkit/ovtk_all.h>

#include <vector>
#include <map>
#include <string>
#include <openvibe/ov_all.h>

#include <ebml/IReader.h>
#include <ebml/IReaderHelper.h>
#include <ebml/IWriter.h>
#include <ebml/IWriterHelper.h>

#include <ebml/TReaderCallbackProxy.h>
#include <ebml/TWriterCallbackProxy.h>

namespace OpenViBEPlugins
{
	namespace Samples
	{	
		// Used to store information about the signal stream
		class CSignalDescription
		{
			public:
				CSignalDescription() : m_ui32StreamVersion(1), m_ui32ChannelCount (0), m_bReadyToSend(false), m_ui32CurrentChannel(0)
				{
				}

			public:
				EBML::uint32 m_ui32StreamVersion;
				EBML::uint32 m_ui32SamplingRate;
				EBML::uint32 m_ui32ChannelCount;
				EBML::uint32 m_ui32SampleCount;
				std::vector<std::string> m_pChannelName;
				EBML::uint32 m_ui32CurrentChannel;
				
				bool m_bReadyToSend;
		};
	
		
	
		/**
		* This plugin may be used to filter a signal stream in order to select only a few desired channels.
		* The desired output channels are to be selected in the plugin configuration box. The different entries 
		* needs to be seperated by a ";". Channels names not present in the original stream are ignored.
		*/
		class CChannelSelector : virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		, virtual public OpenViBEToolkit::IBoxAlgorithmSignalInputReaderCallback::ICallback
		{
		public:	
			CChannelSelector(void);
			
			virtual void release(void);
			
			virtual OpenViBE::boolean initialize();
			virtual OpenViBE::boolean uninitialize();
			
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			
			virtual OpenViBE::boolean process();
			
			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_ChannelSelector)

		public:
			virtual void writeSignalOutput(const void* pBuffer, const EBML::uint64 ui64BufferSize);

			virtual void setChannelCount(const OpenViBE::uint32 ui32ChannelCount);
			virtual void setChannelName(const OpenViBE::uint32 ui32ChannelIndex, const char* sChannelName);
			virtual void setSampleCountPerBuffer(const OpenViBE::uint32 ui32SampleCountPerBuffer);
			virtual void setSamplingRate(const OpenViBE::uint32 ui32SamplingFrequency);
			virtual void setSampleBuffer(const OpenViBE::float64* pBuffer);
		public:
			
			std::vector<std::string> m_vSelectedChannelNames;		//! Contains the names of the channels to select
			std::vector<OpenViBE::uint32> m_vSelectedChannelIndexes;	//! Contains the indexes of the channels to select

			// Needed to read the input
			EBML::IReader* m_pReader;
			
			OpenViBEToolkit::IBoxAlgorithmSignalInputReaderCallback* m_pSignalReaderCallBack;


			// the current node identifier
			EBML::CIdentifier m_oCurrentIdentifier;
			EBML::IReaderHelper* m_pReaderHelper;
			
			OpenViBE::uint64 m_ui64LastChunkStartTime;
			OpenViBE::uint64 m_ui64LastChunkEndTime;

			// if false then the next Dimension will be the channels' one
			//    true  then the samples' one
			OpenViBE::boolean m_bCurrentDimension;

			// stores the indexes of the selected channels in the original channel array
			std::map<std::string,OpenViBE::uint32> m_oSelectedIndex;



			// Needed to write on the plugin output
			EBML::IWriter* m_pWriter;

			EBML::TWriterCallbackProxy2<OpenViBEPlugins::Samples::CChannelSelector, &CChannelSelector::writeSignalOutput > m_oSignalOutputWriterCallbackProxy;

			OpenViBEToolkit::IBoxAlgorithmSignalOutputWriter* m_pSignalOutputWriterHelper;

			OpenViBE::boolean m_bSelectionbyIndex;		//! If true the channels are selected by their index in the stream instead of their name
									

			CSignalDescription * m_pSignalDescription;	//! Structure containing information about the signal stream
			OpenViBE::boolean m_bSignalDescriptionSent;	//! True if the signal description has been sent

			OpenViBE::uint64 m_ui64MatrixBufferSize; 	//! Size of the matrix buffer (output signal)
			EBML::float64* m_pMatrixBuffer;			//! Output signal's matrix buffer
			OpenViBE::boolean m_bMatrixReadyToSend;		//! True if the matrix is ready to be sent
		};
		
		
		/**
		* Description of the channel selection plugin
		*/
		class CChannelSelectorDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			
			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Channel Selector"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Bruno Renier"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Channel selector filter"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.6"); }
			
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_ChannelSelector; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CChannelSelector(); }
			
			virtual OpenViBE::boolean getBoxPrototype(
								OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Input signal", OV_TypeId_Signal);
				rPrototype.addOutput("Filtered signal", OV_TypeId_Signal);
				rPrototype.addSetting("Channels list", OV_TypeId_String, "");
				rPrototype.addSetting("Selection mode", OV_TypeId_String, "S");
		
				return true;
			}
			
			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_ChannelSelectorDesc)
		};
	

		
	}
}
#endif
