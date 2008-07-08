#ifndef __SimpleVisualisationPlugin_CTopographicMap3DView_H__
#define __SimpleVisualisationPlugin_CTopographicMap3DView_H__

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
		class CTopographicMap3DView
		{
		public:

			enum ETopographicMap3DViewport
			{
				TopographicMap3DViewport_Top,
				TopographicMap3DViewport_NumViewport
			};

			CTopographicMap3DView();

			virtual ~CTopographicMap3DView();

			virtual void init();

			/**
			 * Returns pointer to toolbar
			 */
			void getToolbar(
				GtkWidget*& pToolbarWidget);
			/**
			 * Returns name of viewport currently used
			 */
			ETopographicMap3DViewport getCurrentViewport(void);
			/**
			 * Sets current viewport.
			 * \param eViewport Display mode to set.
			 */
			void setCurrentViewport(
				ETopographicMap3DViewport eViewport);

		private:
			/*
			static gboolean redrawCB(::GtkWidget* pWidget, ::GdkEventExpose* pEvent, gpointer data);
			static gboolean sizeAllocateCB(::GtkWidget* pWidget, ::GtkAllocation* pAllocation, gpointer data);
			*/
			/**
			 * Resizes the widget's drawing area.
			 */
			void resize(OpenViBE::uint32 ui32Width, OpenViBE::uint32 ui32Height) {};

			::GladeXML* m_pGladeInterface;
			ETopographicMap3DViewport m_ui32CurrentViewport;
		};
	}
}

#endif // __SimpleVisualisationPlugin_CTopographicMap3DView_H__
