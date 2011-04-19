#ifndef __OpenViBEPlugins_BoxAlgorithm_Signal2Stim_H__
#define __OpenViBEPlugins_BoxAlgorithm_Signal2Stim_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <iostream>
#include <deque>

#define OVP_ClassId_BoxAlgorithm_Signal2Stim     OpenViBE::CIdentifier(0x746C3B2E, 0x7B7549E5)
#define OVP_ClassId_BoxAlgorithm_Signal2StimDesc OpenViBE::CIdentifier(0x09F97C05, 0x31B64514)

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
		class CBoxAlgorithmSignal2Stim : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_Signal2Stim);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalDecoderTrigger;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecodeTrigger;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pDecodedMatrixTrigger;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > op_ui64SamplingRateTrigger;

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoder;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > ip_pStimulationsToEncode;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;

			OpenViBE::uint64 m_ui64StimulationToTrigUp;
			OpenViBE::uint64 m_ui64StimulationToTrigDown;
			OpenViBE::float64 m_f64MemFront;

		};

		class CBoxAlgorithmSignal2StimDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Signal to Stimulation"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Goyat Matthieu"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("GIPSA-Lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Convert Trigger to stimulation"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/Basic"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-jump-to"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_Signal2Stim; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessing::CBoxAlgorithmSignal2Stim; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Trigger channel",		OV_TypeId_Signal		);
				rBoxAlgorithmPrototype.addOutput ("Output stimulations",	OV_TypeId_Stimulations	);

				rBoxAlgorithmPrototype.addSetting("Stimulation for rising edge",    OV_TypeId_Stimulation, "OVTK_StimulationId_Label_00"	);
				rBoxAlgorithmPrototype.addSetting("Stimulation for downward edge",  OV_TypeId_Stimulation, "OVTK_StimulationId_Label_00"	);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_Signal2StimDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_Stim2Signal_H__
