#ifndef __OpenViBEPlugins_SimpleVisualisation_CFrequencySpectrumChannelDisplay_H__
#define __OpenViBEPlugins_SimpleVisualisation_CFrequencySpectrumChannelDisplay_H__

#include "../ovpCBufferDatabase.h"

#include <openvibe/ov_all.h>
#include <gtk/gtk.h>
#include <iostream>
using namespace std;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		/**
		 * Used to display a vertical "amplitude" ruler.
		 * */
		class CFrequencySpectrumChannelDisplay
		{
				GtkWidget * m_pDisplay;

				GdkGC* m_pGraphicsContext;

				CBufferDatabase * m_pDatabase;

				OpenViBE::uint32 m_ui32Channel;

				OpenViBE::float64 m_f64MinimumValue;
				OpenViBE::float64 m_f64MaximumValue;
				OpenViBE::float64 m_f64Attenuation;

				guchar * m_pRGBBuffer;
				OpenViBE::uint32 m_ui32RGBBufferWidth;
				OpenViBE::uint32 m_ui32RGBBufferHeight;
				OpenViBE::uint32 m_ui32Rowstride;

				//GdkPixmap * m_pPixmap;
			public:
				CFrequencySpectrumChannelDisplay(OpenViBE::uint32 ui32Channel, CBufferDatabase& pDatabase);
				~CFrequencySpectrumChannelDisplay();

				//! returns the widget, so it can be added to the main interface
				GtkWidget * getWidget() const { return m_pDisplay; }

				//! toggles (show/hide)
				void toggle(OpenViBE::boolean bActive);

				//! draws the spectrum
				void redraw();

				//! computes the frequency spectrum
				void update();

				void resizeRGBBuffer(OpenViBE::uint32 ui32Width, OpenViBE::uint32 ui32Height);

				void drawBoxToBuffer(OpenViBE::uint32 ui32X, OpenViBE::uint32 ui32Y, OpenViBE::uint32 ui32Width, OpenViBE::uint32 ui32Height, OpenViBE::uint8 ui8Red, OpenViBE::uint8 ui8Green, OpenViBE::uint8 ui8Blue)
				{
					guchar * l_pLineBase = m_pRGBBuffer + (m_ui32Rowstride*ui32Y) + (ui32X*3);

					for(OpenViBE::uint32 j=0 ; j<ui32Height ; j++)
					{
						for(OpenViBE::uint32 i=0 ; i<(ui32Width*3) ; i+=3)
						{
							*(l_pLineBase + i) =  ui8Red;
							*(l_pLineBase + i + 1) =  ui8Green;
							*(l_pLineBase + i + 2) =  ui8Blue;
						}

						l_pLineBase+=(m_ui32Rowstride);
					}
				}

				void setMinMaxAttenuation(OpenViBE::float64 f64Attenuation) { m_f64Attenuation=f64Attenuation; }

		};

	};
};

#endif

