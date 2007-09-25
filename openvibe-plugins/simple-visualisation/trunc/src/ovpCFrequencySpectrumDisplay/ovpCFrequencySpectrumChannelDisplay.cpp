#include "ovpCFrequencySpectrumChannelDisplay.h"

#include <math.h>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

namespace
{
	uint32* getLookUpTable(void)
	{
		static uint32 l_pTable[1<<8];
		static boolean l_bInitialized=false;

		if(!l_bInitialized)
		{
			int i=0;
			for(i=0; i<64; i++)
			{
				l_pTable[i]=i;
			}
			for(i=0; i<64; i++)
			{
				l_pTable[i+64]=(i<<8)+(64+i);
			}
			for(i=0; i<64; i++)
			{
				l_pTable[i+128]=(i<<16)+((64+i)<<8)+(128+i);
			}
			for(i=0; i<64; i++)
			{
				l_pTable[i+192]=((64+i)<<16)+((128+i)<<8)+(192+i);
			}
			l_bInitialized=true;
		}

		return l_pTable;
	}
};

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		gboolean frequencySpectrumSizeAllocateCallback(GtkWidget *widget, GtkAllocation *allocation, gpointer data)
		{
			CFrequencySpectrumChannelDisplay * l_pDisplay = reinterpret_cast<CFrequencySpectrumChannelDisplay*>(data);

			//3bytes per pixel
			l_pDisplay->resizeRGBBuffer(allocation->width,allocation->height);

			return FALSE;
		}

		void CFrequencySpectrumChannelDisplay::resizeRGBBuffer(OpenViBE::uint32 ui32Width, OpenViBE::uint32 ui32Height)
		{
			//first delete the previously allocated buffer
			delete[] m_pRGBBuffer;

			m_ui32Rowstride = ((ui32Width*3)%4 == 0) ? (ui32Width*3) : ((((ui32Width*3)>>2)+1)<<2);

			m_pRGBBuffer = new guchar[m_ui32Rowstride*ui32Height*3];

			m_ui32RGBBufferWidth=ui32Width;
			m_ui32RGBBufferHeight=ui32Height;

			//g_object_unref(G_OBJECT(m_pPixmap));
			//m_pPixmap = gdk_pixmap_new(m_pDisplay->window, ui32Width, ui32Height, -1);
		}

		gboolean frequencySpectrumChannelDisplayExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data)
		{
			//redraw the ruler
			CFrequencySpectrumChannelDisplay * l_pDisplay = reinterpret_cast<CFrequencySpectrumChannelDisplay*>(data);
			l_pDisplay->redraw();

			//don't propagate this signal to the children if any
			return TRUE;
		}

		CFrequencySpectrumChannelDisplay::CFrequencySpectrumChannelDisplay(uint32 ui32Channel, CBufferDatabase& pDatabase) :
			m_pGraphicsContext(NULL),
			m_pDatabase(&pDatabase),
			m_ui32Channel(ui32Channel),
			m_f64MinimumValue(+DBL_MAX),
			m_f64MaximumValue(-DBL_MAX),
			m_f64Attenuation(1),
			m_pRGBBuffer(NULL),
			m_ui32RGBBufferWidth(0)
		{

			m_pDisplay = gtk_drawing_area_new();
			gtk_widget_set_size_request(m_pDisplay, 200, 30);

			g_signal_connect_after(G_OBJECT(m_pDisplay), "expose_event", G_CALLBACK(frequencySpectrumChannelDisplayExposeEventCallback), this);
			g_signal_connect(G_OBJECT(m_pDisplay), "size-allocate", G_CALLBACK(frequencySpectrumSizeAllocateCallback), this);

		}

		CFrequencySpectrumChannelDisplay::~CFrequencySpectrumChannelDisplay()
		{
			g_object_unref(G_OBJECT(m_pGraphicsContext));

			delete[] m_pRGBBuffer;
		}

		//! toggles (show/hide)
		void CFrequencySpectrumChannelDisplay::toggle(OpenViBE::boolean bActive)
		{
			if(bActive)
			{
				gtk_widget_show(m_pDisplay);
			}
			else
			{
				gtk_widget_hide(m_pDisplay);
			}
		}

		//! draws the frequency spectrum
		void CFrequencySpectrumChannelDisplay::update()
		{
			if(!m_pGraphicsContext)
			{
				//creates the GC
				m_pGraphicsContext = gdk_gc_new(m_pDisplay->window);

				//m_pPixmap = gdk_pixmap_new(m_pDisplay->window, 200, 30, -1);

			}

			if(!m_pDatabase || m_pDatabase->m_oSampleBuffers.size()==0 )//|| !m_pRGBBuffer)
			{
				return;
			}

			gint l_iWidth = 0;
			gint l_iHeight = 0;

			uint32 l_ui32FrequencyCount = static_cast<uint32>(m_pDatabase->m_pDimmensionSizes[1]);

			//gets the size of the drawing area
			gdk_drawable_get_size(m_pDisplay->window, &l_iWidth, &l_iHeight);

			uint64 l_ui64NumberOfBufferToDisplay = m_pDatabase->m_ui64NumberOfBufferToDisplay;

			float64 l_f64WidthPerPoint = static_cast<float64>(l_iWidth) / static_cast<float64>(l_ui64NumberOfBufferToDisplay);
			float64 l_f64HeightPerPoint = static_cast<float64>(l_iHeight) / static_cast<float64>(l_ui32FrequencyCount);

			int64 l_i64BaseX = static_cast<int64>(l_iWidth - (m_pDatabase->m_oSampleBuffers.size() * l_f64WidthPerPoint));

			if(l_i64BaseX<0 || l_i64BaseX < l_f64WidthPerPoint)
			{
				l_i64BaseX = 0;
			}

			GdkColor l_oCurrentColor;
			l_oCurrentColor.pixel = 0;

			//computes the color
			float64 l_f64Max;// = m_pDatabase->m_f64MaximumValue;
			float64 l_f64Min;// = m_pDatabase->m_f64MinimumValue;

			//test
			float64 l_f64CurrentBufferMax;
			float64 l_f64CurrentBufferMin;

			m_pDatabase->getLastBufferChannelLocalMinMaxValue(m_ui32Channel, l_f64CurrentBufferMin, l_f64CurrentBufferMax);
			//m_pDatabase->getLastBufferMinMaxValue(l_f64CurrentBufferMin, l_f64CurrentBufferMax);

			l_f64Max = (m_f64MaximumValue < l_f64CurrentBufferMax) ?
				l_f64CurrentBufferMax :
				(m_f64Attenuation * m_f64MaximumValue) + (1-m_f64Attenuation)*l_f64CurrentBufferMax;

			l_f64Min = (m_f64MinimumValue > l_f64CurrentBufferMin) ?
				l_f64CurrentBufferMin :
				(m_f64Attenuation * m_f64MinimumValue) + (1-m_f64Attenuation)*l_f64CurrentBufferMin;

			m_f64MinimumValue = l_f64Min;
			m_f64MaximumValue = l_f64Max;

			//clear the undrawn part of the buffer
			drawBoxToBuffer(0, 0, l_i64BaseX, l_iHeight, 0xFF, 0xFF, 0xFF);

			for(size_t j=0 ; j<m_pDatabase->m_oSampleBuffers.size() ; j++)
			{
				//gets a pointer to this channels' samples data in the current buffer
				const float64 * l_pCurrentChannelSampleBuffer = (m_pDatabase->m_oSampleBuffers)[j] + (m_ui32Channel * l_ui32FrequencyCount);

				//calculates the coordinates of the point in the new base
				for(uint32 f = 0 ; f<l_ui32FrequencyCount ; f++)
				{
					//computes the box coordinates
					gint l_iXposition = (gint) (l_i64BaseX + (j *  l_f64WidthPerPoint));
					gint l_iYPosition = (gint) (l_iHeight - ((f+1) * l_f64HeightPerPoint));

					gint l_iPointW = (gint) (l_i64BaseX + ((j+1) *  l_f64WidthPerPoint) - l_iXposition);
					gint l_iPointH = (gint) (l_iHeight - (f * l_f64HeightPerPoint) - l_iYPosition);

					int32 l_i32ColorIndex=static_cast<int32>(((l_pCurrentChannelSampleBuffer[f] - l_f64Min)*(1<<8))/(l_f64Max - l_f64Min));
					if(l_i32ColorIndex>0xff) l_i32ColorIndex=0xff;
					if(l_i32ColorIndex<0x00) l_i32ColorIndex=0x00;
					uint8 l_ui8ColorR=(getLookUpTable()[l_i32ColorIndex]    )&0xff;
					uint8 l_ui8ColorG=(getLookUpTable()[l_i32ColorIndex]>>8 )&0xff;
					uint8 l_ui8ColorB=(getLookUpTable()[l_i32ColorIndex]>>16)&0xff;

					drawBoxToBuffer(l_iXposition, l_iYPosition, l_iPointW, l_iPointH, l_ui8ColorR, l_ui8ColorG, l_ui8ColorB);

				}
			}

//			gdk_draw_rgb_image(m_pPixmap, m_pDisplay->style->fg_gc[GTK_WIDGET_STATE (m_pDisplay)], 0, 0, m_ui32RGBBufferWidth, m_ui32RGBBufferHeight, GDK_RGB_DITHER_NONE, m_pRGBBuffer, m_ui32Rowstride);
		}

		void CFrequencySpectrumChannelDisplay::redraw()
		{

			gdk_draw_rgb_image(m_pDisplay->window, m_pDisplay->style->fg_gc[GTK_WIDGET_STATE (m_pDisplay)], 0, 0, m_ui32RGBBufferWidth, m_ui32RGBBufferHeight, GDK_RGB_DITHER_NONE, m_pRGBBuffer, m_ui32Rowstride);//m_ui32RGBBufferWidth*3);

			//gdk_draw_drawable(m_pDisplay->window, m_pDisplay->style->fg_gc[GTK_WIDGET_STATE (m_pDisplay)], m_pPixmap,
			//		0,0,0,0, m_ui32RGBBufferWidth, m_ui32RGBBufferHeight);

		}

	}
}
