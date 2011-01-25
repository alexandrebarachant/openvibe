#ifndef __OpenViBEPlugins_SimpleVisualisation_CDisplayCueImage_H__
#define __OpenViBEPlugins_SimpleVisualisation_CDisplayCueImage_H__

#include "ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>
#include <ebml/IReader.h>
#include <gtk/gtk.h>
#include <vector>
#include <string>
#include <map>
#include <deque>

#define Stimulation_Idle 0x320

#define OVP_ClassId_DisplayCueImage                                            OpenViBE::CIdentifier(0x005789A4, 0x3AB78A36)
#define OVP_ClassId_DisplayCueImageDesc                                        OpenViBE::CIdentifier(0x086185A4, 0x796A854C)

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		enum EDisplayCueImageState
		{
			EDisplayCueImageState_Idle,
			EDisplayCueImageState_Reference,
			EDisplayCueImageState_Cue,
		};

		class CDisplayCueImage :
			public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>,
			public OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback::ICallback
		{
		public:

			CDisplayCueImage(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize();
			virtual OpenViBE::boolean uninitialize();
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process();

			virtual void setStimulationCount(const OpenViBE::uint32 ui32StimulationCount);
			virtual void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate);

			virtual void processState(void);

			virtual void redraw(void);
			virtual void resize(OpenViBE::uint32 ui32Width, OpenViBE::uint32 ui32Height);
			virtual void drawReferenceCross();
			virtual void drawCuePicture(OpenViBE::uint32 uint32CueID);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_DisplayCueImage)

		public:

			//The Builder handler used to create the interface
			::GtkBuilder* m_pBuilderInterface;
			::GtkWidget*  m_pMainWindow;
			::GtkWidget*  m_pDrawingArea;

			//ebml
			EBML::IReader* m_pReader[1];
			OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback* m_pStimulationReaderCallBack;

			//
			EDisplayCueImageState   m_eCurrentState;
			OpenViBE::uint32  m_uint32CurrentCueID;

			::GdkPixbuf** m_pOriginalPicture;
			::GdkPixbuf** m_pScaledPicture;

			::GdkColor m_oBackgroundColor;
			::GdkColor m_oForegroundColor;

			//Settings
			OpenViBE::uint32   m_ui32NuberOfCue;
			OpenViBE::CString* m_pImageNames;
			OpenViBE::uint64*  m_pStimulationsId;
			OpenViBE::uint64   m_ui64ClearScreenStimulation;
			OpenViBE::boolean  m_bFullScreen;
			OpenViBE::boolean  m_bPhotoDiodeOn;

			//Start and end time of the last buffer
			OpenViBE::uint64 m_ui64StartTime;
			OpenViBE::uint64 m_ui64EndTime;

			OpenViBE::boolean m_bError;
		};

		/**
		* Plugin's description
		*/
		class CDisplayCueImageDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Display cue image"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Joan Fruitet"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA Sophia"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Display cue images when receiving stimulations"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Display cue images when receiving specified stimulations and a fixation cross for OVTK_GDF_Cross_On_Screen"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Visualisation/Presentation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_DisplayCueImage; }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-fullscreen"); }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SimpleVisualisation::CDisplayCueImage(); }

			virtual OpenViBE::boolean hasFunctionality(OpenViBE::Kernel::EPluginFunctionality ePF) const
			{
				return ePF == OpenViBE::Kernel::PluginFunctionality_Visualization;
			}

			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Stimulations", OV_TypeId_Stimulations);

				rPrototype.addSetting("Number of Cues (maximum 6)", OV_TypeId_Integer, "6");
				rPrototype.addSetting("Resting Image", OV_TypeId_Filename, "../share/openvibe-plugins/simple-visualisation/p300-magic-card/openvibe-logo.png");
				rPrototype.addSetting("Resting Stimulation", OV_TypeId_Stimulation, "OVTK_StimulationId_Label_00");
				rPrototype.addSetting("Cue 1", OV_TypeId_Filename, "../share/openvibe-plugins/simple-visualisation/p300-magic-card/bomberman.png");
				rPrototype.addSetting("Stimulation 1", OV_TypeId_Stimulation, "OVTK_StimulationId_Label_01");
				rPrototype.addSetting("Cue 2", OV_TypeId_Filename, "../share/openvibe-plugins/simple-visualisation/p300-magic-card/dott.png");
				rPrototype.addSetting("Stimulation 2", OV_TypeId_Stimulation, "OVTK_StimulationId_Label_02");
				rPrototype.addSetting("Cue 3", OV_TypeId_Filename, "../share/openvibe-plugins/simple-visualisation/p300-magic-card/lemmings.png");
				rPrototype.addSetting("Stimulation 3", OV_TypeId_Stimulation, "OVTK_StimulationId_Label_03");
				rPrototype.addSetting("Cue 4", OV_TypeId_Filename, "../share/openvibe-plugins/simple-visualisation/p300-magic-card/mario.png");
				rPrototype.addSetting("Stimulation 4", OV_TypeId_Stimulation, "OVTK_StimulationId_Label_04");
				rPrototype.addSetting("Cue 5", OV_TypeId_Filename, "../share/openvibe-plugins/simple-visualisation/p300-magic-card/pacman.png");
				rPrototype.addSetting("Stimulation 5", OV_TypeId_Stimulation, "OVTK_StimulationId_Label_05");
				rPrototype.addSetting("Cue 6", OV_TypeId_Filename, "../share/openvibe-plugins/simple-visualisation/p300-magic-card/worms.png");
				rPrototype.addSetting("Stimulation 6", OV_TypeId_Stimulation, "OVTK_StimulationId_Label_06");
				rPrototype.addSetting("Clear screen", OV_TypeId_Stimulation, "OVTK_StimulationId_VisualStimulationStop");
				rPrototype.addSetting("Display images in full screen", OV_TypeId_Boolean, "false");
				rPrototype.addSetting("Show white square for photodiode", OV_TypeId_Boolean, "false");

				rPrototype.addFlag(OpenViBE::Kernel::BoxFlag_IsUnstable);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_DisplayCueImageDesc)
		};
	};
};

#endif
