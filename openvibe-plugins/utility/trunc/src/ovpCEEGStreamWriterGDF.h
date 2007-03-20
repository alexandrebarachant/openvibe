#ifndef __OpenViBEPlugins_Acquisition_CEEGStreamWriterGDF_H__
#define __OpenViBEPlugins_Acquisition_CEEGStreamWriterGDF_H__

#include "GDFHelpers.h"

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>
#include <ebml/IReaderHelper.h>

#include <system/Memory.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

#include <stdio.h>
#include <errno.h>


namespace OpenViBEPlugins
{
	namespace Utility
	{

		/**
		* The plugin's main class
		*
		*/
		class CEEGStreamWriterGDF : virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		, virtual public OpenViBEToolkit::IBoxAlgorithmSignalInputReaderCallback::ICallback
		, virtual public OpenViBEToolkit::IBoxAlgorithmExperimentInformationInputReaderCallback::ICallback
		, virtual public OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback::ICallback
		{
		public:

			CEEGStreamWriterGDF(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize();
			virtual OpenViBE::boolean uninitialize();
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process();

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_EEGStreamWriterGDF)

		public:
			virtual void setChannelCount(const OpenViBE::uint32 ui32ChannelCount);
			virtual void setChannelName(const OpenViBE::uint32 ui32ChannelIndex, const char* sChannelName);
			virtual void setSampleCountPerBuffer(const OpenViBE::uint32 ui32SampleCountPerBuffer);
			virtual void setSamplingRate(const OpenViBE::uint32 ui32SamplingFrequency);
			virtual void setSampleBuffer(const OpenViBE::float64* pBuffer);

			virtual void setValue(const OpenViBE::uint32 ui32ValueIdentifier, const OpenViBE::uint32 ui32Value);
			virtual void setValue(const OpenViBE::uint32 ui32ValueIdentifier, const char* sValue);

			virtual void setStimulationCount(const OpenViBE::uint32 ui32StimulationCount);
			virtual void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint32 ui32StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate);

			void saveMatrixData();
		public:
			std::ofstream m_oFile;
			OpenViBE::CString m_sFileName;
			
			EBML::IReaderCallBack* m_pReaderCallBack[3];
			EBML::IReader* m_pReader[3];
			
			OpenViBEToolkit::IBoxAlgorithmSignalInputReaderCallback* m_pSignalReaderCallBack;
			OpenViBEToolkit::IBoxAlgorithmExperimentInformationInputReaderCallback* m_pExperimentInformationReaderCallBack;
			OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback* m_pStimulationReaderCallBack;

			//GDF structures
			GDF::CFixedGDF1Header m_oFixedHeader;
			//std::vector< GDF::CVariableGDF1HeaderPerChannel > m_vVariableHeader;
			GDF::CVariableGDF1Header  m_oVariableHeader;


			std::vector< std::vector< OpenViBE::float64 > > m_vSamples;
			std::vector< OpenViBE::int64 > m_vSampleCount;
		
			
			OpenViBE::uint32 m_ui32SamplesPerChannel;
			OpenViBE::uint64 m_ui64SamplingFrequency;
		};

		/**
		* Plugin's description
		*/
		class CEEGStreamWriterGDFDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("EEG stream writer - GDF"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("This algorithm records on disk what comes from a specific output"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This algorithm dumps on disk a stream from a specific output in the standard GDF file format"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Utility"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_EEGStreamWriterGDF; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Utility::CEEGStreamWriterGDF(); }
			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Plugins::IBoxProto& rPrototype) const;

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_EEGStreamWriterGDFDesc)
		};

		
		
	};
};

#endif // __OpenViBEPlugins_Acquisition_CEEGStreamWriterGDF_H__
