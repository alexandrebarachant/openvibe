#ifndef __OpenViBEPlugins_BoxAlgorithm_TestAhmed_H__
#define __OpenViBEPlugins_BoxAlgorithm_TestAhmed_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <deque>

#define OVP_ClassId_BoxAlgorithm_TestAhmed        OpenViBE::CIdentifier(0x3B5E08D0, 0x6E6A13BF)
#define OVP_ClassId_BoxAlgorithm_TestAhmedDesc    OpenViBE::CIdentifier(0x21883E41, 0x1788384C)

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
		class CBoxAlgorithmTestAhmed : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_TestAhmed);

		protected:

			// ...
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoderRow;
            OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecodeRow;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > op_pStimulationSetRow;

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoderColumn;
            OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecodeColumn;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > op_pStimulationSetColumn;
			
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoderCommand;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > ip_pStimulationSetCommand;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBufferCommand;

			OpenViBE::boolean RowHeader;
			OpenViBE::boolean ColumnHeader;
			OpenViBE::boolean HeaderSent;
			OpenViBE::boolean CommandDetected;
			std::deque<OpenViBE::int32 > queueRow;
			std::deque<OpenViBE::int32 > queueColumn;
			OpenViBE::boolean FirstLetterOK;
		};

		class CBoxAlgorithmTestAhmedDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Test Ahmed"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("GIPSA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("test"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/GIPSA"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-about"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_TestAhmed; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessing::CBoxAlgorithmTestAhmed; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("row input", OVTK_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addInput  ("column input", OVTK_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("command output", OVTK_TypeId_Stimulations);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_TestAhmedDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_EMA_H__
