#ifndef __OpenViBEPlugins_SimpleVisualisation_CPowerSpectrumDisplayView_H__
#define __OpenViBEPlugins_SimpleVisualisation_CPowerSpectrumDisplayView_H__

#include "../ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <glade/glade.h>
#include <gtk/gtk.h>

#include "../ovpCBufferDatabase.h"
#include "ovpCPowerSpectrumChannelDisplay.h"

#include <vector>
#include <string>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		enum ESpectrumDisplayMode
		{
			ESpectrumDisplayMode_LocalBestFit,
			ESpectrumDisplayMode_GlobalBestFit/*,
			ESpectrumDisplayMode_ZoomIn,
			ESpectrumDisplayMode_ZoomOut*/,
			ESpectrumDisplayMode_NumDisplayMode
		};

		class CPowerSpectrumDatabase;

		/**
		 * This class contains everything necessary to setup a GTK window and display
		 * a signal thanks to a CBufferDatabase's information.
		 */
		class CPowerSpectrumDisplayView : public CSignalDisplayDrawable
		{
		public:
			CPowerSpectrumDisplayView(
				CPowerSpectrumDatabase& pPowerSpectrumDatabase);

			virtual ~CPowerSpectrumDisplayView();
			/**
			 * Initializes the window.
			 */
			virtual void init();
			/**
			 * Returns pointers to plugin main widget and toolbar widget
			 */
			void getWidgets(GtkWidget*& pWidget, GtkWidget*& pToolbarWidget);
			/**
			 * Returns pointer to topmost widget
			 */
			ESpectrumDisplayMode getCurrentDisplayMode(void);
			/**
			 * Sets current display mode.
			 * \param eDisplayMode Display mode to set.
			 */
			void setDisplayMode(
				ESpectrumDisplayMode eDisplayMode);
			/**
			 * Used to hide/show the rulers on the left of the spectrum displays.
			 * \param bActive Show the ruler if true.
			 */
			void toggleLeftRulers(
				OpenViBE::boolean bActive);
			/**
			 * Used to hide/show the frequency ruler
			 * \param bActive Show the ruler if true.
			 */
			void toggleBottomRuler(
				OpenViBE::boolean bActive);

			void showChannelSelectionDialogCB();
			void applyChannelSelectionCB();
			void applyMinMaxAttenuationCB();

			void resizeBottomRuler(
				gint w,
				gint h);
			void redrawBottomRuler();

		private:
			/**
			 * Invalidates the window's content and tells it to redraw itself.
			 */
			virtual void redraw();
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

			void updateMainTableStatus();

			void activateToolbarButtons(
				OpenViBE::boolean bActive);

			void setMinMaxAttenuation(
				OpenViBE::float64 f64Attenuation);

		private:
			void enableDisplayModeButtonSignals(
				OpenViBE::boolean);

			::GladeXML* m_pGladeInterface;
			//! Table containing the channel displays
			GtkWidget* m_pDisplayTable;
			//! Array of channels labels
			std::vector<GtkWidget*> m_oChannelLabels;
			//! Array of channel displays
			std::vector<CPowerSpectrumChannelDisplay*> m_oChannelDisplays;
			//! Bottom ruler widget
			GtkWidget* m_pBottomRuler;
			//! Pointers to display mode toggle buttons
			GtkToggleToolButton* m_pDisplayModeButtons[ESpectrumDisplayMode_NumDisplayMode];
			//! Display mode
			ESpectrumDisplayMode m_ui32CurrentDisplayMode;
			//! The database that contains the information to use to draw the signals
			CPowerSpectrumDatabase* m_pPowerSpectrumDatabase;
			//! Vector of pointers to the select channels dialog's check buttons
			std::vector<GtkWidget*> m_vChannelsCheckButtons;
			//! Vector of indexes of the channels to display
			std::vector<OpenViBE::uint32> m_vSelectedChannels;
			//CBottomTimeRuler * m_pBottomRuler;
		};
	}
}

#endif
