#ifndef __SamplePlugin_CALGORITHM_EMA_H__
#define __SamplePlugin_CALGORITHM_EMA_H__

#include "../../ovp_defines.h"

#include <openvibe-toolkit/ovtk_all.h>

#define OVP_ClassId_AlgoEMADesc                                                         OpenViBE::CIdentifier(0x4FD93AB3, 0x40A543BF)
#define OVP_ClassId_AlgoEMA                                                             OpenViBE::CIdentifier(0x0E51575C, 0x102B1B9B)

#define OVP_Algorithm_EMA_InputParameter                                                OpenViBE::CIdentifier(0x0B811B83, 0x73483B4C)
#define OVP_Algorithm_EMA_Parameter                                                     OpenViBE::CIdentifier(0x013A71B3, 0x44EA5B0B)
#define OVP_Algorithm_EMA_OutputParameter                                               OpenViBE::CIdentifier(0x304A54D8, 0x78B81D2E)
#define OVP_Algorithm_EMA_InputTriggerId_Initialize                                     OpenViBE::CIdentifier(0x6C811820, 0x06A42DA1)
#define OVP_Algorithm_EMA_InputTriggerId_Process                                        OpenViBE::CIdentifier(0x4CFA2E2B, 0x18540E57)
#define OVP_Algorithm_EMA_OutputTriggerId_ProcessDone                                   OpenViBE::CIdentifier(0x35C76E3C, 0x16FE4B06)

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
		class CAlgorithmEMA : public OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >
		{
		public:

			virtual ~CAlgorithmEMA(void);

			virtual void release(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >, OVP_ClassId_AlgoEMA)

			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > ip_pMatrix;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pMatrix;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::float64 > ip_pParameter;
			std::vector<OpenViBE::float64 >lastBuffer;

		};

		class CAlgorithmEMADesc : public OpenViBE::Plugins::IAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("EMA"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("GIPSA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/GIPSA"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const 		 { return OpenViBE::CString("gtk-execute");	}

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_AlgoEMA; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessing::CAlgorithmEMA(); }

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rPrototype) const
			{
				// Adds box inputs
				rPrototype.addInputParameter (OVP_Algorithm_EMA_InputParameter, "Matrix", OpenViBE::Kernel::ParameterType_Matrix);
				rPrototype.addInputParameter (OVP_Algorithm_EMA_Parameter, "Alpha", OpenViBE::Kernel::ParameterType_Float );
				// Adds box outputs
				rPrototype.addOutputParameter(OVP_Algorithm_EMA_OutputParameter,    "Matrix", OpenViBE::Kernel::ParameterType_Matrix);
				// Adds box triggers
				rPrototype.addInputTrigger(OVP_Algorithm_EMA_InputTriggerId_Initialize,    "Initialize");
				rPrototype.addInputTrigger(OVP_Algorithm_EMA_InputTriggerId_Process,    "Process");
				rPrototype.addInputTrigger(OVP_Algorithm_EMA_OutputTriggerId_ProcessDone,    "ProcessDone");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IAlgorithmDesc, OVP_ClassId_AlgoEMADesc)
		};
	};
};

#endif
