#ifndef __OpenViBEPlugins_SimpleVisualisation_CChannelDisplayWidget_H__
#define __OpenViBEPlugins_SimpleVisualisation_CChannelDisplayWidget_H__

#include "../ovp_defines.h"
#include "ovpCSignalDisplayLeftRuler.h"

#include <glib.h>
#include <glib/gprintf.h>
#include <glib-object.h>
#include <gtk/gtk.h>

#include <openvibe/ov_all.h>

#include <openvibe-toolkit/ovtk_all.h>

#include <memory.h>
#include <math.h>

#include <vector>

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{

	class CSignalDisplayView;
	class CBufferDatabase;

	class CChannelDisplayPoint
	{
		public:
		OpenViBE::float64 m_f64X;
		OpenViBE::float64 m_f64Y;
	};

	class CChannelDisplay
	{
	public:

		//! The table containing the signal display and rulers
		GtkWidget * m_pWidgetTable;
		//! The drawing area where the signal is to be drawn
		GtkWidget * m_pDrawingArea;

		CSignalDisplayLeftRuler m_oLeftRuler;

		//! The index list of the channels to display
		std::vector<OpenViBE::uint32> m_oChannelList;

		//! The "parent" view (which uses this widget)
		CSignalDisplayView * m_pParentDisplayView;

		//! The database from which the information are to be read
		CBufferDatabase * m_pDatabase;

		//Parameters needed to scale and translate the signal correctly in the drawing area
		OpenViBE::float64 m_f64ScaleX;
		OpenViBE::float64 m_f64ScaleY;
		OpenViBE::float64 m_f64TranslateX;
		OpenViBE::float64 m_f64TranslateY;

		//Parameters needed to scale and translate the signal correctly in the drawing area when zooming
		OpenViBE::float64 m_f64ZoomTranslateX;
		OpenViBE::float64 m_f64ZoomTranslateY;
		OpenViBE::float64 m_f64ZoomScaleX;
		OpenViBE::float64 m_f64ZoomScaleY;

		//! The zoom factor step
		const OpenViBE::float64 m_f64ZoomFactor;

		OpenViBEPlugins::SimpleVisualisation::EDisplayMode m_eCurrentSignalMode;

	public:
		CChannelDisplay();
		~CChannelDisplay();

		void init(GtkTable * pTable);

		void setParent(CSignalDisplayView * pDisplayView);

		GtkWidget * getDisplayWidget(){ return m_pDrawingArea; }

		void resetChannelList(){ m_oChannelList.clear(); }
		void addChannel(OpenViBE::uint32 ui32Channel){ m_oChannelList.push_back(ui32Channel); }

		/**
		* Draws the signal on the signal's drawing area.
		*/
		void drawSignal();

		/**
		* Shows or hides the left ruler.
		* \param bActive If true, shows the left ruler, else hides it.
		*/
		void toggleLeftRuler(OpenViBE::boolean bActive);

		/**
		* Computes the parameters necessary for the signal
		* to take up all the available space in the drawing area.
		*/
		void computeBestFit();

		void computeNormalSize();

		/**
		* Computes the parameters necessary for the signal
		* to be zoomed at the selected coordinates.
		* \param bZoomIn If true, the operation is a zoom In, if false it's a zoom Out.
		* \param f64XClick The X-coordinate of the center of the area we want to zoom in.
		* \param f64YClick The Y-coordinate of the center of the area we want to zoom in.
		*/
		void computeZoom(OpenViBE::boolean bZoomIn, OpenViBE::float64 f64XClick, OpenViBE::float64 f64YClick);

		/**
		* Depending on the zooming mode, updates (or not) the scaling/translation
		* parameters of the signals to reflect the latest changes brought by the last
		* arrived signal buffer.
		*/
		void updateSignalZoomParameters();

		/**
		 * Computes the list of points used to draw the lines (m_pParentDisplayView->m_pPoints) using the raw points list
		 * (m_pParentDisplayView->m_pRawPoints) and by cropping the lines when they go out of the window.
		 * \return The number of points to display.
		 */
		OpenViBE::uint64 cropCurve();
};

G_BEGIN_DECLS

#define CHANNEL_DISPLAY_TYPE            (channel_display_get_type ())

#define CHANNEL_DISPLAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), CHANNEL_DISPLAY_TYPE, ChannelDisplay))

#define CHANNEL_DISPLAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CHANNEL_DISPLAY_TYPE, ChannelDisplayClass))

#define IS_CHANNEL_DISPLAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHANNEL_DISPLAY_TYPE))

#define IS_CHANNEL_DISPLAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CHANNEL_DISPLAY_TYPE))

typedef struct _ChannelDisplayClass  ChannelDisplayClass;
typedef struct _ChannelDisplay       ChannelDisplay;

struct _ChannelDisplay
{
	GtkTable m_oTable;

	CChannelDisplay	* m_pChannelDisplay;
};

struct _ChannelDisplayClass
{
	GtkTableClass parent_class;

	void (* channel_display) (ChannelDisplay *channelDisplay);
};

GType          channel_display_get_type        (void);
GtkWidget*     channel_display_new             (void);
void	       channel_display_clear           (ChannelDisplay * channelDisplay);

G_END_DECLS

	}
}

#endif

