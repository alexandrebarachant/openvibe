#ifndef __OpenViBEPlugins_BoxAlgorithm_Stim2Matrix_H__
#define __OpenViBEPlugins_BoxAlgorithm_Stim2Matrix_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <iostream>

//Plugin check time - modifs A2 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같�
#include <time.h>
//End modif A2 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

#define OVP_ClassId_BoxAlgorithm_Stim2Matrix     OpenViBE::CIdentifier(0x63DF24AB, 0x7F1F47A9)
#define OVP_ClassId_BoxAlgorithm_Stim2MatrixDesc OpenViBE::CIdentifier(0x1B0C3BD2, 0x62DD0313)

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
		class CBoxAlgorithmStim2Matrix : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_Stim2Matrix);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;
			OpenViBE::Kernel::TParameterHandler <const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecode;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > op_pStimulationSet;

			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalEncoder;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > ip_pMatrixToEncode;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;

			OpenViBE::uint64 m_ui64StimulationStart;
			OpenViBE::uint64 m_ui64StimulationEnd;
			OpenViBE::CMatrix *m_oMatrix;
		};

		class CBoxAlgorithmStim2MatrixDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Stimulation to Stream Matrix"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("GIPSA-Lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Convert stimulation in a unitary signal."); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/Basic"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-jump-to"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_Stim2Matrix; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessing::CBoxAlgorithmStim2Matrix; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Stimulations",	OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("Data",			OV_TypeId_StreamedMatrix);
				rBoxAlgorithmPrototype.addSetting("plage begin",	OV_TypeId_Stimulation, "OVTK_StimulationId_Label_00"	);
				rBoxAlgorithmPrototype.addSetting("plage end",		OV_TypeId_Stimulation, "OVTK_StimulationId_Label_06"	);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_Stim2MatrixDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_Stim2Matrix_H__
