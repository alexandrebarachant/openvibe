#include "ovpCGrazVisualization.h"

#include <math.h>
#include <iostream>


using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{

		//! Callback for the close window button
		static void gtk_widget_do_nothing(::GtkWidget* pWidget)
		{
		}

		gboolean GrazVisualization_RedrawCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data)
		{
				reinterpret_cast<CGrazVisualization*>(data)->redraw();
				return TRUE;
		}


		void CGrazVisualization::setStimulationCount(const OpenViBE::uint32 ui32StimulationCount)
		{
			/* TODO nothing? */
		}

		void CGrazVisualization::setStimulation(const OpenViBE::uint32 ui32StimulationIndex, const OpenViBE::uint64 ui64StimulationIdentifier, const OpenViBE::uint64 ui64StimulationDate)
		{
			/*
			OVTK_GDF_Start_Of_Trial
			OVTK_GDF_Cross_On_Screen
			OVTK_GDF_Left
			OVTK_GDF_Right
			*/
			boolean l_bStateUpdated = false;

			switch(ui64StimulationIdentifier)
			{
				case Stimulation_Idle:
					m_eCurrentState = EGrazVisualizationState_Idle;
					l_bStateUpdated = true;
					break;

				case OVTK_GDF_Cross_On_Screen:
					m_eCurrentState = EGrazVisualizationState_Reference;
					l_bStateUpdated = true;
					break;

				case OVTK_GDF_Beep:
					gdk_beep();
					break;

				case OVTK_GDF_Left:
					m_eCurrentState = EGrazVisualizationState_Cue;
					m_eCurrentDirection = EArrowDirection_Left;
					l_bStateUpdated = true;
					break;

				case OVTK_GDF_Right:
					m_eCurrentState = EGrazVisualizationState_Cue;
					m_eCurrentDirection = EArrowDirection_Right;
					l_bStateUpdated = true;
					break;

				case OVTK_GDF_Up:
					m_eCurrentState = EGrazVisualizationState_Cue;
					m_eCurrentDirection = EArrowDirection_Up;
					l_bStateUpdated = true;
					break;

				case OVTK_GDF_Down:
					m_eCurrentState = EGrazVisualizationState_Cue;
					m_eCurrentDirection = EArrowDirection_Down;
					l_bStateUpdated = true;
					break;

				case OVTK_GDF_Feedback_Continuous:
					m_eCurrentState = EGrazVisualizationState_ContinousFeedback;
					l_bStateUpdated = true;
					break;
			}
		
			if(l_bStateUpdated)
			{
				processState();
			}
		}


		void CGrazVisualization::processState()
		{
		
			switch(m_eCurrentState)
			{
				case EGrazVisualizationState_Reference:
					gdk_window_invalidate_rect(GTK_WIDGET(m_pDrawingArea)->window,
							NULL,
							true);
					break;

				case EGrazVisualizationState_Cue:
					gdk_window_invalidate_rect(GTK_WIDGET(m_pDrawingArea)->window,
							NULL,
							true);
					break;

				case EGrazVisualizationState_Idle:
					gdk_window_invalidate_rect(GTK_WIDGET(m_pDrawingArea)->window,
							NULL,
							true);
					break;

				case EGrazVisualizationState_ContinousFeedback:
					gdk_window_invalidate_rect(GTK_WIDGET(m_pDrawingArea)->window,
							NULL,
							true);
					break;

				default:
					break;
			}
		}


		/**
		* Constructor
		*/
		CGrazVisualization::CGrazVisualization(void) :
			m_pGladeInterface(NULL),
			m_pMainWindow(NULL),
			m_pDrawingArea(NULL),
			m_pReader(NULL),
			m_pStimulationReaderCallBack(NULL),
			m_eCurrentState(EGrazVisualizationState_Idle),
			m_eCurrentDirection(EArrowDirection_None),
			m_f64BarScale(-0.5)
		{
			
		}
		
		
		OpenViBE::boolean CGrazVisualization::initialize()
		{
			m_pStimulationReaderCallBack=createBoxAlgorithmStimulationInputReaderCallback(*this);
			m_pReader = EBML::createReader(*m_pStimulationReaderCallBack);

			//load the glade interface
			m_pGladeInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-GrazVisualization.glade", NULL, NULL);
			
			if(!m_pGladeInterface)
			{
				g_warning("Couldn't load the interface!");
				return false;
			}
			
			glade_xml_signal_autoconnect(m_pGladeInterface);

			
			m_pDrawingArea = glade_xml_get_widget(m_pGladeInterface, "GrazVisualizationDrawingArea");
			g_signal_connect(G_OBJECT(m_pDrawingArea), "expose_event", G_CALLBACK(GrazVisualization_RedrawCallback), this);

			//does nothing on the main window if the user tries to close it
			g_signal_connect (G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "GrazVisualizationWindow")), 
					"delete_event", 
					G_CALLBACK(gtk_widget_do_nothing), NULL);

			//creates the window
			m_pMainWindow = glade_xml_get_widget(m_pGladeInterface, "GrazVisualizationWindow");
			gtk_widget_show_all(m_pMainWindow);

			return true;
		}
		
		
		OpenViBE::boolean CGrazVisualization::uninitialize()
		{
			releaseBoxAlgorithmStimulationInputReaderCallback(m_pStimulationReaderCallBack);

			//release the ebml reader
			m_pReader->release();
			m_pReader=NULL;
			
			
			//destroy the window and its children
			if(m_pMainWindow)
			{
				gtk_widget_destroy(m_pMainWindow);
				m_pMainWindow = NULL;
			}
			
			/* unref the xml file as it's not needed anymore */
			g_object_unref(G_OBJECT(m_pGladeInterface));
			m_pGladeInterface=NULL;
			
			return true;
		}
		
		
		OpenViBE::boolean CGrazVisualization::processInput(OpenViBE::uint32 ui32InputIndex)
		{
			getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
			return true;
		}
		
		
		OpenViBE::boolean CGrazVisualization::process()
		{
			IDynamicBoxContext* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();

			for(uint32 i=0; i<l_pDynamicBoxContext->getInputChunkCount(0); i++)
			{
				uint64 l_ui64ChunkSize;
				const uint8* l_pChunkBuffer=NULL;

				if(l_pDynamicBoxContext->getInputChunk(0, i, m_ui64StartTime, m_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer))
				{
					m_pReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
					l_pDynamicBoxContext->markInputAsDeprecated(0, i);
				}
			}
			
			return true;
		}
		
		void CGrazVisualization::redraw()
		{
			switch(m_eCurrentState)
			{
				case EGrazVisualizationState_Reference:
					drawReferenceCross();
					break;

				case EGrazVisualizationState_Cue:
					drawReferenceCross();
					drawArrow(m_eCurrentDirection);
					break;

				case EGrazVisualizationState_ContinousFeedback:
					drawReferenceCross();
					drawBar();
					break;

				default:
					break;
			}
		}

		void CGrazVisualization::drawReferenceCross()
		{
			gint l_iWindowWidth = m_pDrawingArea->allocation.width;
			gint l_iWindowHeight = m_pDrawingArea->allocation.height;
			
			//increase line's width
			gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 2, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);

			//horizontal line
			gdk_draw_line(m_pDrawingArea->window,
					m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
					(l_iWindowWidth/4), (l_iWindowHeight/2),
					((3*l_iWindowWidth)/4), (l_iWindowHeight/2)
					);
			 //vertical line
			gdk_draw_line(m_pDrawingArea->window,
					m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)],
					(l_iWindowWidth/2), (l_iWindowHeight/4),
					(l_iWindowWidth/2), ((3*l_iWindowHeight)/4)
					);

			//increase line's width
			gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 1, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);

		}

		void CGrazVisualization::drawArrow(EArrowDirection eDirection)
		{
			gint l_iWindowWidth = m_pDrawingArea->allocation.width;
			gint l_iWindowHeight = m_pDrawingArea->allocation.height;

			//for arrow baseline
			gint l_iX0 = l_iWindowWidth/2;
			gint l_iY0 = l_iWindowHeight/2;
			gint l_iX1 = 0;
			gint l_iY1 = 0;

			//
			gint l_iDX1 = 0;
			gint l_iDY1 = 0;
			gint l_iDX2 = 0;
			gint l_iDY2 = 0;


			switch(eDirection)
			{
				case EArrowDirection_Left:
					l_iX1 = (l_iWindowWidth/8);
					l_iY1 = (l_iWindowHeight/2);
					l_iDX1 = (l_iWindowWidth/8);
					l_iDY1 = -(l_iWindowHeight/8);
					l_iDX2 = (l_iWindowWidth/8);
					l_iDY2 = (l_iWindowHeight/8);
					break;

				case EArrowDirection_Right:
					l_iX1 = ((7*l_iWindowWidth)/8);
					l_iY1 = (l_iWindowHeight/2);
					l_iDX1 = -(l_iWindowWidth/8);
					l_iDY1 = -(l_iWindowHeight/8);
					l_iDX2 = -(l_iWindowWidth/8);
					l_iDY2 = (l_iWindowHeight/8);
					break;
				
				case EArrowDirection_Up:
					l_iX1 = (l_iWindowWidth/2);
					l_iY1 = (l_iWindowHeight/8);
					l_iDX1 = -(l_iWindowWidth/8);
					l_iDY1 = (l_iWindowHeight/8);
					l_iDX2 = (l_iWindowWidth/8);
					l_iDY2 = (l_iWindowHeight/8);
					break;

				case EArrowDirection_Down:
					l_iX1 = (l_iWindowWidth/2);
					l_iY1 = ((7*l_iWindowHeight)/8);
					l_iDX1 = -(l_iWindowWidth/8);
					l_iDY1 = -(l_iWindowHeight/8);
					l_iDX2 = (l_iWindowWidth/8);
					l_iDY2 = -(l_iWindowHeight/8);
					break;

				default:
					break;
			}
		
			//increase line's width
			gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 6, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);


			//draw base line
			gdk_draw_line(m_pDrawingArea->window,
					m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)],
					l_iX0, l_iY0,
					l_iX1, l_iY1
					);

			//arrow head
			gdk_draw_line(m_pDrawingArea->window,
					m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)],
					l_iX1, l_iY1,
					l_iX1+l_iDX1, l_iY1+l_iDY1
					);

			gdk_draw_line(m_pDrawingArea->window,
					m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)],
					l_iX1, l_iY1,
					l_iX1+l_iDX2, l_iY1+l_iDY2
					);

			//back to normal lines
			gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 1, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);

		}
		
		
		void CGrazVisualization::drawBar()
		{
			gint l_iWindowWidth = m_pDrawingArea->allocation.width;
			gint l_iWindowHeight = m_pDrawingArea->allocation.height;

			gint l_iRectangleWidth = static_cast<gint>(fabs(l_iWindowWidth * fabs(m_f64BarScale) / 2));
			gint l_iRectangleHeight = 2*l_iWindowHeight/8;

			gint l_iRectangleTopLeftX = l_iWindowWidth / 2;
			gint l_iRectangleTopLeftY = 3 *l_iWindowHeight / 8;

			if(m_f64BarScale<0)
			{
				l_iRectangleTopLeftX -= l_iRectangleWidth;
			}
	

			gdk_draw_rectangle(m_pDrawingArea->window,
					m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)],
					true,
					l_iRectangleTopLeftX, l_iRectangleTopLeftY,
					l_iRectangleWidth, l_iRectangleHeight
					);

		}

	};
};

