#include "ovpCVoxelView.h"

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
		CVoxelView::CVoxelView() :
			m_pGladeInterface(NULL)/*,
			m_ui32CurrentViewport(VoxelViewport_Top)						*/
		{
			//load the glade interface
			m_pGladeInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-VoxelDisplay.glade", NULL, NULL);

			if(!m_pGladeInterface)
			{
				g_warning("Couldn't load the interface!");
				return;
			}

			glade_xml_signal_autoconnect(m_pGladeInterface);
		}

		CVoxelView::~CVoxelView()
		{
			//unref the xml file as it's not needed anymore
			g_object_unref(G_OBJECT(m_pGladeInterface));
			m_pGladeInterface=NULL;
		}

		void CVoxelView::init()
		{
			/*
			g_signal_connect(G_OBJECT(m_pDrawingArea), "expose_event", G_CALLBACK(redrawCB), this);
			g_signal_connect(G_OBJECT(m_pDrawingArea), "size-allocate", G_CALLBACK(sizeAllocateCB), this);
			*/

			//setCurrentViewport(VoxelViewport_Top);
		}

		void CVoxelView::getToolbar(GtkWidget*& pToolbarWidget)
		{
			pToolbarWidget = NULL;//glade_xml_get_widget(m_pGladeInterface, "Toolbar");
		}
/*
		CVoxelView::EVoxelViewport CVoxelView::getCurrentViewport()
		{
			return m_ui32CurrentViewport;
		}

		void CVoxelView::setCurrentViewport(EVoxelViewport eViewport)
		{
			m_ui32CurrentViewport = eViewport;
		}*/
	};
};
