#include <math.h>
#include <sstream>

#include "ovpCTimeRuler.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace std;

#define convert_time(i) (float64)(i>>32) + (float64)((float64)(i&0xFFFFFFFF) / (float64)((uint64)1<<32))

//callbacks
gboolean timeRulerExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data);
gboolean timeRulerResizeCallback(GtkWidget *widget, GtkAllocation *allocation, gpointer data);

CTimeRuler::CTimeRuler(IStreamDatabase& rStreamDatabase) :
	m_pWidget(NULL),
	m_rStreamDatabase(rStreamDatabase),
	m_ui64PixelsPerBottomRulerLabel(20)
{	
	m_pWidget = gtk_drawing_area_new();

	gtk_widget_set_size_request(m_pWidget, 0, 20);
	
	g_signal_connect_after(G_OBJECT(m_pWidget), "expose_event", G_CALLBACK(timeRulerExposeEventCallback), this);
}

CTimeRuler::~CTimeRuler()
{
}

void CTimeRuler::draw()
{
	//if the widget is invisible, no need to redraw it
	if(!GTK_WIDGET_VISIBLE(m_pWidget))
	{
		return;
	}

	//get number of buffers to display
	uint64 l_ui64NumberOfBufferToDisplay = m_rStreamDatabase.getMaxDisplayedBufferCount();

	//get widget size
	gint l_iBottomRulerWidth;
	gint l_iBottomRulerHeight;
	gdk_drawable_get_size(m_pWidget->window, &l_iBottomRulerWidth, &l_iBottomRulerHeight);

	//draw ruler base (horizontal line)
	gdk_draw_line(m_pWidget->window, m_pWidget->style->fg_gc[GTK_WIDGET_STATE (m_pWidget)], 0, 0, l_iBottomRulerWidth, 0);

	//compute starting and ending time (in ms) of displayed data
	float64 l_f64StartTime = 0;
	if(m_rStreamDatabase.getStartTime(0) != 0)
	{
		l_f64StartTime = convert_time(m_rStreamDatabase.getStartTime(0));
	}

	//displayed time interval (in ms)
	float64 l_f64IntervalWidth = convert_time(l_ui64NumberOfBufferToDisplay * m_rStreamDatabase.getBufferDuration());

	float64 l_f64EndTime = l_f64StartTime + l_f64IntervalWidth;

	float64 l_f64ValueStep;
	float64 l_f64BaseValue;

	//compute step between two values displayed on the ruler
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

	//recompute step base value
	l_f64BaseValue = l_f64ValueStep * floor(l_f64StartTime/l_f64ValueStep);

	//used to compute the X position of the labels
	float64 l_f64WidthPerBuffer = static_cast<float64>(l_iBottomRulerWidth) / static_cast<float64>(l_ui64NumberOfBufferToDisplay);
	//X position of the first label (if there are less buffers than needed)
	int64 l_i64BaseX = static_cast<int64>(floor(l_iBottomRulerWidth - ( m_rStreamDatabase.getBufferCount() * l_f64WidthPerBuffer)));

	if(l_i64BaseX<0 || l_i64BaseX < l_f64WidthPerBuffer)
	{
		l_i64BaseX = 0;
	}

	stringstream l_oTimeLabel;

	for(float64 i=l_f64BaseValue; i<static_cast<float64>(0.5+l_f64EndTime); i+=l_f64ValueStep)
	{
		//clear stringstream
		l_oTimeLabel.str("");

		//compute label position
		gint l_iTextX = static_cast<gint>(l_i64BaseX + ((i - l_f64StartTime)*(((float64)l_iBottomRulerWidth)/l_f64IntervalWidth)));

		l_oTimeLabel<<i;

		PangoLayout * l_pText = gtk_widget_create_pango_layout(m_pWidget, l_oTimeLabel.str().c_str());

		int l_iTextWidth;
		pango_layout_get_pixel_size(l_pText, &l_iTextWidth, NULL);

		//if the width allocated per label becomes too small compared to the effective width of the label
		if( static_cast<uint64>(l_iTextWidth) >= m_ui64PixelsPerBottomRulerLabel-20)
		{
			//increases the allocated width per label
			m_ui64PixelsPerBottomRulerLabel = l_iTextWidth + 30;
		}

		//display it
		gdk_draw_layout(m_pWidget->window, m_pWidget->style->fg_gc[GTK_WIDGET_STATE (m_pWidget)],	l_iTextX, 4, l_pText);

		//draw a small line above it
		gdk_draw_line(m_pWidget->window, m_pWidget->style->fg_gc[GTK_WIDGET_STATE (m_pWidget)], l_iTextX, 0, l_iTextX, 3);
	}
}

void CTimeRuler::toggle(boolean bActive)
{
	if(bActive)
	{
		gtk_widget_show(m_pWidget);
	}
	else
	{
		gtk_widget_hide(m_pWidget);
	}
}

void CTimeRuler::linkWidthToWidget(GtkWidget* pWidget)
{
	//add a callback to the widget for the size-allocate signal
	g_signal_connect(G_OBJECT(pWidget), "size-allocate", G_CALLBACK(timeRulerResizeCallback), this);
}

//! Callback to redraw the bottom ruler
gboolean timeRulerExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	//redraw the ruler
	CTimeRuler* l_pTimeRuler = reinterpret_cast<CTimeRuler*>(data);
	l_pTimeRuler->draw();

	//don't propagate this signal to the children if any
	return TRUE;
}

//! Called when the widget whose width is associated with the ruler is resized.
gboolean timeRulerResizeCallback(GtkWidget *widget, GtkAllocation *allocation, gpointer data)
{
	CTimeRuler* l_pTimeRuler = reinterpret_cast<CTimeRuler*>(data);	
	gtk_widget_set_size_request(l_pTimeRuler->getWidget(), allocation->width, 20/*-1*/);
	
	return FALSE;
}

