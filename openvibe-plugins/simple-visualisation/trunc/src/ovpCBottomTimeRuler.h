#ifndef __OpenViBEPlugins_SimpleVisualisation_CBottomTimeRuler_H__
#define __OpenViBEPlugins_SimpleVisualisation_CBottomTimeRuler_H__

#include <openvibe/ov_all.h>
#include "ovpCBufferDatabase.h"
#include <gtk/gtk.h>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		class CBufferDatabase;

		/**
		 * Used to display an horizontal temporal ruler.
		 * Uses information fetched from a signal database object.
		 * */
		class CBottomTimeRuler
		{
				GtkWidget * m_pBottomRuler;

				CBufferDatabase * m_pDatabase;

				OpenViBE::uint64 m_ui64PixelsPerBottomRulerLabel;

			public:
				CBottomTimeRuler(CBufferDatabase& pDatabase);
				~CBottomTimeRuler();

				//! returns the widget, so it can be added to the main interface
				GtkWidget * getWidget() const { return m_pBottomRuler; }

				//! toggles (show/hide) the ruler
				void toggle(OpenViBE::boolean bActive);

				//! draws the ruler
				void draw();

				//! link the ruler's width to another widget's
				void linkWidthToWidget(GtkWidget * pWidget);
		};

	};
};

#endif

