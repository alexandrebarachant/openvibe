#ifndef __OpenViBEPlugins_SignalProcessingGpl_BoxAlgorithms_Filter_CLDABoxAlgorithm_H__
#define __OpenViBEPlugins_SignalProcessingGpl_BoxAlgorithms_Filter_CLDABoxAlgorithm_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>
#include <vector>

namespace OpenViBEPlugins
{
	namespace SignalProcessingGpl
	{
		class CLDABoxAlgorithm : virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_Box_LDABoxAlgorithm)

		protected:

			std::vector < OpenViBE::Kernel::IAlgorithmProxy* > m_vStreamDecoder;
			std::vector < OpenViBE::uint64 > m_vStreamDecoderEndTime;

			OpenViBE::Kernel::IAlgorithmProxy* m_pStreamEncoder;

			OpenViBE::Kernel::IAlgorithmProxy* m_pComputeFisherLdaFunction;
			OpenViBE::Kernel::IAlgorithmProxy* m_pApplyFisherLdaFunction;
			OpenViBE::Kernel::IAlgorithmProxy* m_pFeatureExtractionLda;

			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* >  m_oSignalIntputMatrixHandle;

			OpenViBE::CMatrix m_oClass1SamplesNumbersInputMatrix;
			OpenViBE::CMatrix* m_pEpochTable;
			OpenViBE::float64* m_pf64EpochTable ;

			OpenViBE::uint64 m_ui64NbEntries;
			OpenViBE::uint64 m_ui64NbEntriesDecoded;

			OpenViBE::uint64 m_ui64LastStartTime;
			OpenViBE::uint64 m_ui64LastEndTime;

			std::vector<OpenViBE::uint64> m_oCoefficients;

			OpenViBE::uint64 m_ui64TrainingIndex;
			OpenViBE::uint64 m_uint64NbTrainingSamples;
			OpenViBE::boolean m_bFirstTime;
			OpenViBE::boolean m_bFirstTimeComputeLDA;

			OpenViBE::boolean m_bHasSentHeader;
		};

		class CLDABoxAlgorithmDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Linear discriminant analisys"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Guillaume Gibert"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INSERM/U821"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("-Unstable-/Signal processing/Classification"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString(""); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Box_LDABoxAlgorithm; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessingGpl::CLDABoxAlgorithm(); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addInput  ("Input epochs",               OV_TypeId_StreamedMatrix);
				rPrototype.addInput  ("Input epochs",               OV_TypeId_StreamedMatrix);
				rPrototype.addInput  ("Input epochs",               OV_TypeId_StreamedMatrix);
				rPrototype.addInput  ("Input epochs",               OV_TypeId_StreamedMatrix);
				rPrototype.addInput  ("Input epochs",               OV_TypeId_StreamedMatrix);
				rPrototype.addInput  ("Input epochs",               OV_TypeId_StreamedMatrix);
				rPrototype.addOutput ("Decision value",             OV_TypeId_StreamedMatrix);
				rPrototype.addSetting("Number of training samples", OV_TypeId_Integer, "5");
				rPrototype.addSetting("Class 1 Samples numbers",    OV_TypeId_String, "2 5 1 3 6");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_Box_LDABoxAlgorithmDesc)
		};
	};
};

#endif // __OpenViBEPlugins_SignalProcessingGpl_BoxAlgorithms_Filter_CLDABoxAlgorithm_H__
