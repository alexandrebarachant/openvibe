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
		 * This class manages Gtk widgets used to display a power spectrum.
		 * Power spectrum data is retrieved from an external CPowerSpectrumDatabase object.
		 */
		class CPowerSpectrumDisplayView : public CSignalDisplayDrawable
		{
		public:
			/**
			 * \brief Constructor
			 * \param pPowerSpectrumDatabase Object storing power spectrum data
			 * \param f64MinDisplayedFrequency Minimum frequency to display
			 * \param f64MaxDisplayedFrequency Maximum frequency to display
			 */
			CPowerSpectrumDisplayView(
				CPowerSpectrumDatabase& pPowerSpectrumDatabase,
				OpenViBE::float64 f64MinDisplayedFrequency,
				OpenViBE::float64 f64MaxDisplayedFrequency);

			/**
			 * \brief Destructor
			 */
			virtual ~CPowerSpectrumDisplayView();

			/**
			 * \brief Initialize widgets
			 * \remark Called automatically by spectrum database when first buffer is received
			 */
			virtual void init();

			/**
			 * \brief Get pointers to plugin main widget and (optional) toolbar widget
			 * \param [out] pWidget Pointer to main widget
			 * \param [out] pToolbarWidget Pointer to (optional) toolbar widget
			 */
			void getWidgets(
				::GtkWidget*& pWidget,
				::GtkWidget*& pToolbarWidget);

			/**
			 * \brief Get current display mode
			 * \return Current display mode
			 */
			ESpectrumDisplayMode getCurrentDisplayMode(void);

			/**
			 * \brief Set current display mode.
			 * \param eDisplayMode Display mode to set.
			 */
			void setDisplayMode(
				ESpectrumDisplayMode eDisplayMode);

			/**
			 * \brief Toggle left rulers (amplitude values) visibility
			 * \param bActive Visibility flag
			 */
			void toggleLeftRulers(
				OpenViBE::boolean bActive);

			/**
			 * \brief Toggle bottom ruler (frequency scale) visibility
			 * \param bActive Visibility flag
			 */
			void toggleBottomRuler(
				OpenViBE::boolean bActive);

			/**
			 * \brief Draw bottom ruler
			 */
			void redrawBottomRuler();

			/** \name Gtk widget callbacks */
			//@{
			void minDisplayedFrequencyChangedCB(::GtkWidget* pWidget);
			void maxDisplayedFrequencyChangedCB(::GtkWidget* pWidget);
			void resizeBottomRulerCB(gint w, gint h);
			void showChannelSelectionDialogCB();
			void applyChannelSelectionCB();
			void applyMinMaxAttenuationCB();
			//@}

		private:
			/**
			 * \brief Invalidate window contents and have it redraw itself
			 */
			virtual void redraw();

			/**
			 * \brief Hide a given channel
			 * \param ui32ChannelIndex Index of channel to hide
			 */
			void hideChannel(
				OpenViBE::uint32 ui32ChannelIndex);

			/**
			 * \brief Show a given channel
			 * \param ui32ChannelIndex Index of channel to show
			 */
			void showChannel(
				OpenViBE::uint32 ui32ChannelIndex);

			/**
			 * \brief Update channel widgets visibility
			 */
			void updateMainTableStatus();

			/**
			 * \brief Set toolbar buttons activation state
			 * \param bActive Activation flag
			 */
			void activateToolbarButtons(
				OpenViBE::boolean bActive);

		private:
			//! The Glade handler used to create the interface
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
			//! Minimum frequency to display
			OpenViBE::float64 m_f64MinDisplayedFrequency;
			//! Maximum frequency to display
			OpenViBE::float64 m_f64MaxDisplayedFrequency;
		};
	}
}

#endif
