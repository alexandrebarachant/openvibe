#include "ovpCBottomTimeRuler.h"

#include <math.h>

#include <sstream>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace std;

#define convert_time(i) (float64)(i>>32) + (float64)((float64)(i&0xFFFFFFFF) / (float64)((uint64)1<<32))

//! Callback to redraw the bottom ruler
gboolean bottomRulerExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	//redraw the ruler
	CBottomTimeRuler * l_pBottomRuler = reinterpret_cast<CBottomTimeRuler*>(data);
	l_pBottomRuler->draw();

	//don't propagate this signal to the children if any
	return TRUE;
}

//! Called when the widget whose width is associated with the ruler is resized.
gboolean resizeBottomRulerCallback(GtkWidget *widget, GtkAllocation *allocation, gpointer data)
{
	CBottomTimeRuler * l_pBottomRuler = reinterpret_cast<CBottomTimeRuler*>(data);

	//resizes the bottom ruler
	gtk_widget_set_size_request(l_pBottomRuler->getWidget(), allocation->width, 20/*-1*/);

	return FALSE;
}

CBottomTimeRuler::CBottomTimeRuler(CBufferDatabase& pDatabase) :
	m_pDatabase(&pDatabase),
	m_ui64PixelsPerBottomRulerLabel(20)
{
	//creates the main drawing area
	m_pBottomRuler = gtk_drawing_area_new();

	gtk_widget_set_size_request(m_pBottomRuler, 0, 20);
	g_signal_connect_after(G_OBJECT(m_pBottomRuler), "expose_event", G_CALLBACK(bottomRulerExposeEventCallback), this);
}

CBottomTimeRuler::~CBottomTimeRuler()
{
}

/**
 * Draws the ruler by using the information from the database.
 * */
void CBottomTimeRuler::draw()
{
	//if the widget is invisible, no need to redraw it
	if(!GTK_WIDGET_VISIBLE(m_pBottomRuler))
	{
		return;
	}

	//gets the number of buffers to display
	uint64 l_ui64NumberOfBufferToDisplay = m_pDatabase->m_ui64NumberOfBufferToDisplay;

	if(m_pDatabase->m_pDimmensionSizes[1] == 1 && l_ui64NumberOfBufferToDisplay != 1)
	{
		// l_ui64NumberOfBufferToDisplay--;
	}

	//gets the widget's size
	gint l_iBottomRulerWidth;
	gint l_iBottomRulerHeight;
	gdk_drawable_get_size(m_pBottomRuler->window, &l_iBottomRulerWidth, &l_iBottomRulerHeight);

	//draw ruler base (horizontal line)
	gdk_draw_line(m_pBottomRuler->window, m_pBottomRuler->style->fg_gc[GTK_WIDGET_STATE (m_pBottomRuler)], 0, 0, l_iBottomRulerWidth, 0);

	//computes the starting and ending time (in ms) of the displayed data
	float64 l_f64StartTime = 0;
	if(m_pDatabase->m_oStartTime.size() != 0)
	{
		l_f64StartTime = convert_time(m_pDatabase->m_oStartTime[0]);
	}

	//in ms
	float64 l_f64IntervalWidth = convert_time(l_ui64NumberOfBufferToDisplay * m_pDatabase->m_ui64BufferDuration);

	float64 l_f64EndTime = l_f64StartTime + l_f64IntervalWidth;

	float64 l_f64ValueStep;
	float64 l_f64BaseValue ;

	//computes the step of the values displayed on the ruler
	float64 l_f64NearestSmallerPowerOf10 = static_cast<float64>(pow(10, floor(log10(l_f64IntervalWidth))));

	uint64 l_ui64MaxNumberOfLabels = (uint64)(l_iBottomRulerWidth / m_ui64PixelsPerBottomRulerLabel);

	if( (uint64)floor(l_f64IntervalWidth / l_f64NearestSmallerPowerOf10) > l_ui64MaxNumberOfLabels )
	{
		l_f64ValueStep = 2 * l_f64NearestSmallerPowerOf10;
	}
	else if( (uint64)floor(l_f64IntervalWidth / l_f64NearestSmallerPowerOf10) < l_ui64MaxNumberOfLabels/2 )
	{
		l_f64ValueStep = l_f64NearestSmallerPowerOf10 / 2;
	}

	else
	{
		l_f64ValueStep = l_f64NearestSmallerPowerOf10;
	}

	//recalculates base value of the step
	l_f64BaseValue = l_f64ValueStep * floor(l_f64StartTime/l_f64ValueStep);

	//used to compute the X position of the labels
	float64 l_f64WidthPerBuffer = static_cast<float64>(l_iBottomRulerWidth) / static_cast<float64>(l_ui64NumberOfBufferToDisplay);
	//X position of the first label (if there are less buffers than needed)
	int64 l_i64BaseX = static_cast<int64>(floor(l_iBottomRulerWidth - ( m_pDatabase->m_oSampleBuffers.size() * l_f64WidthPerBuffer)));

	if(l_i64BaseX<0 || l_i64BaseX < l_f64WidthPerBuffer)
	{
		l_i64BaseX = 0;
	}

	stringstream l_oTimeLabel;

	for(float64 i=l_f64BaseValue ; i<static_cast<float64>(0.5+l_f64EndTime) ; i+=l_f64ValueStep)
	{
		//clears the stringstream
		l_oTimeLabel.str("");

		//compute the position of the label
		gint l_iTextX = static_cast<gint>(l_i64BaseX + ((i - l_f64StartTime)*(((float64)l_iBottomRulerWidth)/l_f64IntervalWidth))) ;

		l_oTimeLabel<<i;

		PangoLayout * l_pText = gtk_widget_create_pango_layout(m_pBottomRuler, l_oTimeLabel.str().c_str());

		int l_iTextWidth;
		pango_layout_get_pixel_size(l_pText, &l_iTextWidth, NULL);

		//if the width allocated per label becomes too small compared to the effective width of the label
		if( static_cast<uint64>(l_iTextWidth) >= m_ui64PixelsPerBottomRulerLabel-20)
		{
			//increases the allocated width per label
			m_ui64PixelsPerBottomRulerLabel = l_iTextWidth + 30;
		}

		//display it
		gdk_draw_layout(m_pBottomRuler->window, m_pBottomRuler->style->fg_gc[GTK_WIDGET_STATE (m_pBottomRuler)],
				l_iTextX, 4, l_pText);

		//draw a small line above it
		gdk_draw_line(m_pBottomRuler->window, m_pBottomRuler->style->fg_gc[GTK_WIDGET_STATE (m_pBottomRuler)], l_iTextX, 0, l_iTextX, 3);

	}
}

/**
 * Shows/Hides the ruler.
 * \param bActive Whether the ruler must be displayed or not.
 * */
void CBottomTimeRuler::toggle(OpenViBE::boolean bActive)
{
	if(bActive)
	{
		gtk_widget_show(m_pBottomRuler);
	}
	else
	{
		gtk_widget_hide(m_pBottomRuler);
	}
}

/**
 * Associates another widget to this ruler.
 * The ruler's width will be resized when the other widget's width changes.
 * \param pWidget The widget you want to associate the ruler with.
 * */
void CBottomTimeRuler::linkWidthToWidget(GtkWidget * pWidget)
{
	//adds a callback to the widget for the size-allocate signal
	g_signal_connect(G_OBJECT(pWidget), "size-allocate", G_CALLBACK(resizeBottomRulerCallback), this);
}

