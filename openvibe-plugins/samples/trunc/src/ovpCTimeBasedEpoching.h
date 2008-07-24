#ifndef __SamplePlugin_CTimeBasedEpoching_H__
#define __SamplePlugin_CTimeBasedEpoching_H__

#include "ovp_defines.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/TWriterCallbackProxy.h>

#include <vector>

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CTimeBasedEpoching : public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{
			class COutputHandler;

		public:

			CTimeBasedEpoching(void);

			virtual void release(void);

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_TimeBasedEpoching)

		public:

			virtual void setChannelCount(const OpenViBE::uint32 ui32ChannelCount);
			virtual void setChannelName(const OpenViBE::uint32 ui32ChannelIndex, const char* sChannelName);
			virtual void setSampleCountPerBuffer(const OpenViBE::uint32 ui32SampleCountPerBuffer);
			virtual void setSamplingRate(const OpenViBE::uint32 ui32SamplingFrequency);
			virtual void setSampleBuffer(const OpenViBE::float64* pBuffer);

		protected:

			std::vector<OpenViBEPlugins::Samples::CTimeBasedEpoching::COutputHandler*> m_vOutputHandler;
			OpenViBE::uint32 m_ui32InputSampleCountPerBuffer;
			OpenViBE::uint64 m_ui64LastStartTime;
			OpenViBE::uint64 m_ui64LastEndTime;

			EBML::IReader* m_pSignalInputReader;
			OpenViBEToolkit::IBoxAlgorithmSignalInputReaderCallback* m_pSignalInputReaderCallback;
			OpenViBEToolkit::IBoxAlgorithmSignalInputReaderCallback::TCallbackProxy1<CTimeBasedEpoching> m_oSignalInputReaderCallbackProxy;
		};

		class CTimeBasedEpochingDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Time based epoching"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/Epoching"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_TimeBasedEpoching; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CTimeBasedEpoching(); }
			virtual OpenViBE::CString getStockItemName(void) const       { return "gtk-cut"; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Input signal",               OV_TypeId_Signal);

				rPrototype.addOutput("Epoched signal 1",          OV_TypeId_Signal);

				rPrototype.addSetting("Epoch duration (in sec)",   OV_TypeId_Float,         "1");
				rPrototype.addSetting("Epoch intervals (in sec)",  OV_TypeId_Float,         "0.5");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_TimeBasedEpochingDesc)
		};
	};
};

#endif // __SamplePlugin_CTimeBasedEpoching_H__
