#ifndef __OpenViBEPlugins_SimpleVisualisation_CGrazVisualization_H__
#define __OpenViBEPlugins_SimpleVisualisation_CGrazVisualization_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>

#include <glade/glade.h>
#include <gtk/gtk.h>

#include <vector>
#include <string>

#define Stimulation_Idle	0x320

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		enum EArrowDirection
		{
			EArrowDirection_None,
			EArrowDirection_Left,
			EArrowDirection_Right,
			EArrowDirection_Up,
			EArrowDirection_Down
		};

		enum EGrazVisualizationState
		{
			EGrazVisualizationState_Idle,
			EGrazVisualizationState_Reference,
			EGrazVisualizationState_Cue,
			EGrazVisualizationState_ContinousFeedback
		};

		/**
		*/
		class CGrazVisualization : 
			virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>,
		       	virtual public OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback::ICallback 
		{
		public:

			CGrazVisualization(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize();
			virtual OpenViBE::boolean uninitialize();
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process();
			
			virtual void setStimulationCount(const OpenViBE::uint32 ui32StimulationCount);
			virtual void setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate);

			virtual void processState();

			virtual void redraw();
			virtual void drawReferenceCross();
			virtual void drawArrow(EArrowDirection eDirection);
			virtual void drawBar();

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_GrazVisualization)

		public:
			//! The Glade handler used to create the interface
			::GladeXML* m_pGladeInterface;
			
			GtkWidget * m_pMainWindow;

			GtkWidget * m_pDrawingArea;


			//ebml 
			EBML::IReader* m_pReader;
			OpenViBEToolkit::IBoxAlgorithmStimulationInputReaderCallback* m_pStimulationReaderCallBack;

			EGrazVisualizationState m_eCurrentState;
			EArrowDirection m_eCurrentDirection;

			OpenViBE::float64 m_f64BarScale;

			//Start and end time of the last buffer
			OpenViBE::uint64 m_ui64StartTime;
			OpenViBE::uint64 m_ui64EndTime;
		};

		/**
		* Plugin's description
		*/
		class CGrazVisualizationDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Graz visualization"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Bruno Renier"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Visualization plugin for the Graz experiment"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Visualization/Feedback plugin for the Graz experiment"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Simple visualisation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_GrazVisualization; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SimpleVisualisation::CGrazVisualization(); }

			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Stimulations", OV_TypeId_Stimulations);
				return true;
			}

			
			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_GrazVisualizationDesc)

			
		};


	};
};

#endif
