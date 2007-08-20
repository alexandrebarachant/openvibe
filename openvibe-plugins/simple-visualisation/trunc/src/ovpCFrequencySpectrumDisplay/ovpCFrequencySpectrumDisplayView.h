#ifndef __OpenViBEPlugins_SimpleVisualisation_CFrequencySpectrumDisplayView_H__
#define __OpenViBEPlugins_SimpleVisualisation_CFrequencySpectrumDisplayView_H__

#include "../ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <glade/glade.h>
#include <gtk/gtk.h>

#include "../ovpCBufferDatabase.h"
#include "../ovpCBottomTimeRuler.h"
#include "ovpCFrequencySpectrumChannelDisplay.h"

#include <vector>
#include <string>


namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{

			class CBufferDatabase;
			class CBottomTimeRuler;	

			/**
			 * This class contains everything necessary to setup a GTK window and display
			 * a signal thanks to a CBufferDatabase's information.
			 */
			class CFrequencySpectrumDisplayView : virtual public CSignalDisplayDrawable
			{

				public:

					//! The Glade handler used to create the interface
					::GladeXML* m_pGladeInterface;

					//! The main window
					GtkWidget * m_pMainWindow;

					//! The table containing the displays
					GtkWidget * m_pDisplayTable;

					//! Array of the channel's labels
					std::vector<GtkWidget*> m_oChannelLabel;

					//! Array of Displays (one per channel, displays the corresponding channel)
					std::vector<CFrequencySpectrumChannelDisplay*> m_oChannelDisplay;

					//! The database that contains the information to use to draw the signals
					CBufferDatabase * m_pBufferDatabase;

					//! Vector of pointers to the select channels dialog's check buttons
					std::vector<GtkWidget *> m_vChannelsCheckButtons;
					//! Vector of indexes of the channels to display
					std::vector<OpenViBE::uint32> m_vSelectedChannels;

					CBottomTimeRuler * m_pBottomRuler;
				public: 
					CFrequencySpectrumDisplayView(CBufferDatabase& pBufferDatabase);
					virtual ~CFrequencySpectrumDisplayView();

					/**
					 * Initializes the window.
					 */
					virtual void init();

					/**
					 * Invalidates the window's content and tells it to redraw itself.
					 */
					virtual void redraw();

					/**
					 * Used to hide/show the rulers on the left of the signal displays.
					 * \param bActive Show the ruler if true.
					 */
					void toggleLeftRulers(OpenViBE::boolean bActive);

					/**
					 * Used to hide/show the rulers on the bottom of the signal displays.
					 * \param bActive Show the ruler if true.
					 */
					void toggleBottomRulers(OpenViBE::boolean bActive);

					/**
					 * Hides a whole channel's display.
					 * \param ui64ChannelIndex The index of the channel to hide.
					 */
					void hideChannel(OpenViBE::uint32 ui32ChannelIndex);

					/**
					 * Shows a channel's display.
					 * \param ui64ChannelIndex The index of the channel to show.
					 */
					void showChannel(OpenViBE::uint32 ui32ChannelIndex);

					void updateMainTableStatus();

					void activateToolbarButtons(OpenViBE::boolean bActive);

					void setMinMaxAttenuation(OpenViBE::float64 f64Attenuation);
			};

	}
	
}

#endif
