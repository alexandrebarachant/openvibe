#include "ovpCChannelDisplayWidget.h"

#include "ovpCSignalDisplayView.h"
#include <cfloat>

#include <sstream>
#include <iostream>

using namespace std;

using namespace OpenViBE;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{

enum {
  LAST_SIGNAL
};

static void channel_display_class_init          (ChannelDisplayClass * klass);
static void channel_display_init                (ChannelDisplay      * pChannelDisplay);

GType channel_display_get_type(void)
{
  static GType channel_display_type = 0;

  if (!channel_display_type)
    {
      static const GTypeInfo channel_display_info =
      {
	sizeof (ChannelDisplayClass),
	NULL, /* base_init */
        NULL, /* base_finalize */
	(GClassInitFunc) channel_display_class_init,
        NULL, /* class_finalize */
	NULL, /* class_data */
        sizeof (ChannelDisplay),
	0,
	(GInstanceInitFunc) channel_display_init,
      };

      channel_display_type = g_type_register_static(GTK_TYPE_TABLE, "ChannelDisplay", &channel_display_info, (GTypeFlags)0);
    }

  return channel_display_type;
}



static void channel_display_init(ChannelDisplay * pChannelDisplay)
{
	pChannelDisplay->m_pChannelDisplay = new CChannelDisplay();
	pChannelDisplay->m_pChannelDisplay->init(GTK_TABLE(pChannelDisplay));
  //init object
}

GtkWidget* channel_display_new()
{
  return GTK_WIDGET(g_object_new(channel_display_get_type(), NULL));
}

static void channel_display_destroy (GtkObject * object)
{
	ChannelDisplayClass * klass;

	g_return_if_fail (object != NULL);
	g_return_if_fail (IS_CHANNEL_DISPLAY(object));

	ChannelDisplay * l_pObject = CHANNEL_DISPLAY(object);

	delete l_pObject->m_pChannelDisplay;
	l_pObject->m_pChannelDisplay = NULL;

	klass = reinterpret_cast<ChannelDisplayClass *>(gtk_type_class(gtk_widget_get_type ()));

	if (GTK_OBJECT_CLASS (klass)->destroy) {
		(* GTK_OBJECT_CLASS (klass)->destroy) (object);
	}
}

static void channel_display_class_init (ChannelDisplayClass *klass)
{
	GtkObjectClass * object_class;

	object_class = (GtkObjectClass *) klass;

	object_class->destroy = channel_display_destroy;
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

//! Callback used when the signal's drawing area needs to be redrawn (expose event)
gboolean drawingAreaExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	//redraw the signals
	CChannelDisplay * m_pChannelDisplay = reinterpret_cast<CChannelDisplay*>(data);
	m_pChannelDisplay->drawSignal();

	//don't propagate this signal to the children if any
	return TRUE;
}

//! Callback used when the user clicked on the signal's drawing area
void drawingAreaClickedEventCallback(GtkWidget *widget, GdkEventButton *event, gpointer data)
{

	CChannelDisplay * m_pChannelDisplay = reinterpret_cast<CChannelDisplay*>(data);
	OpenViBE::boolean l_bZoomChanged = false;

	uint32 l_ui32CursorMode = m_pChannelDisplay->m_pParentDisplayView->m_ui32CurrentCursorMode;

	//if it's a right click and the action is a zoom in or a zoom out
	//then do the opposite action
	if(event->button == 3)
	{
		if(l_ui32CursorMode==1)
		{
			l_ui32CursorMode = 2;
		}
		else if(l_ui32CursorMode==2)
		{
			l_ui32CursorMode = 1;
		}
	}

	//depending on the cursor mode, change the zoom level
	switch(l_ui32CursorMode)
	{
		case 1 : //ZoomIn
			m_pChannelDisplay->computeZoom(true, event->x, event->y);
			l_bZoomChanged = true;
			break;

		case 2 : //ZoomOut
			if(m_pChannelDisplay->m_f64ZoomScaleX != 1.0)
			{
				m_pChannelDisplay->computeZoom(false, event->x, event->y);
				l_bZoomChanged = true;
			}
			break;

		case 3 : //Best fit
			m_pChannelDisplay->computeBestFit();
			l_bZoomChanged = true;
			break;

		case 4 : //Normal size
			m_pChannelDisplay->computeNormalSize();
			l_bZoomChanged = true;
			break;
	}

	//keep track of the current mode
	m_pChannelDisplay->m_ui32CurrentSignalMode = l_ui32CursorMode;

	//if the zoom level has changed, redraw the signal and left ruler
	if(l_bZoomChanged)
	{
		gdk_window_invalidate_rect(GTK_WIDGET(m_pChannelDisplay->m_pDrawingArea)->window,
					   NULL,
					   true);
		
		gdk_window_invalidate_rect(GTK_WIDGET(m_pChannelDisplay->m_pLeftRuler)->window,
					   NULL,
					   true);
	}

}

//! Callback for when the cursor enters the signal's drawing area
void drawingAreaEnterEventCallback(GtkWidget *widget, GdkEventCrossing *event, gpointer data)
{
	CChannelDisplay * m_pChannelDisplay = reinterpret_cast<CChannelDisplay*>(data);

	//change the cursor to the one corresponding to the cursor mode
	gdk_window_set_cursor(widget->window, m_pChannelDisplay->m_pParentDisplayView->m_pCursor[m_pChannelDisplay->m_pParentDisplayView->m_ui32CurrentCursorMode]);

}

//! Callback for when the cursor leaves the signal's drawing area
void drawingAreaLeaveEventCallback(GtkWidget *widget, GdkEventCrossing *event, gpointer data)
{
	CChannelDisplay * m_pChannelDisplay = reinterpret_cast<CChannelDisplay*>(data);

	//change the cursor back to the normal one
	gdk_window_set_cursor(widget->window, m_pChannelDisplay->m_pParentDisplayView->m_pCursor[0]);
}

//! Callback to redraw the left ruler
gboolean leftRulerExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	//redraw the ruler
	CChannelDisplay * m_pChannelDisplay = reinterpret_cast<CChannelDisplay*>(data);
	m_pChannelDisplay->drawLeftRuler();

	//don't propagate this signal to the children if any
	return TRUE;
}

//! Callback to redraw the bottom ruler
gboolean bottomRulerExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	//redraw the ruler
	CChannelDisplay * m_pChannelDisplay = reinterpret_cast<CChannelDisplay*>(data);
	m_pChannelDisplay->drawBottomRuler();

	//don't propagate this signal to the children if any
	return TRUE;
}


CChannelDisplay::CChannelDisplay() :
	m_pWidgetTable(NULL),
	m_pDrawingArea(NULL),
	m_pLeftRuler(NULL),
	m_pBottomRuler(NULL),
	m_pDatabase(NULL),
	m_f64ScaleX(1),
	m_f64ScaleY(1),
	m_f64TranslateX(0),
	m_f64TranslateY(0),
	m_f64ZoomTranslateX(0),
	m_f64ZoomTranslateY(0),
	m_f64ZoomScaleX(1),
	m_f64ZoomScaleY(1),
	m_f64ZoomFactor(1.5),
	m_ui32CurrentSignalMode(4),
	m_ui64PixelsPerBottomRulerLabel(20)
{

}


CChannelDisplay::~CChannelDisplay()
{
}

void CChannelDisplay::setParent(CSignalDisplayView * pDisplayView)
{
	m_pParentDisplayView=pDisplayView;
	m_pDatabase=pDisplayView->m_pSignalDisplayDatabase;
}

void CChannelDisplay::init(GtkTable * pTable)
{
	m_pWidgetTable = GTK_WIDGET(pTable);

 	gtk_table_resize(GTK_TABLE(m_pWidgetTable), 2, 2);
 	gtk_table_set_homogeneous(GTK_TABLE(m_pWidgetTable), FALSE);

	//creates the drawing area
	m_pDrawingArea = gtk_drawing_area_new();

	//creates the left ruler
	m_pLeftRuler = gtk_drawing_area_new();
	gtk_widget_set_size_request(m_pLeftRuler, 50, 80);

	//creates the bottom ruler
	m_pBottomRuler = gtk_drawing_area_new();
	gtk_widget_set_size_request(m_pBottomRuler, 600, 20);

	//adds them to the table
	gtk_table_attach(GTK_TABLE(m_pWidgetTable), m_pDrawingArea,
			1, 2, 0, 1,
			static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL),
			0, 0);

	gtk_table_attach(GTK_TABLE(m_pWidgetTable), m_pLeftRuler,
			0, 1, 0, 1,
			GTK_FILL, GTK_FILL,
			0, 0);

	gtk_table_attach(GTK_TABLE(m_pWidgetTable), m_pBottomRuler,
			1, 2, 1, 2,
			GTK_FILL, GTK_FILL,
			0, 0);

	//connects the signals
	g_signal_connect_after(G_OBJECT(m_pDrawingArea), "expose_event", G_CALLBACK(drawingAreaExposeEventCallback), this);

	gtk_widget_add_events(GTK_WIDGET(m_pDrawingArea), GDK_BUTTON_PRESS_MASK);
	gtk_widget_add_events(GTK_WIDGET(m_pDrawingArea), GDK_ENTER_NOTIFY_MASK);
	gtk_widget_add_events(GTK_WIDGET(m_pDrawingArea), GDK_LEAVE_NOTIFY_MASK);

	g_signal_connect_after(G_OBJECT(m_pDrawingArea), "button-press-event", G_CALLBACK(drawingAreaClickedEventCallback), this);

	g_signal_connect_after(G_OBJECT(m_pDrawingArea), "enter-notify-event", G_CALLBACK(drawingAreaEnterEventCallback), this);

	g_signal_connect_after(G_OBJECT(m_pDrawingArea), "leave-notify-event", G_CALLBACK(drawingAreaLeaveEventCallback), this);

	g_signal_connect_after(G_OBJECT(m_pLeftRuler), "expose_event", G_CALLBACK(leftRulerExposeEventCallback), this);
	g_signal_connect_after(G_OBJECT(m_pBottomRuler), "expose_event", G_CALLBACK(bottomRulerExposeEventCallback), this);
}


uint64 CChannelDisplay::cropCurve()
{
	GdkPoint l_oPoint;
	
	gint l_iWidth = 0;
	gint l_iHeight = 0;

	vector<CChannelDisplayPoint>& l_vCurvePoints = m_pParentDisplayView->m_pRawPoints;
	
	//gets the size of the drawing area
	gdk_drawable_get_size(m_pDrawingArea->window, &l_iWidth, &l_iHeight);

	//clears the vector of the points to draw
	m_pParentDisplayView->m_pPoints.clear();
	
	//for each couple of successive points
	for(size_t i=0 ; i<(m_pDatabase->m_oSampleBuffers.size() *  m_pDatabase->m_ui32SamplesPerBuffer) -1 ; i++)
	{
		//get the two points coordinates
		float64 l_f64X0 = l_vCurvePoints[i].m_f64X;
		float64 l_f64Y0 = l_vCurvePoints[i].m_f64Y;
		float64 l_f64X1 = l_vCurvePoints[i+1].m_f64X;
		float64 l_f64Y1 = l_vCurvePoints[i+1].m_f64Y;
		
		//if one of the point is out of the drawing area
		if(l_f64Y0<0 || l_f64Y0>=l_iHeight || l_f64Y1<0 || l_f64Y1>=l_iHeight)
		{
			//computes the line's coefficients
			float64 l_f64A = (l_f64Y1-l_f64Y0)/(l_f64X1-l_f64X0);
			float64 l_f64B = l_f64Y0 - (l_f64A*l_f64X0);
			
			//if the first point is out of the window
			if(l_f64Y0<0)
			{
				//computes its X-coordinate with the minimum Y
				l_oPoint.x = static_cast<gint>(-l_f64B/l_f64A);
				//we take -1 and not 0, this way, the line between the two successive intersect points won't be drawn
				l_oPoint.y = static_cast<gint>(-1);
				//adds it to the vector
				m_pParentDisplayView->m_pPoints.push_back(l_oPoint);
				
			}
			else if(l_f64Y0>=l_iHeight)
			{
				//same with the maximum Y
				l_oPoint.x = static_cast<gint>((l_iHeight-l_f64B)/l_f64A);
				l_oPoint.y = l_iHeight;
				m_pParentDisplayView->m_pPoints.push_back(l_oPoint);
				
			}
			//if it is inside, keep its current coordinates
			else
			{
				l_oPoint.x = static_cast<gint>(l_f64X0);
				l_oPoint.y = static_cast<gint>(l_f64Y0);
				m_pParentDisplayView->m_pPoints.push_back(l_oPoint);
			}
			
			//if the second point is out of the window, computes its intersect point and adds it
			if(l_f64Y1<0)
			{
				l_oPoint.x = static_cast<gint>(-l_f64B/l_f64A);
				l_oPoint.y = static_cast<gint>(-1);
				m_pParentDisplayView->m_pPoints.push_back(l_oPoint);
				
			}
			else if(l_f64Y1>=l_iHeight)
			{
				l_oPoint.x = static_cast<gint>((l_iHeight-l_f64B)/l_f64A);
				l_oPoint.y = static_cast<gint>(l_iHeight) ;
				m_pParentDisplayView->m_pPoints.push_back(l_oPoint);
			}
		}
		//keep the first point
		else
		{
			l_oPoint.x = static_cast<gint>(l_f64X0);
			l_oPoint.y = static_cast<gint>(l_f64Y0);
			m_pParentDisplayView->m_pPoints.push_back(l_oPoint);

			//Adds the last point
			if(i == (m_pDatabase->m_oSampleBuffers.size() *  m_pDatabase->m_ui32SamplesPerBuffer) -2)
			{
				l_oPoint.x = static_cast<gint>(l_f64X1);
				l_oPoint.y = static_cast<gint>(l_f64Y1);
				m_pParentDisplayView->m_pPoints.push_back(l_oPoint);
			}
			
		}
		
	}

	//return the number of points to draw
	return m_pParentDisplayView->m_pPoints.size();
}

void CChannelDisplay::drawSignal()
{

	if(!m_pDatabase || m_pDatabase->m_oSampleBuffers.size()==0)
	{
		return;
	}

	gint l_iWidth = 0;
	gint l_iHeight = 0;

	//gets the size of the drawing area
	gdk_drawable_get_size(m_pDrawingArea->window, &l_iWidth, &l_iHeight);


	uint64 l_ui64NumberOfBufferToDisplay = m_pDatabase->m_ui64NumberOfBufferToDisplay;
	if(m_pDatabase->m_ui32SamplesPerBuffer == 1 && l_ui64NumberOfBufferToDisplay != 1)
	{
		l_ui64NumberOfBufferToDisplay--;
	}

	float64 l_f64WidthPerBuffer = static_cast<float64>(l_iWidth) / static_cast<float64>(l_ui64NumberOfBufferToDisplay);

	int64 l_i64BaseX = static_cast<int64>(floor(l_iWidth - ( m_pDatabase->m_oSampleBuffers.size() * l_f64WidthPerBuffer)));

	if(l_i64BaseX<0 || l_i64BaseX < l_f64WidthPerBuffer)
	{
		l_i64BaseX = 0;
	}

	float64 l_f64PointStep = 0;
	
	//Computes the horizontal step of displayed points
	if( (m_pDatabase->m_ui32SamplesPerBuffer*l_ui64NumberOfBufferToDisplay) - 1 != 0 )
	{
		l_f64PointStep = static_cast<float64>(l_iWidth) / ( (m_pDatabase->m_ui32SamplesPerBuffer*l_ui64NumberOfBufferToDisplay) - 1);
	}

	for(size_t k=0 ; k<m_oChannelList.size() ; k++)
	{
		for(size_t j=0 ; j<m_pDatabase->m_oSampleBuffers.size() ; j++)
		{
			//gets a pointer to this channels' samples data in the current buffer
			const float64 * l_pCurrentChannelSampleBuffer = (m_pDatabase->m_oSampleBuffers)[j] + (m_oChannelList[k] * (m_pDatabase->m_ui32SamplesPerBuffer));

			//calculates the coordinates of the point in the new base
			for(uint32 i = 0 ; i<m_pDatabase->m_ui32SamplesPerBuffer ; i++)
			{
				size_t l_ui64PointIndex = (j * m_pDatabase->m_ui32SamplesPerBuffer)+i;

				(m_pParentDisplayView->m_pRawPoints)[l_ui64PointIndex].m_f64Y = (m_f64ScaleY * m_f64ZoomScaleY * l_iHeight * (m_f64TranslateY - l_pCurrentChannelSampleBuffer[i]) ) +  ((l_iHeight*m_f64ZoomScaleY)/2)  - (m_f64ZoomTranslateY * m_f64ZoomScaleY);

				(m_pParentDisplayView->m_pRawPoints)[l_ui64PointIndex].m_f64X = ((l_i64BaseX + (j*l_f64WidthPerBuffer) + (i*l_f64PointStep) - m_f64TranslateX ) * m_f64ScaleX) ;
			}
		}

		uint64 l_ui64NumberOfPointsToDisplay = cropCurve();

		if(l_ui64NumberOfPointsToDisplay != 0)
		{
			//draw all the points and link them
			gdk_draw_lines(m_pDrawingArea->window, m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], &m_pParentDisplayView->m_pPoints[0], (gint)l_ui64NumberOfPointsToDisplay);
		}

	}

	//switch to dashed line
	gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 1, GDK_LINE_ON_OFF_DASH, GDK_CAP_BUTT, GDK_JOIN_BEVEL);

	gint l_iZeroY = static_cast<gint>(
			(m_f64ScaleY * m_f64ZoomScaleY * l_iHeight * (m_f64TranslateY - 0) ) +  ((l_iHeight*m_f64ZoomScaleY)/2) - (m_f64ZoomTranslateY* m_f64ZoomScaleY));

	gdk_draw_line(m_pDrawingArea->window, m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 0, l_iZeroY, l_iWidth ,l_iZeroY);

	//switch back to normal line
	gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 1, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);
}

void CChannelDisplay::drawLeftRuler()
{
	if(!GTK_WIDGET_VISIBLE(m_pLeftRuler))
	{
		return;
	}

	gint l_iLeftRulerWidth;
	gint l_iLeftRulerHeight;
	gdk_drawable_get_size(m_pLeftRuler->window, &l_iLeftRulerWidth, &l_iLeftRulerHeight);
	
	//draw ruler base (vertical line)
	gdk_draw_line(m_pLeftRuler->window, m_pLeftRuler->style->fg_gc[GTK_WIDGET_STATE (m_pLeftRuler)],  l_iLeftRulerWidth-1, 0, l_iLeftRulerWidth-1, l_iLeftRulerHeight);

	//computes the step in values for the ruler

	//maximum and minimum signal values that will be displayed with this zoom level
	float64 l_f64MaximumDisplayedValue = m_f64TranslateY - ( (0 - ((l_iLeftRulerHeight*m_f64ZoomScaleY)/2) + (m_f64ZoomTranslateY* m_f64ZoomScaleY)) / (m_f64ScaleY * m_f64ZoomScaleY * l_iLeftRulerHeight) );

	float64 l_f64MinimumDisplayedValue = m_f64TranslateY - ( (l_iLeftRulerHeight - ((l_iLeftRulerHeight*m_f64ZoomScaleY)/2) + (m_f64ZoomTranslateY* m_f64ZoomScaleY)) / (m_f64ScaleY * m_f64ZoomScaleY * l_iLeftRulerHeight) );

	float64 l_f64IntervalWidth = l_f64MaximumDisplayedValue-l_f64MinimumDisplayedValue;
	float64 l_f64ValueStep;
	float64 l_f64BaseValue ;

	//if the signal is not constant
	if(l_f64IntervalWidth != 0)
	{
		//computes the step
		float64 l_f64NearestSmallerPowerOf10 = static_cast<float64>(pow(10, floor(log10(l_f64IntervalWidth))));


		if( (l_f64IntervalWidth / l_f64NearestSmallerPowerOf10) > 8 )
		{
			l_f64ValueStep = 5 * l_f64NearestSmallerPowerOf10;
		}
		else if( (l_f64IntervalWidth / l_f64NearestSmallerPowerOf10) > 4 )
		{
			l_f64ValueStep = 2 * l_f64NearestSmallerPowerOf10;
		}
		else if ((l_f64IntervalWidth / l_f64NearestSmallerPowerOf10) > 2 )
		{
			l_f64ValueStep = l_f64NearestSmallerPowerOf10;
		}
		else
		{
			l_f64ValueStep = l_f64NearestSmallerPowerOf10/2;
		}
		
		//recalculate base value of the step
		l_f64BaseValue = l_f64ValueStep *floor(l_f64MinimumDisplayedValue/l_f64ValueStep);
	}
	else
	{
		l_f64ValueStep = 1;
		l_f64BaseValue = floor(l_f64MinimumDisplayedValue - 0.5); 
	}


	int l_iTextW;
	int l_iTextH;

	//if the step is too small, it causes problems, so don't display anything and return
	if(l_f64ValueStep < 0.5e-5)
	{
		return;
	}

	for(float64 i=l_f64BaseValue ; i<static_cast<float64>(0.5+l_f64MaximumDisplayedValue) ; i+=l_f64ValueStep)
	{
		gchar l_pValueLabel[40];

		//computes the coordinate of the current label
		gint l_iTextY = static_cast<gint>(
				(m_f64ScaleY * m_f64ZoomScaleY * l_iLeftRulerHeight * (m_f64TranslateY - i ) ) +  ((l_iLeftRulerHeight*m_f64ZoomScaleY)/2) - (m_f64ZoomTranslateY* m_f64ZoomScaleY));

		if(l_iTextY>=0 && l_iTextY<=l_iLeftRulerHeight)
		{
			
			//if the current value is (almost) 0, displays 0
			if(i < 0.5e-10 && i> -0.5e-10)
			{
				g_sprintf(l_pValueLabel,"0");
			}
			else
			{
				g_sprintf(l_pValueLabel, "%g", i);
			}

			PangoLayout * l_pText = gtk_widget_create_pango_layout(m_pLeftRuler, l_pValueLabel);
			pango_layout_set_width(l_pText, 28);
			pango_layout_set_justify(l_pText, PANGO_ALIGN_RIGHT);
	
			pango_layout_get_pixel_size(l_pText, &l_iTextW, &l_iTextH);
	
			gdk_draw_layout(m_pLeftRuler->window, m_pLeftRuler->style->fg_gc[GTK_WIDGET_STATE (m_pLeftRuler)],
					0, l_iTextY-(l_iTextH/2), l_pText);

			gdk_draw_line(m_pLeftRuler->window, m_pLeftRuler->style->fg_gc[GTK_WIDGET_STATE (m_pLeftRuler)], l_iLeftRulerWidth-2, l_iTextY, l_iLeftRulerWidth, l_iTextY);

		}
	}

}

void CChannelDisplay::drawBottomRuler()
{
	//if the widget is invisible, no need to redraw it
	if(!GTK_WIDGET_VISIBLE(m_pBottomRuler))
	{
		return;
	}

	uint64 l_ui64NumberOfBufferToDisplay = m_pDatabase->m_ui64NumberOfBufferToDisplay;

	if(m_pDatabase->m_ui32SamplesPerBuffer == 1 && l_ui64NumberOfBufferToDisplay != 1)
	{
		l_ui64NumberOfBufferToDisplay--;
	}


	//gets the widget's size
	gint l_iBottomRulerWidth;
	gint l_iBottomRulerHeight;
	gdk_drawable_get_size(m_pBottomRuler->window, &l_iBottomRulerWidth, &l_iBottomRulerHeight);
	
	//draw ruler base (horizontal line)
	gdk_draw_line(m_pBottomRuler->window, m_pBottomRuler->style->fg_gc[GTK_WIDGET_STATE (m_pBottomRuler)], 0, 0, l_iBottomRulerWidth, 0);

	//compute the number of starting time labels to display on the ruler
	uint64 l_ui64NumberOfStartTimeToDisplay = l_iBottomRulerWidth / m_ui64PixelsPerBottomRulerLabel;

	//compute the index step to iterate over the start time list (depending on the number of labels to display)
	size_t l_iIndexStep = (l_ui64NumberOfStartTimeToDisplay <= l_ui64NumberOfBufferToDisplay ) ?
		static_cast<size_t>(1+(l_ui64NumberOfBufferToDisplay/l_ui64NumberOfStartTimeToDisplay) ) :
		1;

	//used to compute the X position of the labels
	float64 l_f64WidthPerBuffer = static_cast<float64>(l_iBottomRulerWidth) / static_cast<float64>(l_ui64NumberOfBufferToDisplay);

	//X position of the first label
	int64 l_i64BaseX = static_cast<int64>(floor(l_iBottomRulerWidth - ( m_pDatabase->m_oSampleBuffers.size() * l_f64WidthPerBuffer)));

	if(l_i64BaseX<0 || l_i64BaseX < l_f64WidthPerBuffer)
	{
		l_i64BaseX = 0;
	}

	stringstream l_oTimeLabel;

	for(size_t j=0 ; j<m_pDatabase->m_oSampleBuffers.size() ; j+=l_iIndexStep)
	{
		//clears the stringstream
		l_oTimeLabel.str("");

		//compute the position of the label
		gint l_iTextX = static_cast<gint>( ((l_i64BaseX + (j*l_f64WidthPerBuffer) - m_f64TranslateX ) * m_f64ScaleX ) );
		
		//convert the time in a displayable format
		float64 l_f64Ms = (((static_cast<uint32>(m_pDatabase->m_oStartTime[j]))>>24) *100) / 256;

		//creates the string to display
		l_oTimeLabel<< (m_pDatabase->m_oStartTime[j]>>32)<<","<<l_f64Ms;

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



void CChannelDisplay::toggleLeftRuler(OpenViBE::boolean bActive)
{
	if(bActive)
	{
		gtk_widget_show(m_pLeftRuler);
	}
	else
	{
		gtk_widget_hide(m_pLeftRuler);
	}
}

void CChannelDisplay::toggleBottomRuler(OpenViBE::boolean bActive)
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


void CChannelDisplay::computeZoom(OpenViBE::boolean bZoomIn, OpenViBE::float64 f64XClick, OpenViBE::float64 f64YClick)
{
	gint l_iWidth = 0;
	gint l_iHeight = 0;
	//gets the size of the drawing area
	gdk_drawable_get_size(m_pDrawingArea->window, &l_iWidth, &l_iHeight);

	if(bZoomIn)
	{
		m_f64ZoomTranslateX += (f64XClick - (l_iWidth/ (m_f64ZoomFactor * 2))) / m_f64ZoomScaleX;
		m_f64ZoomTranslateY += (f64YClick - (l_iHeight/ (m_f64ZoomFactor * 2))) / m_f64ZoomScaleY;

		m_f64ZoomScaleX *= m_f64ZoomFactor;
		m_f64ZoomScaleY *= m_f64ZoomFactor;
	}
	else
	{
		m_f64ZoomScaleX /= m_f64ZoomFactor;
		m_f64ZoomScaleY /= m_f64ZoomFactor;

		m_f64ZoomTranslateX -= (f64XClick - (l_iWidth/ (m_f64ZoomFactor * 2))) / m_f64ZoomScaleX;
		m_f64ZoomTranslateY -= (f64YClick - (l_iHeight/ (m_f64ZoomFactor * 2))) / m_f64ZoomScaleY;
	}

	//check if we are out of the window
	if(m_f64ZoomTranslateX < 0)
	{
		m_f64ZoomTranslateX=0;
	}
	if(m_f64ZoomTranslateY < 0)
	{
		m_f64ZoomTranslateY=0;
	}
	if(m_f64ZoomTranslateX > l_iWidth - (l_iWidth/ m_f64ZoomScaleX) )
	{
		m_f64ZoomTranslateX = l_iWidth - (l_iWidth/ m_f64ZoomScaleX);
	}
	if(m_f64ZoomTranslateY > l_iHeight - (l_iHeight/ m_f64ZoomScaleY) )
	{
		m_f64ZoomTranslateY = l_iHeight - (l_iHeight/ m_f64ZoomScaleY);
	}
}

void CChannelDisplay::computeBestFit()
{
	float64 l_f64LocalMaximum = -DBL_MAX;
	float64 l_f64LocalMinimum = +DBL_MAX;

	for(size_t k=0 ; k<m_oChannelList.size() ; k++)
	{
		//get local min/max
		for(size_t j=0 ; j<m_pDatabase->m_oSampleBuffers.size() ; j++)
		{
			float64 * l_pCurrentBuffer = (m_pDatabase->m_oSampleBuffers)[j]  + (m_oChannelList[k] * m_pDatabase->m_ui32SamplesPerBuffer);

			for(uint64 i = 0 ; i<m_pDatabase->m_ui32SamplesPerBuffer ; i++)
			{
				if(l_pCurrentBuffer[i] > l_f64LocalMaximum)
				{
					l_f64LocalMaximum = l_pCurrentBuffer[i];
				}
				if(l_pCurrentBuffer[i] < l_f64LocalMinimum)
				{
					l_f64LocalMinimum = l_pCurrentBuffer[i];
				}
			}
		}

	}

	//compute the scaling needed for the signal to take up all the drawing area's available space
	m_f64ScaleX = 1;

	if(l_f64LocalMaximum - l_f64LocalMinimum == 0)
	{
		m_f64ScaleY = 1;
	}
	else
	{
		m_f64ScaleY =  1 / (l_f64LocalMaximum - l_f64LocalMinimum);
	}

	//compute the translation needed to center the signal correctly in the window
	m_f64TranslateX = 0;
	m_f64TranslateY =  (l_f64LocalMaximum + l_f64LocalMinimum) / 2;

	//reset the zoom
	m_f64ZoomScaleX = 1;
	m_f64ZoomScaleY = 1;
	m_f64ZoomTranslateX = 0;
	m_f64ZoomTranslateY = 0;
}


void CChannelDisplay::computeNormalSize()
{
	m_f64ZoomTranslateX = 0;
	m_f64ZoomTranslateY = 0;
	m_f64ZoomScaleX = 1;
	m_f64ZoomScaleY = 1;

	m_f64ScaleX = 1;

	if(m_pDatabase->m_f64MaximumValue - m_pDatabase->m_f64MinimumValue == 0)
	{
		m_f64ScaleY = 1;
	}
	else
	{
		m_f64ScaleY =  1 / (m_pDatabase->m_f64MaximumValue - m_pDatabase->m_f64MinimumValue);
	}

	m_f64TranslateX = 0;
	m_f64TranslateY =  (m_pDatabase->m_f64MaximumValue + m_pDatabase->m_f64MinimumValue) / 2;
}

void CChannelDisplay::updateSignalZoomParameters()
{
	//depending on the cursor mode, change the parameters
	switch(m_ui32CurrentSignalMode)
	{
		case 1 :	//zoom in
		case 2 :	//or zoom out, do nothing (keep the current zoom level)
			break;

		case 3 : //Best fit -> actualize
			computeBestFit();
			break;

		case 4 : //Normal size -> actualize
			computeNormalSize();
			break;
	}
}


	}
}

