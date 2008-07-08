#include "ovpCTopographicMap3DView.h"

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
		CTopographicMap3DView::CTopographicMap3DView() :
			m_pGladeInterface(NULL),
			m_ui32CurrentViewport(TopographicMap3DViewport_Top)
		{
			//load the glade interface
			m_pGladeInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-TopographicMap3D.glade", NULL, NULL);

			if(!m_pGladeInterface)
			{
				g_warning("Couldn't load the interface!");
				return;
			}

			glade_xml_signal_autoconnect(m_pGladeInterface);
		}

		CTopographicMap3DView::~CTopographicMap3DView()
		{
			//unref the xml file as it's not needed anymore
			g_object_unref(G_OBJECT(m_pGladeInterface));
			m_pGladeInterface=NULL;
		}

		void CTopographicMap3DView::init()
		{
			/*
			g_signal_connect(G_OBJECT(m_pDrawingArea), "expose_event", G_CALLBACK(redrawCB), this);
			g_signal_connect(G_OBJECT(m_pDrawingArea), "size-allocate", G_CALLBACK(sizeAllocateCB), this);
			*/

			setCurrentViewport(TopographicMap3DViewport_Top);
		}

		void CTopographicMap3DView::getToolbar(GtkWidget*& pToolbarWidget)
		{
			pToolbarWidget = NULL;//glade_xml_get_widget(m_pGladeInterface, "Toolbar");
		}

		CTopographicMap3DView::ETopographicMap3DViewport CTopographicMap3DView::getCurrentViewport()
		{
			return m_ui32CurrentViewport;
		}

		void CTopographicMap3DView::setCurrentViewport(ETopographicMap3DViewport eViewport)
		{
			m_ui32CurrentViewport = eViewport;
		}
	};
};
