#include "ovpCChannelDisplayWidget.h"

#include "ovpCSignalDisplayView.h"
#include <cfloat>

#include <sstream>
#include <iostream>

using namespace std;

using namespace OpenViBE;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

#define convert_time(i) (float64)(i>>32) + (float64)((float64)(i&0xFFFFFFFF) / (float64)((uint64)1<<32))

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

	EDisplayMode l_eCursorMode = m_pChannelDisplay->m_pParentDisplayView->m_eCurrentCursorMode;

	//if it's a right click and the action is a zoom in or a zoom out
	//then do the opposite action
	if(event->button == 3)
	{
		if(l_eCursorMode==DisplayMode_ZoomIn)
		{
			l_eCursorMode = DisplayMode_ZoomOut;
		}
		else if(l_eCursorMode==DisplayMode_ZoomOut)
		{
			l_eCursorMode = DisplayMode_ZoomIn;
		}
	}

	//depending on the cursor mode, change the zoom level
	switch(l_eCursorMode)
	{
		case DisplayMode_ZoomIn:
			m_pChannelDisplay->computeZoom(true, event->x, event->y);
			l_bZoomChanged = true;
			break;

		case DisplayMode_ZoomOut:
			if(m_pChannelDisplay->m_f64ZoomScaleX != 1.0)
			{
				m_pChannelDisplay->computeZoom(false, event->x, event->y);
				l_bZoomChanged = true;
			}
			break;

		case DisplayMode_BestFit:
			m_pChannelDisplay->computeBestFit();
			l_bZoomChanged = true;
			break;

		case DisplayMode_Normal:
			m_pChannelDisplay->computeNormalSize();
			l_bZoomChanged = true;
			break;

		default:
			break;
	}

	//keep track of the current mode
	m_pChannelDisplay->m_eCurrentSignalMode = l_eCursorMode;

	//if the zoom level has changed, redraw the signal and left ruler
	if(l_bZoomChanged)
	{
		gdk_window_invalidate_rect(GTK_WIDGET(m_pChannelDisplay->m_pDrawingArea)->window,
					   NULL,
					   true);
		gdk_window_invalidate_rect(GTK_WIDGET(m_pChannelDisplay->m_oLeftRuler.getWidget())->window,
					   NULL,
					   true);

	}

}

//! Callback for when the cursor enters the signal's drawing area
void drawingAreaEnterEventCallback(GtkWidget *widget, GdkEventCrossing *event, gpointer data)
{
	CChannelDisplay * m_pChannelDisplay = reinterpret_cast<CChannelDisplay*>(data);

	//change the cursor to the one corresponding to the cursor mode
	gdk_window_set_cursor(widget->window, m_pChannelDisplay->m_pParentDisplayView->m_pCursor[m_pChannelDisplay->m_pParentDisplayView->m_eCurrentCursorMode]);

}

//! Callback for when the cursor leaves the signal's drawing area
void drawingAreaLeaveEventCallback(GtkWidget *widget, GdkEventCrossing *event, gpointer data)
{
	CChannelDisplay * m_pChannelDisplay = reinterpret_cast<CChannelDisplay*>(data);

	//change the cursor back to the normal one
	gdk_window_set_cursor(widget->window, m_pChannelDisplay->m_pParentDisplayView->m_pCursor[0]);
}

CChannelDisplay::CChannelDisplay() :
	m_pWidgetTable(NULL),
	m_pDrawingArea(NULL),
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
	m_eCurrentSignalMode(DisplayMode_BestFit)
{

}

CChannelDisplay::~CChannelDisplay()
{
}

void CChannelDisplay::setParent(CSignalDisplayView * pDisplayView)
{
	m_pParentDisplayView=pDisplayView;
	m_pDatabase=pDisplayView->m_pBufferDatabase;
}

void CChannelDisplay::init(GtkTable * pTable)
{
	m_pWidgetTable = GTK_WIDGET(pTable);

 	gtk_table_resize(GTK_TABLE(m_pWidgetTable), 2, 2);
 	gtk_table_set_homogeneous(GTK_TABLE(m_pWidgetTable), FALSE);

	//set table minimum height
	gtk_widget_set_size_request(m_pWidgetTable, -1, 20);

	//creates the drawing area
	m_pDrawingArea = gtk_drawing_area_new();
	gtk_widget_set_size_request(m_pDrawingArea, 20, 20);

	//adds them to the table
	gtk_table_attach(GTK_TABLE(m_pWidgetTable), m_pDrawingArea, 1, 2, 0, 1, static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), 0, 0);

	//adds the left ruler
	gtk_table_attach(GTK_TABLE(m_pWidgetTable), m_oLeftRuler.getWidget(), 0, 1, 0, 1, GTK_SHRINK, static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), 0, 0);

	//connects the signals
	gtk_widget_add_events(GTK_WIDGET(m_pDrawingArea), GDK_BUTTON_PRESS_MASK);
	gtk_widget_add_events(GTK_WIDGET(m_pDrawingArea), GDK_ENTER_NOTIFY_MASK);
	gtk_widget_add_events(GTK_WIDGET(m_pDrawingArea), GDK_LEAVE_NOTIFY_MASK);

	g_signal_connect_after(G_OBJECT(m_pDrawingArea), "expose_event",       G_CALLBACK(drawingAreaExposeEventCallback), this);
	g_signal_connect_after(G_OBJECT(m_pDrawingArea), "button-press-event", G_CALLBACK(drawingAreaClickedEventCallback), this);
	g_signal_connect_after(G_OBJECT(m_pDrawingArea), "enter-notify-event", G_CALLBACK(drawingAreaEnterEventCallback), this);
	g_signal_connect_after(G_OBJECT(m_pDrawingArea), "leave-notify-event", G_CALLBACK(drawingAreaLeaveEventCallback), this);
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
	for(size_t i=0 ; i<(m_pDatabase->m_oSampleBuffers.size() *  m_pDatabase->m_pDimmensionSizes[1]) -1 ; i++)
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
			if(i == (m_pDatabase->m_oSampleBuffers.size() *  m_pDatabase->m_pDimmensionSizes[1]) -2)
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

	uint32 l_ui32SamplesPerBuffer = (uint32)m_pDatabase->m_pDimmensionSizes[1];

	//gets the size of the drawing area
	gdk_drawable_get_size(m_pDrawingArea->window, &l_iWidth, &l_iHeight);

	// updates the left ruler
	float64 l_f64MaximumDisplayedValue = m_f64TranslateY - ( (0 - ((l_iHeight*m_f64ZoomScaleY)/2) + (m_f64ZoomTranslateY* m_f64ZoomScaleY)) / (m_f64ScaleY * m_f64ZoomScaleY * l_iHeight) );

	float64 l_f64MinimumDisplayedValue = m_f64TranslateY - ( (l_iHeight - ((l_iHeight*m_f64ZoomScaleY)/2) + (m_f64ZoomTranslateY* m_f64ZoomScaleY)) / (m_f64ScaleY * m_f64ZoomScaleY * l_iHeight) );

	m_oLeftRuler.setMinMax(l_f64MinimumDisplayedValue,l_f64MaximumDisplayedValue);

	//
	uint64 l_ui64NumberOfBufferToDisplay = m_pDatabase->m_ui64NumberOfBufferToDisplay;
	if(l_ui32SamplesPerBuffer == 1 && l_ui64NumberOfBufferToDisplay != 1)
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
	if( (l_ui32SamplesPerBuffer*l_ui64NumberOfBufferToDisplay) - 1 != 0 )
	{
		l_f64PointStep = static_cast<float64>(l_iWidth) / ( (l_ui32SamplesPerBuffer*l_ui64NumberOfBufferToDisplay) - 1);
	}

	for(size_t k=0 ; k<m_oChannelList.size() ; k++)
	{
		for(size_t j=0 ; j<m_pDatabase->m_oSampleBuffers.size() ; j++)
		{
			//gets a pointer to this channels' samples data in the current buffer
			const float64 * l_pCurrentChannelSampleBuffer = (m_pDatabase->m_oSampleBuffers)[j] + (m_oChannelList[k] * l_ui32SamplesPerBuffer);

			//calculates the coordinates of the point in the new base
			for(uint32 i = 0 ; i<l_ui32SamplesPerBuffer ; i++)
			{
				size_t l_ui64PointIndex = (j * l_ui32SamplesPerBuffer)+i;

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

	// ##################################################
	//
	// Dirty add on yrenard so stimulations are displayed
	//
	// ##################################################
	{
		//computes the starting and ending time (in ms) of the displayed data
		uint64 l_ui64StartTime=m_pDatabase->m_oStartTime[0];
		uint64 l_ui64EndTime=m_pDatabase->m_oEndTime[m_pDatabase->m_oStartTime.size()-1];

		// draw stimulations
		std::deque<std::pair<uint64, uint64> >::iterator it;
		for(it=m_pDatabase->m_oStimulations.begin(); it!=m_pDatabase->m_oStimulations.end(); it++)
		{
			float64 l_f64X = ((float64)((it->first-l_ui64StartTime)>>24) / (float64)((l_ui64EndTime-l_ui64StartTime)>>24)); //  ((l_i64BaseX + (j*l_f64WidthPerBuffer) + (i*l_f64PointStep) - m_f64TranslateX ) * m_f64ScaleX) ;
			l_f64X = ((l_f64X - m_f64TranslateX)*m_f64ScaleX);
			l_f64X *= l_iWidth;
			gdk_draw_line(m_pDrawingArea->window, m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], (int)l_f64X, 0, (int)l_f64X, l_iHeight);
		}
	}

	gint l_iZeroY = static_cast<gint>(
			(m_f64ScaleY * m_f64ZoomScaleY * l_iHeight * (m_f64TranslateY - 0) ) +  ((l_iHeight*m_f64ZoomScaleY)/2) - (m_f64ZoomTranslateY* m_f64ZoomScaleY));

	gdk_draw_line(m_pDrawingArea->window, m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 0, l_iZeroY, l_iWidth ,l_iZeroY);

	//switch back to normal line
	gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 1, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);
}

void CChannelDisplay::toggleLeftRuler(OpenViBE::boolean bActive)
{
	m_oLeftRuler.toggle(bActive);
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
	float64 l_f64CurrentMaximum;
	float64 l_f64CurrentMinimum;

	for(size_t k=0 ; k<m_oChannelList.size() ; k++)
	{
		//get local min/max
		m_pDatabase->getDisplayedChannelLocalMinMaxValue(m_oChannelList[k], l_f64CurrentMinimum, l_f64CurrentMaximum);

		if(l_f64CurrentMinimum<l_f64LocalMinimum)
		{
			l_f64LocalMinimum=l_f64CurrentMinimum;
		}

		if(l_f64CurrentMaximum>l_f64LocalMaximum)
		{
			l_f64LocalMaximum=l_f64CurrentMaximum;
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

	float64 l_f64GlobalMin = m_pDatabase->m_f64MinimumValue;
	float64 l_f64GlobalMax = m_pDatabase->m_f64MaximumValue;

	if(l_f64GlobalMax == l_f64GlobalMin)
	{
		m_f64ScaleY = 1;
	}
	else
	{
		m_f64ScaleY =  1 / (l_f64GlobalMax - l_f64GlobalMin);
	}

	m_f64TranslateX = 0;
	m_f64TranslateY =  (l_f64GlobalMax + l_f64GlobalMin) / 2;
}

void CChannelDisplay::updateSignalZoomParameters()
{
	//depending on the cursor mode, change the parameters
	switch(m_eCurrentSignalMode)
	{
		case DisplayMode_Default:
		case DisplayMode_ZoomIn:
		case DisplayMode_ZoomOut: // do nothing (keep the current zoom level)
			break;

		case DisplayMode_BestFit: // actualize
			computeBestFit();
			break;

		case DisplayMode_Normal: // actualize
			computeNormalSize();
			break;
	}
}

	}
}

