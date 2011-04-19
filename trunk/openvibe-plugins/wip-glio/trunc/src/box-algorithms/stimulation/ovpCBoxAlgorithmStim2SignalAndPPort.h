#ifndef __OpenViBEPlugins_BoxAlgorithm_Stim2SignalAndPPort_H__
#define __OpenViBEPlugins_BoxAlgorithm_Stim2SignalAndPPort_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#if defined OVP_OS_Windows

#include <iostream>

//Plugin check time - modifs A2 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같�
#include <time.h>
//End modif A2 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

//stim2signalAndPPort boxAlgo
#define OVP_ClassId_BoxAlgorithm_Stim2SignalAndPPort		OpenViBE::CIdentifier(0x575B753D, 0x40F909D3)
#define OVP_ClassId_BoxAlgorithm_Stim2SignalAndPPortDesc	OpenViBE::CIdentifier(0x162F39E9, 0x3BC35A60)

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmStim2SignalAndPPort : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_Stim2SignalAndPPort);

		protected:

			// ...
			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalDecoder;
            OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecode1;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pDecodedMatrix;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > op_ui64SamplingRate;

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;
			OpenViBE::Kernel::TParameterHandler <const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecode2;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > op_pStimulationSet;

			OpenViBE::Kernel::IAlgorithmProxy* m_pSignalEncoder;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > ip_pMatrixToEncode;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > ip_ui64SamplingRate;

			OpenViBE::uint32 m_ui32FlagStart;
			OpenViBE::uint64 m_ui64Stimulation;
			OpenViBE::CString m_StimulationLabel;
			OpenViBE::uint32 m_ui32StimChannelState;

			clock_t m_clock_t_StartClock;


		};

		class CBoxAlgorithmStim2SignalAndPPortDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Stimulation 2 Signal and PPort"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Guillaume Lio"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("GIPSA-Lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Convert stimulation in a supplementary binary channel for the signal and send 0x64 to the parallel port."); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-connect"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_Stim2SignalAndPPort; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmStim2SignalAndPPort; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Original Data",		OV_TypeId_Signal										);
				rBoxAlgorithmPrototype.addInput  ("Stimulations",		OV_TypeId_Stimulations									);
				rBoxAlgorithmPrototype.addOutput ("Data+Stimulations",	OV_TypeId_Signal										);

				rBoxAlgorithmPrototype.addSetting("Trigger",            OV_TypeId_Stimulation, "OVTK_StimulationId_Label_01"	);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_Stim2SignalAndPPortDesc);
		};
	};
};

#endif

#endif // __OpenViBEPlugins_BoxAlgorithm_Stim2SignalAndPPort_H__
