#ifndef __OpenViBEPlugins_SimpleVisualisation_CSignalDisplayLeftRuler_H__
#define __OpenViBEPlugins_SimpleVisualisation_CSignalDisplayLeftRuler_H__

#include <openvibe/ov_all.h>
#include <gtk/gtk.h>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		/**
		 * Used to display a vertical "amplitude" ruler.
		 * */
		class CSignalDisplayLeftRuler
		{
				GtkWidget * m_pLeftRuler;

				OpenViBE::float64 m_f64MaximumDisplayedValue;
				OpenViBE::float64 m_f64MinimumDisplayedValue;

				OpenViBE::uint64 m_ui64PixelsPerLeftRulerLabel;

			public:
				CSignalDisplayLeftRuler();
				~CSignalDisplayLeftRuler();

				void setMinMax(OpenViBE::float64 f64Min, OpenViBE::float64 f64Max)
				{
					m_f64MaximumDisplayedValue = f64Max;
					m_f64MinimumDisplayedValue = f64Min;

					//redraw the ruler
					gdk_window_invalidate_rect(m_pLeftRuler->window, NULL, true);
				}

				//! returns the widget, so it can be added to the main interface
				GtkWidget * getWidget() const { return m_pLeftRuler; }

				//! toggles (show/hide) the ruler
				void toggle(OpenViBE::boolean bActive);

				//! draws the ruler
				void draw();

				//! link the ruler's height to another widget's
				void linkHeightToWidget(GtkWidget * pWidget);
		};

	};
};

#endif

