#ifndef __OpenViBEPlugins_Stimulation_CFeatureAggregator_H__
#define __OpenViBEPlugins_Stimulation_CFeatureAggregator_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IWriter.h>
#include <ebml/TWriterCallbackProxy.h>

#include <string>
#include <vector>
#include <queue>

namespace OpenViBEPlugins
{
	namespace FeatureExtraction
	{
		/**
		 * Main plugin class of the feature aggregator plugins.
		 * Aggregates the features received in a feature vector then outputs it.
		 * */
		class CFeatureAggregator : virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, virtual public OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback::ICallback
		{
		public:

			CFeatureAggregator(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize();
			virtual OpenViBE::boolean uninitialize();

			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);

			virtual OpenViBE::boolean process();

			virtual void writeFeatureVectorOutput(const void* pBuffer, const EBML::uint64 ui64BufferSize);

			virtual void setMatrixDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount);
			virtual void setMatrixDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize);
			virtual void setMatrixDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel);
			virtual void setMatrixBuffer(const OpenViBE::float64* pBuffer);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_FeatureAggregator)

		public:
				//EBML writing stuff
				EBML::IWriter* m_pWriter;
				EBML::TWriterCallbackProxy1<OpenViBEPlugins::FeatureExtraction::CFeatureAggregator> * m_pOutputWriterCallbackProxy;
				OpenViBEToolkit::IBoxAlgorithmFeatureVectorOutputWriter * m_pFeatureVectorOutputWriterHelper;

				// Needed to read the input
				EBML::IReader* m_pReader;
				OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback* m_pMatrixReaderCallBack;

				// contains the labels for each dimmension for each input
				std::vector<std::vector<std::vector<std::string> > > m_oFeatureNames;

				// contains the dimmension size for each dimmension of each input
				std::vector<std::vector<OpenViBE::uint32> > m_oDimmensionSize;

				// contains the input buffer's total size for each input
				std::vector<OpenViBE::uint64> m_oInputBufferSizes;

				//start time and end time of the last arrived chunk
				OpenViBE::uint64 m_ui64LastChunkStartTime;
				OpenViBE::uint64 m_ui64LastChunkEndTime;

				// number of inputs
				OpenViBE::uint32 m_ui32NumberOfInput;

				// current input (usesful in the EBML callbacks)
				OpenViBE::uint32 m_ui32CurrentInput;

				// the feature vector
				OpenViBE::float64 * m_pVectorBuffer;
				// the feature vector size
				OpenViBE::uint32 m_ui32VectorSize;

				// error flag
				OpenViBE::boolean m_bError;
		};

		/**
		* Plugin's description
		*/
		class CFeatureAggregatorDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Feature aggregator"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Bruno Renier"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Aggregates features in a vector"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Aggregates features in a vector"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Feature extraction"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.5"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_FeatureAggregator; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::FeatureExtraction::CFeatureAggregator(); }

			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Feature Input 1", OV_TypeId_StreamedMatrix);
				rPrototype.addInput("Feature Input 2", OV_TypeId_StreamedMatrix);

				rPrototype.addOutput("Feature vector output", OV_TypeId_FeatureVector);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_FeatureAggregatorDesc)

		};

	};
};

#endif

