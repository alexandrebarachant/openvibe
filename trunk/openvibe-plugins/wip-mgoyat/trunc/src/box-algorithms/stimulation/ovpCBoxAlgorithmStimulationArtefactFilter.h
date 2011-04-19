#ifndef __OpenViBEPlugins_BoxAlgorithm_StimArtefactFilter_H__
#define __OpenViBEPlugins_BoxAlgorithm_StimArtefactFilter_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <vector>

#define OVP_ClassId_BoxAlgorithm_StimArtefactFilter  		   		OpenViBE::CIdentifier(0x22A36C4C, 0x73DE4034)
#define OVP_ClassId_BoxAlgorithm_StimArtefactFilterDesc 			OpenViBE::CIdentifier(0x4001285C, 0x3578034A)

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		class CBoxAlgorithmStimArtefactFilter : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32Index);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_StimArtefactFilter);

		protected:

			OpenViBE::uint64 m_ui64MinimumDuration;
			OpenViBE::uint64 m_ui64StimulationStart;
			OpenViBE::uint64 m_ui64StimulationEnd;

		private:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoder;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IStimulationSet* > ip_pStimulationSet;
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoder;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecode;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > op_pStimulationSet;
			OpenViBE::IStimulationSet* m_oStimulationSet;
			OpenViBE::IStimulationSet* m_oReserveStimulationSet;
			
			OpenViBE::uint64 m_ui64startTimeStim;
			OpenViBE::boolean m_bSkip;
			//
			OpenViBE::boolean m_bNeedChunkMemory;
			OpenViBE::boolean m_bCutChunkTime;
			OpenViBE::uint64 m_ui64MemStartTime;
			OpenViBE::uint64 m_ui64MemEndTime;
		};

		class CBoxAlgorithmStimArtefactFilterDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("stimulation time artefact filter"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu Goyat"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-select-font"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_StimArtefactFilter; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CBoxAlgorithmStimArtefactFilter; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Incoming stimulations",           OV_TypeId_Stimulations);
				rBoxAlgorithmPrototype.addOutput ("Produced stimulations",           OV_TypeId_Stimulations);

				rBoxAlgorithmPrototype.addSetting("minimum time of duration (s)",  	OV_TypeId_Float, "0.016");
				rBoxAlgorithmPrototype.addSetting("Stimulation type of start",  	OV_TypeId_Stimulation, "OVTK_StimulationId_Label_1A");
				rBoxAlgorithmPrototype.addSetting("Stimulation type of end",  		OV_TypeId_Stimulation, "OVTK_StimulationId_Label_1B");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_StimArtefactFilterDesc);
		};
	};
};

#endif
