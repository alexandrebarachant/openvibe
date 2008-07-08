#ifndef __OpenViBEPlugins_SimpleVisualisation_CPowerSpectrumChannelDisplay_H__
#define __OpenViBEPlugins_SimpleVisualisation_CPowerSpectrumChannelDisplay_H__

#include "ovpCPowerSpectrumDatabase.h"

#include <openvibe/ov_all.h>
#include <gtk/gtk.h>
#include <iostream>
using namespace std;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		class CPowerSpectrumDisplayView;

		class CPowerSpectrumChannelDisplay
		{
		public:
			CPowerSpectrumChannelDisplay(
				CPowerSpectrumDisplayView* pParentDisplay,
				OpenViBE::uint32 ui32Channel,
				CPowerSpectrumDatabase& pDatabase);

			~CPowerSpectrumChannelDisplay();

			//! returns the widget, so it can be added to the main interface
			GtkWidget* getWidget() const;

			GtkWidget* getSpectrumDisplay() const;

			//! toggles (show/hide)
			void toggle(
				OpenViBE::boolean bActive);

			/**
			* Shows or hides the left ruler.
			* \param bActive If true, shows the left ruler, else hides it.
			*/
			void toggleLeftRuler(OpenViBE::boolean bActive);

			//! draws the spectrum
			void drawSpectrum();

			//! Draws the left ruler.
			void drawLeftRuler();

			//! Draws the bottom ruler.
			void drawBottomRuler();

			//! computes the frequency spectrum
			void update();

			void resizeRGBBuffer(
				OpenViBE::uint32 ui32Width,
				OpenViBE::uint32 ui32Height);

			void drawBoxToBuffer(
				OpenViBE::uint32 ui32X,
				OpenViBE::uint32 ui32Y,
				OpenViBE::uint32 ui32Width,
				OpenViBE::uint32 ui32Height,
				OpenViBE::uint8 ui8Red,
				OpenViBE::uint8 ui8Green,
				OpenViBE::uint8 ui8Blue);

			void setMinMaxAttenuation(
				OpenViBE::float64 f64Attenuation);

		private:
			CPowerSpectrumDisplayView* m_pParentDisplay;
			//! Table containing spectrum and ruler
			GtkTable* m_pWidgetTable;
			//! Drawing area where the spectrum is to be drawn
			GtkWidget* m_pDisplay;
			//! Drawing area where powers/frequencies are drawn
			GtkWidget* m_pLeftRuler;

			GdkGC* m_pGraphicsContext;
			CPowerSpectrumDatabase* m_pDatabase;
			OpenViBE::uint32 m_ui32Channel;
			OpenViBE::float64 m_f64MinimumValue, m_f64MaximumValue;
			OpenViBE::float64 m_f64Attenuation;
			guchar * m_pRGBBuffer;
			OpenViBE::uint32 m_ui32RGBBufferWidth, m_ui32RGBBufferHeight;
			OpenViBE::uint32 m_ui32Rowstride;
		};
	};
};

#endif

