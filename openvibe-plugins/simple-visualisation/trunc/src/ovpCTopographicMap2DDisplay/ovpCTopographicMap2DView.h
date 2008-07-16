#ifndef __SimpleVisualisationPlugin_CTopographicMap2DView_H__
#define __SimpleVisualisationPlugin_CTopographicMap2DView_H__

#include "../ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <glade/glade.h>
#include <gtk/gtk.h>

#include "../ovpCTopographicMapDatabase.h"

#include <vector>
#include <string>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		/**
		 * This class contains everything necessary to setup a GTK window and display
		 * a 2D topographic map
		 */
		class CTopographicMap2DView : public CTopographicMapDrawable
		{
		public:
			enum ETopographicMap2DProjection
		{
			TopographicMap2DProjection_Axial,
			TopographicMap2DProjection_Radial,
			TopographicMap2DProjection_NumProjection
		};

			enum ETopographicMap2DViewport
			{
				TopographicMap2DViewport_Top,/*
				TopographicMap2DViewport_Left,
				TopographicMap2DViewport_Right,
				TopographicMap2DViewport_Back*/
				TopographicMap2DViewport_NumViewport
			};

			CTopographicMap2DView(
				CTopographicMapDatabase& rTopographicMapDatabase);

			virtual ~CTopographicMap2DView();

			//CSignalDisplayDrawable implementation
			//-------------------------------------
			/**
			 * Initializes the window.
			 */
			virtual void init();
			/**
			 * Invalidates the window's content and tells it to redraw itself.
			 */
			virtual void redraw();
			//-------------------------------------

			//CTopographicMapDrawable implementation
			//----------------------------------------
			/**
			* Returns pointer to sample coords matrix (places where to interpolate values)
			*/
			virtual OpenViBE::CMatrix* getSampleCoordinatesMatrix();
			/**
			* Sets pointer to sample values matrix (values interpolated at places specified in sample matrix)
			*/
			virtual OpenViBE::boolean setSampleValuesMatrix(
				OpenViBE::IMatrix* pSampleValuesMatrix);
			//----------------------------------------

			/**
			 * Returns pointers to plugin main widget and toolbar widget
			 */
			void getWidgets(
				GtkWidget*& pWidget,
				GtkWidget*& pToolbarWidget);
			/**
			 * Returns name of viewport currently used
			 */
			ETopographicMap2DViewport getCurrentViewport(void);
			/**
			 * Sets current viewport.
			 * \param eViewport Display mode to set.
			 */
			void setCurrentViewport(
				ETopographicMap2DViewport eViewport);

		private:
			void drawPalette(
				OpenViBE::uint32 ui32X,
				OpenViBE::uint32 ui32Y,
				OpenViBE::uint32 ui32Width,
				OpenViBE::uint32 ui32Height);

			void drawTopViewport(
				OpenViBE::uint32 ui32X,
				OpenViBE::uint32 ui32Y,
				OpenViBE::uint32 ui32Width,
				OpenViBE::uint32 ui32Height);

			void refreshPotentials();
			void drawPotentials();

			void drawElectrodes();

			void drawBoxToBuffer(
				OpenViBE::uint32 ui32X,
				OpenViBE::uint32 ui32Y,
				OpenViBE::uint32 ui32Width,
				OpenViBE::uint32 ui32Height,
				OpenViBE::uint8 ui8Red,
				OpenViBE::uint8 ui8Green,
				OpenViBE::uint8 ui8Blue);

			void enableProjectionButtonSignals(
				OpenViBE::boolean);
			void setProjection(
				ETopographicMap2DProjection eProjection);

			static void toggleAxialProjectionCB(
				GtkWidget* pWidget,
				gpointer data);
			static void toggleRadialProjectionCB(
				GtkWidget* widget,
				gpointer data);

			static gboolean redrawCB(::GtkWidget* pWidget, ::GdkEventExpose* pEvent, gpointer data);

			static gboolean sizeAllocateCB(::GtkWidget* pWidget, ::GtkAllocation* pAllocation, gpointer data);

			/**
			 * Resizes the widget's drawing area.
			 */
			void resize(OpenViBE::uint32 ui32Width, OpenViBE::uint32 ui32Height);

			OpenViBE::uint32 getSamplesCount();
			void computeSamplesNormalizedCoordinates();
			void computeColorIndices();

			void resizeData();

			//void enableViewportButtonSignals(OpenViBE::boolean);

			//! The database that contains the information to use to draw the signals
			CTopographicMapDatabase& m_rTopographicMapDatabase;

			::GladeXML* m_pGladeInterface;

			//! Pointers to projection type toggle buttons
			GtkToggleToolButton* m_pProjectionButtons[TopographicMap2DProjection_NumProjection];
			//! Display mode
			ETopographicMap2DProjection m_ui32CurrentProjection;

			::GtkWidget* m_pDrawingArea;
			::GdkBitmap* m_pClipmask; //origin is upper left corner of skull bounding square
			::GdkGC* m_pClipmaskGC;
			OpenViBE::boolean m_bRedrawClipmask;
			::GdkColor m_oBackgroundColor;
			ETopographicMap2DViewport m_ui32CurrentViewport;

			OpenViBE::boolean m_bNeedResize;

			OpenViBE::uint32 m_ui32GridSize;
			OpenViBE::uint32 m_ui32CellSize;

			OpenViBE::CMatrix m_oSampleCoordinatesMatrix;

			std::vector<OpenViBE::uint32> m_oSampleValues;
			std::vector<std::pair<OpenViBE::uint32, OpenViBE::uint32> > m_oSample2DCoordinates; //in skull coords

			OpenViBE::uint32 m_ui32MinPaletteBarHeight;
			OpenViBE::uint32 m_ui32MaxPaletteBarHeight;

			OpenViBE::uint32 m_ui32HeadWindowWidth;
			OpenViBE::uint32 m_ui32HeadWindowHeight;

			OpenViBE::uint32 m_ui32PaletteWindowWidth;
			OpenViBE::uint32 m_ui32PaletteWindowHeight;

			OpenViBE::uint32 m_ui32NoseY;
			OpenViBE::uint32 m_ui32SkullX;
			OpenViBE::uint32 m_ui32SkullY;
			OpenViBE::uint32 m_ui32SkullDiameter;

			guchar* m_pSkullRGBBuffer;
			OpenViBE::uint32 m_ui32RowStride;
		};
	}
}

#endif // __SimpleVisualisationPlugin_CTopographicMap2DView_H__
