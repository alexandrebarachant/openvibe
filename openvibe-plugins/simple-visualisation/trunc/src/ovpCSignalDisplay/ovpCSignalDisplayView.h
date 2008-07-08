#ifndef __OpenViBEPlugins_SimpleVisualisation_CSignalDisplayView_H__
#define __OpenViBEPlugins_SimpleVisualisation_CSignalDisplayView_H__

#include "../ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <glade/glade.h>
#include <gtk/gtk.h>

#include "ovpCChannelDisplayWidget.h"
#include "../ovpCBufferDatabase.h"

#include "../ovpCBottomTimeRuler.h"

#include <vector>
#include <string>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		/**
		* This class contains everything necessary to setup a GTK window and display
		* a signal thanks to a CBufferDatabase's information.
		*/
		class CSignalDisplayView : public CSignalDisplayDrawable
		{
		public:

			CSignalDisplayView(
				CBufferDatabase& rBufferDatabase);

			virtual ~CSignalDisplayView(void);
			/**
			 * Returns pointers to plugin main widget and toolbar widget
			 */
			void getWidgets(GtkWidget*& pWidget, GtkWidget*& pToolbarWidget);
			/**
			 * Initializes the window.
			 */
			virtual void init(void);
			/**
			 * Invalidates the window's content and tells it to redraw itself.
			 */
			virtual void redraw(void);
			/**
			* Used to hide/show the rulers on the left of the signal displays.
			* \param bActive Show the ruler if true.
			*/
			void toggleLeftRulers(
				OpenViBE::boolean bActive);
			/**
			 * Used to hide/show the rulers on the bottom of the signal displays.
			 * \param bActive Show the ruler if true.
			 */
			void toggleBottomRulers(
				OpenViBE::boolean bActive);
			/**
			 * Hides a whole channel's display.
			 * \param ui64ChannelIndex The index of the channel to hide.
			 */
			void hideChannel(
				OpenViBE::uint32 ui32ChannelIndex);
			/**
			 * Shows a channel's display.
			 * \param ui64ChannelIndex The index of the channel to show.
			 */
			void showChannel(
				OpenViBE::uint32 ui32ChannelIndex);
			void changeMultiView(void);
			void updateMainTableStatus(void);
			void activateToolbarButtons(OpenViBE::boolean bActive);

		public:

			//! The Glade handler used to create the interface
			::GladeXML* m_pGladeInterface;

			//! The main window
			GtkWidget * m_pMainWindow;

			//! The table containing the CChannelDisplays
			GtkWidget * m_pSignalDisplayTable;

			//! Array of the channel's labels
			std::vector<GtkWidget*> m_oChannelLabel;

			//! Array of CChannelDisplays (one per channel, displays the corresponding channel)
			std::vector<GtkWidget*> m_oChannelDisplay;

			//! Pointers to the cursor mode buttons' widgets
			GtkToggleToolButton * m_pCursorMode[4];
			//! The current cursor mode
			OpenViBEPlugins::SimpleVisualisation::EDisplayMode m_eCurrentCursorMode;

			//! The cursors for the different cursor mode
			GdkCursor * m_pCursor[5];

			//! The database that contains the information to use to draw the signals
			CBufferDatabase * m_pBufferDatabase;

			//! vector of gdk points. Used to draw the signals.
			std::vector<GdkPoint> m_pPoints;

			//! vector of raw points. Stores the points' coordinates before cropping.
			std::vector<CChannelDisplayPoint> m_pRawPoints;

			//! Vector of pointers to the select channels dialog's check buttons
			std::vector<GtkWidget *> m_vChannelsCheckButtons;
			//! Vector of indexes of the channels to display
			std::vector<OpenViBE::uint32> m_vSelectedChannels;

			OpenViBE::boolean m_bMultiViewInitialized;
			std::vector<GtkWidget *> m_vMultiViewChannelsCheckButtons;
			std::vector<OpenViBE::uint32> m_vMultiViewSelectedChannels;

			CBottomTimeRuler * m_pBottomRuler;
		};
	}
}

#endif
