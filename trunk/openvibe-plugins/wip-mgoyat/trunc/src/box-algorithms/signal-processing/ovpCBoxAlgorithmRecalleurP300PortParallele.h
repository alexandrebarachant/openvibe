#ifndef __OpenViBEPlugins_BoxAlgorithm_RecalleurP300PP_H__
#define __OpenViBEPlugins_BoxAlgorithm_RecalleurP300PP_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <iostream>
#include <deque>
#include <queue>
#include <fstream>

#define OVP_ClassId_BoxAlgorithm_RecalleurP300     OpenViBE::CIdentifier(0x54A74421, 0x1D8F7F26)
#define OVP_ClassId_BoxAlgorithm_RecalleurP300Desc OpenViBE::CIdentifier(0x20707B52, 0x3B992070)

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
	
		typedef struct structure1
		{
			OpenViBE::uint64 time;
			OpenViBE::boolean itIsAStart;
			OpenViBE::uint64 chunkStartTime;
			OpenViBE::uint64 chunkEndTime;
		}StructureTimeTrigger;
		
		//typedef struct structure2
		class StructureStimulationP300AtTime
		{
		public:
			OpenViBE::CStimulationSet stimSet;
			OpenViBE::uint64 chunkStartTime;
			OpenViBE::uint64 chunkEndTime;
		};//StructureStimulationP300AtTime;
		
		
		class CBoxAlgorithmRecalleurP300 : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_RecalleurP300);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoderTrigger;
			OpenViBE::Kernel::TParameterHandler <const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecodeTrigger;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > op_pStimulationSetTrigger;
			
			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationDecoderRCIndex;
			OpenViBE::Kernel::TParameterHandler <const OpenViBE::IMemoryBuffer* > ip_pMemoryBufferToDecodeRCIndex;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > op_pStimulationSetRowColumnIndex;

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoder;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > ip_pStimulationsToEncode;
            OpenViBE::Kernel::TParameterHandler < OpenViBE::IMemoryBuffer* > op_pEncodedMemoryBuffer;
			
			OpenViBE::uint64 m_ui64StimulationTriggerStart;
			OpenViBE::uint64 m_ui64StimulationToTrigStart;
			OpenViBE::uint64 m_ui64StimulationTriggerStop;
			OpenViBE::uint64 m_ui64StimulationToTrigStop;
			
			std::deque<StructureTimeTrigger> m_oStimTimeTrigger;
			std::deque<StructureStimulationP300AtTime*> m_oStimP300;
			std::deque<StructureStimulationP300AtTime*> m_oConcernedStimP300;
			std::deque<StructureStimulationP300AtTime*> m_oStimOthers;
			
			OpenViBE::uint64 m_ui64lastTriggerTime;
			
			OpenViBE::uint64 m_ui64LastChunkStartTimeTrigger;
			OpenViBE::uint64 m_ui64LastChunkEndTimeTrigger;
			
			OpenViBE::boolean AsNotTime(OpenViBE::uint64 time);
			OpenViBE::boolean aggregateStim(OpenViBE::uint64 id, OpenViBE::uint64 date, OpenViBE::uint64 duration);
			OpenViBE::int32 findStimP300IndexWith(OpenViBE::boolean);
			void ChangeTimeAtIdx(const StructureTimeTrigger&,OpenViBE::int32);
			
			void FindAndSendOthersBeforeThisTrigger(OpenViBE::Kernel::IBoxIO*,const StructureTimeTrigger&,OpenViBE::int32);
			OpenViBE::int32 findOthersBefore(OpenViBE::uint64 date);
			void setTimeOthers(OpenViBE::uint64,OpenViBE::int32);
			void sendStimulationOthers(OpenViBE::Kernel::IBoxIO*,OpenViBE::int32);
			void sendAllOthers(OpenViBE::Kernel::IBoxIO*);
			void flushAllOthers();
			
			
			FILE * pFile;

		};

		class CBoxAlgorithmRecalleurP300Desc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Recalleur P300"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Goyat Matthieu"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("GIPSA-Lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Reajuste les temps des stimulations sur l'entrée trigger"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/GIPSA"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-nothing"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_RecalleurP300; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessing::CBoxAlgorithmRecalleurP300; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Trigger channel",		OV_TypeId_Stimulations	);
				rBoxAlgorithmPrototype.addInput  ("P300 stimulations",		OV_TypeId_Stimulations	);
				rBoxAlgorithmPrototype.addOutput ("Output stimulations",	OV_TypeId_Stimulations	);

				rBoxAlgorithmPrototype.addSetting("Stimulation which trig for start", 	OV_TypeId_Stimulation, "OVTK_StimulationId_Label_1A"	);
				rBoxAlgorithmPrototype.addSetting("Stimulation to trig for start",    	OV_TypeId_Stimulation, "OVTK_StimulationId_VisualStimulationStart"	);
				rBoxAlgorithmPrototype.addSetting("Stimulation which trig for stop",  	OV_TypeId_Stimulation, "OVTK_StimulationId_Label_1B"	);
				rBoxAlgorithmPrototype.addSetting("Stimulation to trig for stop",  		OV_TypeId_Stimulation, "OVTK_StimulationId_VisualStimulationStop"	);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_RecalleurP300Desc);
		};
	};
};

#endif
