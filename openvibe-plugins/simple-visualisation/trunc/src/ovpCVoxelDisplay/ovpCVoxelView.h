#ifndef __SimpleVisualisationPlugin_CVoxelView_H__
#define __SimpleVisualisationPlugin_CVoxelView_H__

#include "../ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <glade/glade.h>
#include <gtk/gtk.h>

#include "../ovpCBufferDatabase.h"

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
		class CVoxelView
		{
		public:
			/*
			enum EVoxelViewport
			{
				VoxelViewport_Top,
				VoxelViewport_NumViewport
			};*/

			CVoxelView();

			virtual ~CVoxelView();

			virtual void init();

			/**
			 * Returns pointer to toolbar
			 */
			void getToolbar(
				GtkWidget*& pToolbarWidget);
			/**
			 * Returns name of viewport currently used
			 */
			//EVoxelViewport getCurrentViewport(void);
			/**
			 * Sets current viewport.
			 * \param eViewport Display mode to set.
			 */
			//void setCurrentViewport(
				//EVoxelViewport eViewport);

		private:
			/*
			static gboolean redrawCB(::GtkWidget* pWidget, ::GdkEventExpose* pEvent, gpointer data);
			static gboolean sizeAllocateCB(::GtkWidget* pWidget, ::GtkAllocation* pAllocation, gpointer data);
			*/
			/**
			 * Resizes the widget's drawing area.
			 */
			//void resize(OpenViBE::uint32 ui32Width, OpenViBE::uint32 ui32Height) {};

			::GladeXML* m_pGladeInterface;
			//EVoxelViewport m_ui32CurrentViewport;
		};
	}
}

#endif // __SimpleVisualisationPlugin_CVoxelView_H__
