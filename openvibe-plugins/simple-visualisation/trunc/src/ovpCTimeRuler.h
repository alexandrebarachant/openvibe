#ifndef __OpenViBEPlugins_SimpleVisualisation_CTimeRuler_H__
#define __OpenViBEPlugins_SimpleVisualisation_CTimeRuler_H__

#include <openvibe/ov_all.h>
#include "ovpIStreamDatabase.h"
#include <gtk/gtk.h>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		/**
		 * Displays a time ruler.
		 * Uses information fetched from a stream database object.
		 */
		class CTimeRuler
		{
		public:
			/**
			 * \brief Constructor
			 * \param rStreamDatabase Object from which data is retrieved
			 */
			CTimeRuler(
				IStreamDatabase& rStreamDatabase);

			/**
			 * \brief Destructor
			 */
			~CTimeRuler();

			/**
			 * \brief Get widget handled by this object
			 * \return Gtk widget
			 */
			GtkWidget* getWidget() const { return m_pWidget; }

			/**
			 * \brief Toggle ruler on/off
			 * \param bActive Activation flag
			 */
			void toggle(OpenViBE::boolean bActive);

			/**
			 * \brief Draw ruler
			 */
			void draw();

			/**
			 * \brief Link ruler width to another widget's
			 * \param pWidget Widget whose width must be matched by this object
			 */
			void linkWidthToWidget(GtkWidget* pWidget);

		private:
			//! Ruler widget
			GtkWidget* m_pWidget;
			//! Database from which stream information is retrieved
			IStreamDatabase& m_rStreamDatabase;
			//! Size available per label along the ruler
			OpenViBE::uint64 m_ui64PixelsPerBottomRulerLabel;
		};
	}
}

#endif
