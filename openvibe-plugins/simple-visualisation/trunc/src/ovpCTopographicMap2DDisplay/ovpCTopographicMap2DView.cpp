#include "ovpCTopographicMap2DView.h"

#include <iostream>

#include <sstream>

#include <math.h>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		static const unsigned int s_nbColors = 13;
		static GdkColor s_palette[13];
		static uint8 s_palette8[13*3];

		CTopographicMap2DView::CTopographicMap2DView(CTopographicMapDatabase& rTopographicMapDatabase)
			:m_rTopographicMapDatabase(rTopographicMapDatabase)
			,m_pGladeInterface(NULL)
			,m_ui32CurrentProjection(TopographicMap2DProjection_Radial)
			,m_pDrawingArea(NULL)
			,m_pClipmask(NULL)
			,m_pClipmaskGC(NULL)
			,m_bRedrawClipmask(true)
			,m_ui32CurrentViewport(TopographicMap2DViewport_Top)
			,m_bNeedResize(true)
			,m_ui32GridSize(0)
			,m_ui32CellSize(0)
			,m_ui32MinPaletteBarHeight(10)
			,m_ui32MaxPaletteBarHeight(30)
			,m_ui32HeadWindowWidth(0)
			,m_ui32HeadWindowHeight(0)
			,m_ui32PaletteWindowWidth(0)
			,m_ui32PaletteWindowHeight(0)
			,m_pSkullRGBBuffer(NULL)
		{
			m_oSampleCoordinatesMatrix.setDimensionCount(2);

			//load the glade interface
			m_pGladeInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-TopographicMap2D.glade", NULL, NULL);

			if(!m_pGladeInterface)
			{
				g_warning("Couldn't load the interface!");
				return;
			}

			glade_xml_signal_autoconnect(m_pGladeInterface);

			//toolbar
			//-------

			//get pointers to projection mode buttons
			m_pProjectionButtons[TopographicMap2DProjection_Axial] =
				GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "AxialProjection"));
			m_pProjectionButtons[TopographicMap2DProjection_Radial] =
				GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "RadialProjection"));

			//connect callbacks
			enableProjectionButtonSignals(true);

			m_oBackgroundColor.pixel = 0;
			m_oBackgroundColor.red = 0xFFFF;
			m_oBackgroundColor.green = 0;//0xFFFF;
			m_oBackgroundColor.blue = 0;//0xFFFF;

			s_palette[0].red = 255*65535/255; s_palette[0].green = 0*65535/255; s_palette[0].blue = 0*65535/255;
			s_palette[1].red = 234*65535/255; s_palette[1].green = 1*65535/255; s_palette[1].blue = 0*65535/255;
			s_palette[2].red = 205*65535/255; s_palette[2].green = 0*65535/255; s_palette[2].blue = 101*65535/255;
			s_palette[3].red = 153*65535/255; s_palette[3].green = 0*65535/255; s_palette[3].blue = 178*65535/255;
			s_palette[4].red = 115*65535/255; s_palette[4].green = 1*65535/255; s_palette[4].blue = 177*65535/255;
			s_palette[5].red = 77*65535/255; s_palette[5].green = 0*65535/255; s_palette[5].blue = 178*65535/255;
			s_palette[6].red = 0*65535/255; s_palette[6].green = 0*65535/255; s_palette[6].blue = 152*65535/255;
			s_palette[7].red = 0*65535/255; s_palette[7].green = 97*65535/255; s_palette[7].blue = 121*65535/255;
			s_palette[8].red = 0*65535/255; s_palette[8].green = 164*65535/255; s_palette[8].blue = 100*65535/255;
			s_palette[9].red = 0*65535/255; s_palette[9].green = 225*65535/255; s_palette[9].blue = 25*65535/255;
			s_palette[10].red = 150*65535/255; s_palette[10].green = 255*65535/255; s_palette[10].blue = 0*65535/255;
			s_palette[11].red = 200*65535/255; s_palette[11].green = 255*65535/255; s_palette[11].blue = 0*65535/255;
			s_palette[12].red = 255*65535/255; s_palette[12].green = 255*65535/255; s_palette[12].blue = 0*65535/255;

			s_palette8[0] = 255; s_palette8[1] = 0; s_palette8[2] = 0;
			s_palette8[3] = 234; s_palette8[4] = 1; s_palette8[5] = 0;
			s_palette8[6] = 205; s_palette8[7] = 0; s_palette8[8] = 101;
			s_palette8[9] = 153; s_palette8[10] = 0; s_palette8[11] = 178;
			s_palette8[12] = 115; s_palette8[13] = 1; s_palette8[14] = 177;
			s_palette8[15] = 77; s_palette8[16] = 0; s_palette8[17] = 178;
			s_palette8[18] = 0; s_palette8[19] = 0; s_palette8[20] = 152;
			s_palette8[21] = 0; s_palette8[22] = 97; s_palette8[23] = 121;
			s_palette8[24] = 0; s_palette8[25] = 164; s_palette8[26] = 100;
			s_palette8[27] = 0; s_palette8[28] = 225; s_palette8[29] = 25;
			s_palette8[30] = 150; s_palette8[31] = 255; s_palette8[32] = 0;
			s_palette8[33] = 200; s_palette8[34] = 255; s_palette8[35] = 0;
			s_palette8[36] = 255; s_palette8[37] = 255; s_palette8[38] = 0;
		}

		CTopographicMap2DView::~CTopographicMap2DView()
		{
			//destroy clip mask
			if(m_pClipmask)
			{
				//gtk_object_destroy(GTK_OBJECT(m_pClipmask)); //invalid cast
				g_object_unref(m_pClipmask);
				m_pClipmask = NULL;
			}
			if(m_pClipmaskGC)
			{
				g_object_unref(m_pClipmaskGC);
				m_pClipmaskGC=NULL;
			}
			//destroy pixmap
			if(m_pSkullRGBBuffer)
			{
				delete m_pSkullRGBBuffer;
				m_pSkullRGBBuffer = NULL;
			}

			//unref the xml file as it's not needed anymore
			g_object_unref(G_OBJECT(m_pGladeInterface));
			m_pGladeInterface=NULL;
		}

		void CTopographicMap2DView::init()
		{
			m_pDrawingArea = glade_xml_get_widget(m_pGladeInterface, "TopographicMap2DDrawingArea");

			gtk_widget_set_double_buffered(m_pDrawingArea, TRUE);

			g_signal_connect(G_OBJECT(m_pDrawingArea), "expose_event", G_CALLBACK(redrawCB), this);
			g_signal_connect(G_OBJECT(m_pDrawingArea), "size-allocate", G_CALLBACK(sizeAllocateCB), this);

			gtk_widget_show(m_pDrawingArea);

			setCurrentViewport(TopographicMap2DViewport_Top);
			setProjection(m_ui32CurrentProjection);
		}

		void CTopographicMap2DView::getWidgets(GtkWidget*& pWidget, GtkWidget*& pToolbarWidget)
		{
			pWidget = glade_xml_get_widget(m_pGladeInterface, "TopographicMap2DDrawingArea");
			pToolbarWidget = glade_xml_get_widget(m_pGladeInterface, "Toolbar");
		}

		CTopographicMap2DView::ETopographicMap2DViewport CTopographicMap2DView::getCurrentViewport()
		{
			return m_ui32CurrentViewport;
		}

		void CTopographicMap2DView::setCurrentViewport(ETopographicMap2DViewport eViewport)
		{
			m_ui32CurrentViewport = eViewport;
		}

		gboolean CTopographicMap2DView::redrawCB(GtkWidget *widget, GdkEventExpose *event, gpointer data)
		{
			reinterpret_cast<CTopographicMap2DView*>(data)->redraw();
			return TRUE;
		}

		void CTopographicMap2DView::redraw()
		{
			if(m_pDrawingArea != NULL && GTK_WIDGET_VISIBLE(m_pDrawingArea))
			{
				if(m_bNeedResize == true)
				{
					resizeData();

					if(m_rTopographicMapDatabase.interpolateValues() == false)
					{
						m_bNeedResize = true; //FIXME : have 2 separate flags, one for resize and one for interpolation of values
						return;
					}
				}

				//draw head
				drawTopViewport(0, 0, m_ui32HeadWindowWidth, m_ui32HeadWindowHeight);

				//draw palette
				drawPalette(0, m_ui32HeadWindowHeight, m_ui32PaletteWindowWidth, m_ui32PaletteWindowHeight);

				//gdk_window_invalidate_rect(m_pDrawingArea->window,	NULL,	true);
			}
		}

		gboolean CTopographicMap2DView::sizeAllocateCB(GtkWidget *widget, GtkAllocation *allocation, gpointer data)
		{
			reinterpret_cast<CTopographicMap2DView*>(data)->resize((uint32)allocation->width, (uint32)allocation->height);
			return FALSE;
		}

		void CTopographicMap2DView::resize(uint32 ui32Width, uint32 ui32Height)
		{
			m_bNeedResize = true;
		}

		CMatrix* CTopographicMap2DView::getSampleCoordinatesMatrix()
		{
			if(m_bNeedResize == true)
				resizeData();

			return &m_oSampleCoordinatesMatrix;
		}

		boolean CTopographicMap2DView::setSampleValuesMatrix(IMatrix* pSampleValuesMatrix)
		{
			//ensure matrix has the right size
			if(pSampleValuesMatrix == NULL || pSampleValuesMatrix->getDimensionSize(0) < m_oSampleValues.size())
				return false;

			//retrieve min/max potentials
			float64 l_f64MinPotential, l_f64MaxPotential;
			m_rTopographicMapDatabase.getLastBufferInterpolatedMinMaxValue(l_f64MinPotential, l_f64MaxPotential);
			float64 l_f64InvPotentialStep = s_nbColors / (l_f64MaxPotential - l_f64MinPotential);

			//determine color index of each sample
			for(uint32 i=0; i<m_oSampleValues.size(); i++)
			{
				float64 l_f64Value = *(pSampleValuesMatrix->getBuffer() + i);
				uint32 l_iColorIndex = (uint32)((l_f64Value - l_f64MinPotential) * l_f64InvPotentialStep);
				if(l_iColorIndex >= s_nbColors)
				{
					l_iColorIndex = s_nbColors-1;
				}
				m_oSampleValues[i] = l_iColorIndex;
			}

			refreshPotentials();

			//force redraw

			return true;
		}

		void CTopographicMap2DView::drawPalette(uint32 ui32X, uint32 ui32Y, uint32 ui32Width, uint32 ui32Height)
		{
			if(ui32Width == 0 || ui32Height == 0)
				return;

			boolean l_bDrawText = true;

			//retrieve text size
			PangoLayout* l_pText = gtk_widget_create_pango_layout(GTK_WIDGET(m_pDrawingArea), "0");
			gint l_iTextHeight;
			pango_layout_get_pixel_size(l_pText, NULL, &l_iTextHeight);

			//don't draw text if not enough room
			if(l_iTextHeight >= (gint)(ui32Height - m_ui32MinPaletteBarHeight))
				l_bDrawText = false;

			//determine palette bar dims
			gint l_iPaletteBarWidth = (gint)(0.9 * ui32Width);
			gint l_iPaletteBarHeight = (gint)(ui32Height - l_iTextHeight);
			if(l_iPaletteBarHeight < (gint)(m_ui32MinPaletteBarHeight))
				l_iPaletteBarHeight = (gint)m_ui32MinPaletteBarHeight;
			else if(l_iPaletteBarHeight > (gint)(m_ui32MaxPaletteBarHeight))
				l_iPaletteBarHeight = (gint)(m_ui32MaxPaletteBarHeight);

			gint l_iPaletteBarStartX = (gint)(ui32X + (ui32Width - l_iPaletteBarWidth)/2);
			gint l_iPaletteBarStartY = (gint)(ui32Y);

			gint l_iTextWidth;
			gint l_iLabelX;
			gint l_iLabelY = l_iPaletteBarStartY + l_iPaletteBarHeight + (ui32Height - l_iPaletteBarHeight - l_iTextHeight)/2;

			//draw 0 label
			pango_layout_get_pixel_size(l_pText, &l_iTextWidth, NULL);
			l_iLabelX = ui32X + (ui32Width - l_iTextWidth)/2;

			gdk_draw_layout(m_pDrawingArea->window, m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
				l_iLabelX,
				l_iLabelY,
				l_pText);

			//draw + label
			pango_layout_set_text(l_pText, "+", 1);
			pango_layout_get_pixel_size(l_pText, &l_iTextWidth, NULL);
			l_iLabelX = l_iPaletteBarStartX - l_iTextWidth/2;
			if(l_iLabelX < 0)
				l_iLabelX = 0;

			gdk_draw_layout(m_pDrawingArea->window, m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
				l_iLabelX,
				l_iLabelY,
				l_pText);

			//draw - label
			pango_layout_set_text(l_pText, "-", 1);
			pango_layout_get_pixel_size(l_pText, &l_iTextWidth, NULL);
			l_iLabelX = l_iPaletteBarStartX + l_iPaletteBarWidth - l_iTextWidth/2;
			if(l_iLabelX + l_iTextWidth >= (gint)ui32Width)
				l_iLabelX = ui32Width - l_iTextWidth;

			gdk_draw_layout(m_pDrawingArea->window, m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
				l_iLabelX,
				l_iLabelY,
				l_pText);

			//draw palette bar (typically reversed : high potentials to the left; low potentials to the right)
			gint l_iCurrentX = l_iPaletteBarStartX;

			for(int i=s_nbColors-1; i>=0; i--)
			{
				gdk_gc_set_rgb_fg_color(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
					&s_palette[i]);

				gdk_draw_rectangle(m_pDrawingArea->window,
					m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
					TRUE,
					l_iCurrentX,
					l_iPaletteBarStartY,
					l_iPaletteBarWidth/s_nbColors,
					l_iPaletteBarHeight);

				l_iCurrentX += l_iPaletteBarWidth/13;
			}

			//restore default black color
			GdkColor l_oBlack;
			l_oBlack.red = l_oBlack.green = l_oBlack.blue = 0;
			gdk_gc_set_rgb_fg_color(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)], &l_oBlack);
		}

		void CTopographicMap2DView::drawTopViewport(uint32 ui32X, uint32 ui32Y, uint32 ui32Width, uint32 ui32Height)
		{
			//draw nose
			//---------

			gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 2, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);

			//head center
			uint32 l_ui32SkullCenterX = m_ui32SkullX + m_ui32SkullDiameter/2;
			uint32 l_ui32SkullCenterY = m_ui32SkullY + m_ui32SkullDiameter/2;

			const float32 l_f32NoseHalfAngle = 6;

#ifndef M_PI
#define M_PI 3.1415926535
#endif

#define DEG2RAD(x) (x)*M_PI/180

			//nose lower left anchor
			uint32 l_ui32NoseLowerLeftX = (uint32)(l_ui32SkullCenterX + m_ui32SkullDiameter/2*cos(DEG2RAD(90+l_f32NoseHalfAngle)));
			uint32 l_ui32NoseLowerLeftY = (uint32)(l_ui32SkullCenterY - m_ui32SkullDiameter/2*sin(DEG2RAD(90+l_f32NoseHalfAngle)));

			//nose lower right anchor
			uint32 l_ui32NoseLowerRightX = (uint32)(l_ui32SkullCenterX + m_ui32SkullDiameter/2*cos(DEG2RAD(90-l_f32NoseHalfAngle)));
			uint32 l_ui32NoseLowerRightY = (uint32)(l_ui32SkullCenterY - m_ui32SkullDiameter/2*sin(DEG2RAD(90-l_f32NoseHalfAngle)));

			gdk_draw_line(m_pDrawingArea->window,
				m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
				(gint)l_ui32NoseLowerLeftX,
				(gint)l_ui32NoseLowerLeftY,
				(gint)l_ui32SkullCenterX,
				(gint)m_ui32NoseY);

			gdk_draw_line(m_pDrawingArea->window,
				m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
				(gint)l_ui32NoseLowerRightX,
				(gint)l_ui32NoseLowerRightY,
				(gint)l_ui32SkullCenterX,
				(gint)m_ui32NoseY);

			//draw head outline
			//-----------------

			gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 4, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);

			gdk_draw_arc(m_pDrawingArea->window,
				m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
				FALSE,
				(gint)m_ui32SkullX,
				(gint)m_ui32SkullY,
				(gint)m_ui32SkullDiameter,
				(gint)m_ui32SkullDiameter,
				0,
				64*360);

			//if(m_bRedrawClipmask == true && m_pClipmask != NULL)
			{
				GdkColor l_oWhite;
				l_oWhite.red = l_oWhite.green = l_oWhite.blue = 65535;
				gdk_gc_set_rgb_fg_color(m_pClipmaskGC/*m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)]*/, &l_oWhite);

				gdk_draw_arc(m_pClipmask,
				m_pClipmaskGC/*m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)]*/,
				TRUE,
				0,
				0,
				(gint)m_ui32SkullDiameter,
				(gint)m_ui32SkullDiameter,
				0,
				64*360);

				//restore default black color
				GdkColor l_oBlack;
				l_oBlack.red = l_oBlack.green = l_oBlack.blue = 0;
				gdk_gc_set_rgb_fg_color(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)], &l_oBlack);

				gdk_gc_set_clip_origin(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
					(gint)m_ui32SkullX,
					(gint)m_ui32SkullY);

				m_bRedrawClipmask = false;
			}

			gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 1, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);

			gdk_gc_set_clip_mask(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)], m_pClipmask);

			drawPotentials();
			drawElectrodes();

			gdk_gc_set_clip_mask(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)], NULL);
		}

		void CTopographicMap2DView::resizeData()
		{
			m_bNeedResize = false;

			uint32 l_iWindowWidth = m_pDrawingArea->allocation.width;
			uint32 l_iWindowHeight = m_pDrawingArea->allocation.height;

			//retrieve text size
			PangoLayout* l_pText = gtk_widget_create_pango_layout(GTK_WIDGET(m_pDrawingArea), "0");
			int l_iTextHeight;
			pango_layout_get_pixel_size(l_pText, NULL, &l_iTextHeight);
			//FIXME : delete text

			//palette sub-window dims
			m_ui32PaletteWindowWidth = l_iWindowWidth;
			m_ui32PaletteWindowHeight = (uint32)(0.1 * l_iWindowHeight);
			if(m_ui32PaletteWindowHeight > (uint32)(m_ui32MaxPaletteBarHeight + l_iTextHeight + 4))
			{
				m_ui32PaletteWindowHeight = m_ui32MaxPaletteBarHeight + l_iTextHeight + 4;
			}
			else if(m_ui32PaletteWindowHeight < (uint32)(m_ui32MinPaletteBarHeight + l_iTextHeight))
			{
				m_ui32PaletteWindowHeight = (uint32)(m_ui32MinPaletteBarHeight + l_iTextHeight);
			}

			//return if not enough room available
			if(m_ui32PaletteWindowHeight > l_iWindowHeight)
			{
				return;
			}

			//head sub-window dims
			m_ui32HeadWindowWidth = l_iWindowWidth;
			m_ui32HeadWindowHeight = l_iWindowHeight - m_ui32PaletteWindowHeight;

			uint32 l_ui32HeadHeight;
			if(m_ui32HeadWindowWidth < m_ui32HeadWindowHeight)
			{
				l_ui32HeadHeight = (uint32)(0.9 * m_ui32HeadWindowWidth);
			}
			else
			{
				l_ui32HeadHeight = (uint32)(0.9 * m_ui32HeadWindowHeight);
			}

			uint32 l_ui32NoseHeight = (uint32)(0.1 * l_ui32HeadHeight);
			m_ui32SkullDiameter = l_ui32HeadHeight - l_ui32NoseHeight;
			m_ui32NoseY = (uint32)((m_ui32HeadWindowHeight - l_ui32HeadHeight)/2);
			m_ui32SkullX = (m_ui32HeadWindowWidth - m_ui32SkullDiameter)/2;
			m_ui32SkullY = m_ui32NoseY + l_ui32NoseHeight;

			//allocate clipmask
			if(m_pClipmask)
			{
				//gtk_object_destroy(GTK_OBJECT(m_pClipmask)); //invalid cast!
				g_object_unref(m_pClipmask);
			}
			if(m_pClipmaskGC)
			{
				g_object_unref(m_pClipmaskGC);
			}
			m_pClipmask = gdk_pixmap_new(m_pDrawingArea->window, m_ui32SkullDiameter, m_ui32SkullDiameter, 1);
			m_pClipmaskGC = gdk_gc_new(GDK_DRAWABLE(m_pClipmask));
			gdk_gc_set_colormap(m_pClipmaskGC, gdk_gc_get_colormap(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)]));
			//clear it
			gdk_draw_rectangle(m_pClipmask, m_pClipmaskGC/*m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)]*/,
				TRUE, 0, 0, m_ui32SkullDiameter, m_ui32SkullDiameter);

			//allocate RGB pixmap
			if(m_pSkullRGBBuffer != NULL)
			{
				delete [] m_pSkullRGBBuffer;
			}
			//align lines on 32bit boundaries
			m_ui32RowStride = ((m_ui32SkullDiameter*3)%4 == 0) ? (m_ui32SkullDiameter*3) : ((((m_ui32SkullDiameter*3)>>2)+1)<<2);
			m_pSkullRGBBuffer = new guchar[m_ui32RowStride*m_ui32SkullDiameter];

			//determine size of colored cells
#if 1
			uint32 l_ui32CellMinSize = 1;
			//uint32 l_ui32CellMaxSize = 40;
			float64 l_f64CellOverSkullSizeRatio = 0.02;
			m_ui32CellSize = (uint32)(m_ui32SkullDiameter * l_f64CellOverSkullSizeRatio);

			if(m_ui32CellSize < l_ui32CellMinSize)
			{
				m_ui32CellSize = l_ui32CellMinSize;
			}/*
			else if(m_ui32CellSize > l_ui32CellMaxSize)
			{
				m_ui32CellSize = l_ui32CellMaxSize;
			}*/
#else
			m_ui32CellSize = m_ui32SkullDiameter/2;
#endif
			if(m_ui32CellSize == 0)
			{
				return;
			}

			//number of samples in a row or column
			m_ui32GridSize = (uint32)ceil(m_ui32SkullDiameter / (double)m_ui32CellSize);

			//determine number of samples lying within skull
			uint32 l_ui32NbSamples = getSamplesCount();

			//resize sample grids accordingly
			m_oSample2DCoordinates.resize(l_ui32NbSamples);
			m_oSampleCoordinatesMatrix.setDimensionSize(0, l_ui32NbSamples);
			m_oSampleCoordinatesMatrix.setDimensionSize(1, 3);
			m_oSampleValues.resize(l_ui32NbSamples);

			//compute samples normalized coordinates
			computeSamplesNormalizedCoordinates();
		}

		void CTopographicMap2DView::refreshPotentials()
		{
			uint32 w, h;
			for(uint32 i=(uint32)m_rTopographicMapDatabase.getChannelCount(); i<m_oSampleValues.size(); i++)
			{
				//cells of last row and last column may be smaller than other ones
				if(m_oSample2DCoordinates[i].first + m_ui32CellSize >= m_ui32SkullDiameter)
				{
					w = m_ui32SkullDiameter - m_oSample2DCoordinates[i].first;
				}
				else
				{
					w = m_ui32CellSize;
				}

				if(m_oSample2DCoordinates[i].second + m_ui32CellSize >= m_ui32SkullDiameter)
				{
					h = m_ui32SkullDiameter - m_oSample2DCoordinates[i].second;
				}
				else
				{
					h = m_ui32CellSize;
				}

				drawBoxToBuffer(
					m_oSample2DCoordinates[i].first,
					m_oSample2DCoordinates[i].second,
					w,
					h,
					s_palette8[3*m_oSampleValues[i]],
					s_palette8[3*m_oSampleValues[i]+1],
					s_palette8[3*m_oSampleValues[i]+2]);
			}
		}

		void CTopographicMap2DView::drawPotentials()
		{
			gdk_draw_rgb_image(
				m_pDrawingArea->window,
				m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
				m_ui32SkullX, m_ui32SkullY,
				m_ui32SkullDiameter, m_ui32SkullDiameter,
				GDK_RGB_DITHER_NONE,
				m_pSkullRGBBuffer,
				m_ui32RowStride);
		}

		void CTopographicMap2DView::drawElectrodes()
		{
			uint32 l_ui32SkullCenterX = m_ui32SkullX + m_ui32SkullDiameter/2;
			uint32 l_ui32SkullCenterY = m_ui32SkullY + m_ui32SkullDiameter/2;

			//determine size of electrode rings
			uint32 l_ui32ElectrodeRingMinSize = 1;
			uint32 l_ui32ElectrodeRingMaxSize = 20;
			float64 l_f64ElectrodeRingOverSkullSizeRatio = 0.05;
			gint l_i32ElectrodeRingSize = (gint)(m_ui32SkullDiameter * l_f64ElectrodeRingOverSkullSizeRatio);
			if(l_i32ElectrodeRingSize < (gint)l_ui32ElectrodeRingMinSize)
			{
				l_i32ElectrodeRingSize = (gint)l_ui32ElectrodeRingMinSize;
			}
			else if(l_i32ElectrodeRingSize > (gint)l_ui32ElectrodeRingMaxSize)
			{
				l_i32ElectrodeRingSize = (gint)l_ui32ElectrodeRingMaxSize;
			}

			if(l_i32ElectrodeRingSize == 0)
				return;

			GdkColor l_oWhite;
			l_oWhite.red = 65535;
			l_oWhite.green = 65535;
			l_oWhite.blue = 65535;

			GdkColor l_oBlack;
			l_oBlack.red = 0;
			l_oBlack.green = 0;
			l_oBlack.blue = 0;

			//set electrode ring thickness
			gint l_i32ElectrodeRingThickness = 1;
			gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], l_i32ElectrodeRingThickness, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);

			//electrode label
			CString l_oElectrodeLabel;
			PangoLayout* l_pElectrodeLabelLayout = gtk_widget_create_pango_layout(GTK_WIDGET(m_pDrawingArea), " ");
			gint l_iTextHeight;
			gint l_iTextWidth;
			pango_layout_get_pixel_size(l_pElectrodeLabelLayout, NULL, &l_iTextHeight);

			//draw rings
			uint32 l_ui32ElectrodeCount = (uint32)m_rTopographicMapDatabase.getChannelCount();
			gint l_i32RingX;
			gint l_i32RingY;

			for(uint32 i=0; i<l_ui32ElectrodeCount; i++)
			{
				if(m_ui32CurrentProjection == TopographicMap2DProjection_Axial)
				{
					float64* l_pElectrodePosition;
					m_rTopographicMapDatabase.getChannelPosition(i, l_pElectrodePosition);
					l_i32RingX = (gint)(l_ui32SkullCenterX + l_pElectrodePosition[0] * m_ui32SkullDiameter/2);
					//y axis is down in 2D and up in 3D convention
					l_i32RingY = (gint)(l_ui32SkullCenterY - l_pElectrodePosition[1] * m_ui32SkullDiameter/2);
				}
				else //radial
				{
					float64 l_f64Theta;
					float64 l_f64Phi;
					m_rTopographicMapDatabase.getChannelSphericalCoordinates(i, l_f64Theta, l_f64Phi);

					//linear plotting along radius
					float64 l_f64Length = l_f64Theta/90 * m_ui32SkullDiameter/2;

					//determine point coords on unit circle
					float64 l_f64X = cos(l_f64Phi * 3.1415926535 / 180);
					float64 l_f64Y = sin(l_f64Phi * 3.1415926535 / 180);

					//scale vector so that it is l_f64Length long
					l_i32RingX = (gint)(l_ui32SkullCenterX + l_f64Length * l_f64X);
					//y axis is down in 2D and up in 3D convention
					l_i32RingY = (gint)(l_ui32SkullCenterY - l_f64Length * l_f64Y);
				}

				//ring
				gdk_gc_set_rgb_fg_color(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)], &s_palette[m_oSampleValues[i]]);

				gdk_draw_arc(m_pDrawingArea->window,
				m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
				TRUE,
				l_i32RingX - l_i32ElectrodeRingSize/2,
				l_i32RingY - l_i32ElectrodeRingSize/2,
				l_i32ElectrodeRingSize,
				l_i32ElectrodeRingSize,
				0,
				64*360);

				gdk_gc_set_rgb_fg_color(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)], &l_oBlack);

				gdk_draw_arc(m_pDrawingArea->window,
				m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
				FALSE,
				l_i32RingX - l_i32ElectrodeRingSize/2,
				l_i32RingY - l_i32ElectrodeRingSize/2,
				l_i32ElectrodeRingSize,
				l_i32ElectrodeRingSize,
				0,
				64*360);

				//label
				gdk_gc_set_rgb_fg_color(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)], &l_oWhite);

				m_rTopographicMapDatabase.getChannelLabel(i, l_oElectrodeLabel);
				pango_layout_set_text(l_pElectrodeLabelLayout, l_oElectrodeLabel, strlen(l_oElectrodeLabel));
				pango_layout_get_pixel_size(l_pElectrodeLabelLayout, &l_iTextWidth, NULL);
				gdk_draw_layout(m_pDrawingArea->window, m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)],
					l_i32RingX-l_iTextWidth/2,
					l_i32RingY-l_i32ElectrodeRingSize/2-l_iTextHeight-5,
					l_pElectrodeLabelLayout);
			}

			//restore default line thickness
			gdk_gc_set_line_attributes(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE (m_pDrawingArea)], 1, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);

			//restore default black color
			gdk_gc_set_rgb_fg_color(m_pDrawingArea->style->fg_gc[GTK_WIDGET_STATE(m_pDrawingArea)], &l_oBlack);
		}

		void CTopographicMap2DView::drawBoxToBuffer(uint32 ui32X, uint32 ui32Y, uint32 ui32Width, uint32 ui32Height, uint8 ui8Red, uint8 ui8Green, uint8 ui8Blue)
		{
			guchar* l_pLineBase = m_pSkullRGBBuffer + (m_ui32RowStride*ui32Y) + (ui32X*3);

			for(uint32 j=0 ; j<ui32Height ; j++)
			{
				for(uint32 i=0 ; i<(ui32Width*3) ; i+=3)
				{
					*(l_pLineBase + i) =  ui8Red;
					*(l_pLineBase + i + 1) =  ui8Green;
					*(l_pLineBase + i + 2) =  ui8Blue;
				}

				l_pLineBase+=(m_ui32RowStride);
			}
		}

		uint32 CTopographicMap2DView::getSamplesCount()
		{
			uint32 l_ui32SamplesCount = (uint32)m_rTopographicMapDatabase.getChannelCount();

			float32 l_f32CurX, l_f32CurY;
			uint32 i, j;
			float32 l_f32SquareRadius = (m_ui32SkullDiameter/2.f) * (m_ui32SkullDiameter/2.f);
			float32 l_f32SkullCenterX = m_ui32SkullX + m_ui32SkullDiameter/2.f;
			float32 l_f32SkullCenterY = m_ui32SkullY + m_ui32SkullDiameter/2.F;
			float32 l_f32ClosestX, l_f32ClosestY;

			//for each row
			for(i=0, l_f32CurY=(float32)m_ui32SkullY; i<m_ui32GridSize; i++, l_f32CurY+=m_ui32CellSize)
			{
				//for each column
				for(j=0, l_f32CurX=(float32)m_ui32SkullX; j<m_ui32GridSize; j++, l_f32CurX+=m_ui32CellSize)
				{
					//find corner closest to skull center
					l_f32ClosestX = fabs(l_f32CurX - l_f32SkullCenterX) < fabs(l_f32CurX + m_ui32CellSize - l_f32SkullCenterX) ? l_f32CurX : (l_f32CurX + m_ui32CellSize);
					l_f32ClosestY = fabs(l_f32CurY - l_f32SkullCenterY) < fabs(l_f32CurY + m_ui32CellSize - l_f32SkullCenterY) ? l_f32CurY : (l_f32CurY + m_ui32CellSize);

					//compute square distance of skull center to closest cell corner
					float32 l_f32SquareDistance = (l_f32ClosestX - l_f32SkullCenterX) * (l_f32ClosestX - l_f32SkullCenterX) +
						(l_f32ClosestY - l_f32SkullCenterY) * (l_f32ClosestY - l_f32SkullCenterY);

					//ensure corner lies within skull
					if(l_f32SquareDistance <= l_f32SquareRadius)
					{
						l_ui32SamplesCount++;
					}
				}
			}

			return l_ui32SamplesCount;
		}

		void CTopographicMap2DView::computeSamplesNormalizedCoordinates()
		{
			uint32 l_ui32CurSample = 0;
			uint32 l_ui32ChannelCount = (uint32)m_rTopographicMapDatabase.getChannelCount();
			float64* l_pElectrodePosition=NULL;

			//sampling at electrode locations
			for(l_ui32CurSample=0; l_ui32CurSample<l_ui32ChannelCount; l_ui32CurSample++)
			{
				m_rTopographicMapDatabase.getChannelPosition(l_ui32CurSample, l_pElectrodePosition);

				//dummy 2D coords - actual coords are computed when drawing electrode rings
				m_oSample2DCoordinates[l_ui32CurSample].first = 0;
				m_oSample2DCoordinates[l_ui32CurSample].second= 0;

				*(m_oSampleCoordinatesMatrix.getBuffer() + 3*l_ui32CurSample) = *l_pElectrodePosition;
				*(m_oSampleCoordinatesMatrix.getBuffer() + 3*l_ui32CurSample+1) = *(l_pElectrodePosition+1);
				*(m_oSampleCoordinatesMatrix.getBuffer() + 3*l_ui32CurSample+2) = *(l_pElectrodePosition+2);
			}

			//sampling over skull area
			float32 l_f32CurX, l_f32CurY;
			uint32 i, j;
			float32 l_f32SquareRadius = (m_ui32SkullDiameter/2.f) * (m_ui32SkullDiameter/2.f);
			float32 l_f32SkullCenterX = m_ui32SkullX + m_ui32SkullDiameter/2.f;
			float32 l_f32SkullCenterY = m_ui32SkullY + m_ui32SkullDiameter/2.F;
			float32 l_f32ClosestX, l_f32ClosestY;
			float32 l_f32X, l_f32Y; //normalized coordinates in projection plane

			//for each row
			for(i=0, l_f32CurY=(float32)m_ui32SkullY; i<m_ui32GridSize; i++, l_f32CurY+=m_ui32CellSize)
			{
				//for each column
				for(j=0, l_f32CurX=(float32)m_ui32SkullX; j<m_ui32GridSize; j++, l_f32CurX+=m_ui32CellSize)
				{
					//find corner closest to skull center
					l_f32ClosestX = fabs(l_f32CurX - l_f32SkullCenterX) < fabs(l_f32CurX + m_ui32CellSize - l_f32SkullCenterX) ? l_f32CurX : (l_f32CurX + m_ui32CellSize);
					l_f32ClosestY = fabs(l_f32CurY - l_f32SkullCenterY) < fabs(l_f32CurY + m_ui32CellSize - l_f32SkullCenterY) ? l_f32CurY : (l_f32CurY + m_ui32CellSize);

					//compute square distance of skull center to closest cell corner
					float32 l_f32SquareDistance = (l_f32ClosestX - l_f32SkullCenterX) * (l_f32ClosestX - l_f32SkullCenterX) +
						(l_f32ClosestY - l_f32SkullCenterY) * (l_f32ClosestY - l_f32SkullCenterY);

					//ensure corner lies within skull
					if(l_f32SquareDistance <= l_f32SquareRadius)
					{
						m_oSample2DCoordinates[l_ui32CurSample].first = j*m_ui32CellSize;
						m_oSample2DCoordinates[l_ui32CurSample].second= i*m_ui32CellSize;

						//normalized X, Y coords in (X, Y) plane
						l_f32X = (l_f32ClosestX - l_f32SkullCenterX) / (m_ui32SkullDiameter/2.f);
						l_f32Y = -(l_f32ClosestY - l_f32SkullCenterY) / (m_ui32SkullDiameter/2.f); //y axis down in 2D but up in 3D convention

						uint32 l_ui32BaseIndex = 3* l_ui32CurSample;

						//compute normalized coordinates to be fed to spherical spline algorithm
						if(m_ui32CurrentProjection == TopographicMap2DProjection_Axial)
						{
							//x and y computed directly from point position
							*(m_oSampleCoordinatesMatrix.getBuffer() + l_ui32BaseIndex) = l_f32X;
							*(m_oSampleCoordinatesMatrix.getBuffer() + l_ui32BaseIndex+1) = l_f32Y;
							//z = sqrt(1-x*x-y*y)
							*(m_oSampleCoordinatesMatrix.getBuffer() + l_ui32BaseIndex+2) = sqrt(1 -
								*(m_oSampleCoordinatesMatrix.getBuffer() + l_ui32BaseIndex) * *(m_oSampleCoordinatesMatrix.getBuffer() + l_ui32BaseIndex) -
								*(m_oSampleCoordinatesMatrix.getBuffer() + l_ui32BaseIndex+1) * *(m_oSampleCoordinatesMatrix.getBuffer() + l_ui32BaseIndex+1));
						}
						else //radial
						{
							//compute arc length and theta angle
							float32 l_f32ArcLength = sqrtf(l_f32X*l_f32X + l_f32Y*l_f32Y);
							float32 l_f32Theta = l_f32ArcLength * 3.1415926535f / 2;
							//x and y computed from radial projection formula
							float32 l_f32ScalingFactor = sinf(l_f32Theta) / l_f32ArcLength;
							*(m_oSampleCoordinatesMatrix.getBuffer() + l_ui32BaseIndex) = l_f32ScalingFactor * l_f32X;
							*(m_oSampleCoordinatesMatrix.getBuffer() + l_ui32BaseIndex+1) = l_f32ScalingFactor * l_f32Y;
							//z = cos(theta)
							*(m_oSampleCoordinatesMatrix.getBuffer() + l_ui32BaseIndex+2) = cosf(l_f32Theta);
						}

						l_ui32CurSample++;
					}
				}
			}
		}

		void CTopographicMap2DView::enableProjectionButtonSignals(boolean enable)
		{
			if(enable == true)
			{
				g_signal_connect(G_OBJECT(m_pProjectionButtons[TopographicMap2DProjection_Axial]),
					"toggled", G_CALLBACK (&CTopographicMap2DView::toggleAxialProjectionCB), this);
				g_signal_connect(G_OBJECT(m_pProjectionButtons[TopographicMap2DProjection_Radial]),
					"toggled", G_CALLBACK (&CTopographicMap2DView::toggleRadialProjectionCB), this);
			}
			else
			{
				g_signal_handlers_disconnect_by_func(G_OBJECT(m_pProjectionButtons[TopographicMap2DProjection_Axial]),
					(void*)(G_CALLBACK (&CTopographicMap2DView::toggleAxialProjectionCB)), this);
				g_signal_handlers_disconnect_by_func(G_OBJECT(m_pProjectionButtons[TopographicMap2DProjection_Radial]),
					(void*)(G_CALLBACK (&CTopographicMap2DView::toggleRadialProjectionCB)), this);
			}
		}

		void CTopographicMap2DView::setProjection(ETopographicMap2DProjection eProjection)
		{
			m_ui32CurrentProjection = eProjection;

			enableProjectionButtonSignals(false);

			for(ETopographicMap2DProjection eProjection = TopographicMap2DProjection_Axial;
				eProjection != TopographicMap2DProjection_NumProjection;
				eProjection = (ETopographicMap2DProjection)(eProjection + 1))
			{
				gtk_toggle_tool_button_set_active(
					m_pProjectionButtons[eProjection],
					(eProjection == m_ui32CurrentProjection) ? TRUE : FALSE);
			}

			enableProjectionButtonSignals(true);

			m_bNeedResize = true; //recompute sample points coordinates
		}

		void CTopographicMap2DView::toggleAxialProjectionCB(GtkWidget* pWidget, gpointer data)
		{
			CTopographicMap2DView* l_pTopographicMap2DView = reinterpret_cast<CTopographicMap2DView*>(data);
			l_pTopographicMap2DView->setProjection(TopographicMap2DProjection_Axial);
		}

		void CTopographicMap2DView::toggleRadialProjectionCB(GtkWidget* pWidget, gpointer data)
		{
			CTopographicMap2DView* l_pTopographicMap2DView = reinterpret_cast<CTopographicMap2DView*>(data);
			l_pTopographicMap2DView->setProjection(TopographicMap2DProjection_Radial);
		}
	};
};
