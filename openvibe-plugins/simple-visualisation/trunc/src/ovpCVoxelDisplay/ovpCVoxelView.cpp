#include "ovpCVoxelView.h"

#include <iostream>

#include <sstream>

#include <math.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		static void setVoxelObjectCallback(GtkButton *button, gpointer data);
		static void toggleColorModificationCallback(::GtkWidget* pWidget, gpointer data);
		static void toggleTransparencyModificationCallback(::GtkWidget* pWidget, gpointer data);
		static void toggleSizeModificationCallback(::GtkWidget* pWidget, gpointer data);
		static gboolean setMinScaleFactorCallback(::GtkWidget* pWidget, gpointer data);
		static gboolean setMaxScaleFactorCallback(::GtkWidget* pWidget, gpointer data);
		static void setVoxelDisplayThresholdCallback(::GtkRange *range, gpointer data);
		static void setSkullOpacityCallback(::GtkRange *range, gpointer data);
		static void toggleFreezeCallback(GtkButton *button, gpointer data);
		static void repositionCameraCallback(GtkButton *button, gpointer data);

		CVoxelView::CVoxelView(CVoxelDisplay& rVoxelDisplay) :
			m_rVoxelDisplay(rVoxelDisplay),
			m_pGladeInterface(NULL),
			m_pCubeButton(NULL),
			m_pSphereButton(NULL)
		{
			//load the glade interface
			m_pGladeInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-VoxelDisplay.glade", NULL, NULL);

			if(!m_pGladeInterface)
			{
				g_warning("Couldn't load the interface!");
				return;
			}

			glade_xml_signal_autoconnect(m_pGladeInterface);

			//toolbar
			//-------

			//voxel object buttons
			m_pCubeButton = GTK_RADIO_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "CubeButton"));
			m_pSphereButton = GTK_RADIO_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "SphereButton"));

			g_signal_connect(G_OBJECT(m_pCubeButton), "toggled", G_CALLBACK(setVoxelObjectCallback), this);
			g_signal_connect(G_OBJECT(m_pSphereButton), "toggled", G_CALLBACK(setVoxelObjectCallback), this);

			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "ModifyColorToolButton")), "toggled", G_CALLBACK(toggleColorModificationCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "ModifyTransparencyToolButton")), "toggled", G_CALLBACK(toggleTransparencyModificationCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "ModifySizeToolButton")), "toggled", G_CALLBACK(toggleSizeModificationCallback), this);

			//min voxel scale factor
			GtkSpinButton* l_pMinScaleFactorSpinButton = GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "MinScaleFactorSpinButton"));
			float64 l_f64MinScaleFactor = 1;
			m_rVoxelDisplay.setMinScaleFactor(l_f64MinScaleFactor);
			gtk_spin_button_configure(
				l_pMinScaleFactorSpinButton,
				GTK_ADJUSTMENT(gtk_adjustment_new(
					l_f64MinScaleFactor, //initial value
					0, //lower
					1, //upper
					0.1, //step increment
					0.5, //page increment
					1)), //page size
					0.1, //climb rate
					1); //digits
			g_signal_connect(G_OBJECT(l_pMinScaleFactorSpinButton), "value-changed", G_CALLBACK(setMinScaleFactorCallback), this);

			//max voxel scale factor
			GtkSpinButton* l_pMaxScaleFactorSpinButton = GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "MaxScaleFactorSpinButton"));
			float64 l_f64MaxScaleFactor = 2;
			m_rVoxelDisplay.setMaxScaleFactor(l_f64MaxScaleFactor);
			gtk_spin_button_configure(
				l_pMaxScaleFactorSpinButton,
				GTK_ADJUSTMENT(gtk_adjustment_new(
					l_f64MaxScaleFactor, //initial value
					0, //lower
					4, //upper
					0.1, //step increment
					0.5, //page increment
					1)), //page size
					0.1, //climb rate
					1); //digits
			g_signal_connect(G_OBJECT(l_pMaxScaleFactorSpinButton), "value-changed", G_CALLBACK(setMaxScaleFactorCallback), this);

			//voxel display threshold slider
			GtkWidget* l_pDisplayThresholdScale = gtk_hscale_new_with_range(0.0, 1.0, 0.05);
			gtk_range_set_value(GTK_RANGE(l_pDisplayThresholdScale), 0);
			gtk_scale_set_value_pos(GTK_SCALE(l_pDisplayThresholdScale), GTK_POS_TOP);
			gtk_range_set_update_policy(GTK_RANGE(l_pDisplayThresholdScale), GTK_UPDATE_CONTINUOUS);
			gtk_widget_set_size_request(l_pDisplayThresholdScale, 100, -1);
			gtk_widget_show_all(l_pDisplayThresholdScale);
			g_signal_connect(G_OBJECT(l_pDisplayThresholdScale), "value_changed", G_CALLBACK(setVoxelDisplayThresholdCallback), this);

			//replace existing scale (which somehow can't be used) with the newly created one
			GtkWidget* l_pOldScale = glade_xml_get_widget(m_pGladeInterface, "DisplayThresholdScale");
			GtkWidget* l_pScaleParent = gtk_widget_get_parent(l_pOldScale);
			if(l_pScaleParent != NULL && GTK_IS_CONTAINER(l_pScaleParent))
			{
				gtk_container_remove(GTK_CONTAINER(l_pScaleParent), l_pOldScale);
				if(GTK_IS_BOX(l_pScaleParent))
				{
					gtk_box_pack_start(GTK_BOX(l_pScaleParent), l_pDisplayThresholdScale, TRUE, TRUE, 0);
					gtk_box_reorder_child(GTK_BOX(l_pScaleParent), l_pDisplayThresholdScale, 0);
				}
			}

			//skull opacity slider
			GtkWidget* l_pSkullOpacityScale = gtk_hscale_new_with_range(0.0, 1.0, 0.05);
			gtk_range_set_value(GTK_RANGE(l_pSkullOpacityScale), 0);
			gtk_scale_set_value_pos(GTK_SCALE(l_pSkullOpacityScale), GTK_POS_TOP);
			gtk_range_set_update_policy(GTK_RANGE(l_pSkullOpacityScale), GTK_UPDATE_CONTINUOUS);
			gtk_widget_set_size_request(l_pSkullOpacityScale, 100, -1);
			gtk_widget_show_all(l_pSkullOpacityScale);
			g_signal_connect(G_OBJECT(l_pSkullOpacityScale), "value_changed", G_CALLBACK(setSkullOpacityCallback), this);

			//replace existing scale (which somehow can't be used) with the newly created one
			l_pOldScale = glade_xml_get_widget(m_pGladeInterface, "SkullOpacityScale");
			l_pScaleParent = gtk_widget_get_parent(l_pOldScale);
			if(l_pScaleParent != NULL && GTK_IS_CONTAINER(l_pScaleParent))
			{
				gtk_container_remove(GTK_CONTAINER(l_pScaleParent), l_pOldScale);
				if(GTK_IS_BOX(l_pScaleParent))
				{
					gtk_box_pack_start(GTK_BOX(l_pScaleParent), l_pSkullOpacityScale, TRUE, TRUE, 0);
					gtk_box_reorder_child(GTK_BOX(l_pScaleParent), l_pSkullOpacityScale, 0);
				}
			}

			//freeze button
			GtkToggleToolButton* l_pFreezeButton = GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "FreezeButton"));
			gtk_toggle_tool_button_set_active(l_pFreezeButton, false);
			g_signal_connect(G_OBJECT(l_pFreezeButton), "toggled", G_CALLBACK(toggleFreezeCallback), this);

			//reposition camera
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "RepositionCamera")),	"clicked", G_CALLBACK(repositionCameraCallback), this);
		}

		CVoxelView::~CVoxelView()
		{
			//unref the xml file as it's not needed anymore
			g_object_unref(G_OBJECT(m_pGladeInterface));
			m_pGladeInterface=NULL;
		}

		boolean CVoxelView::init()
		{
			//setup database parameters here
			return true;
		}

		boolean CVoxelView::redraw()
		{
			//drawing handled by CVoxelDisplay
			return true;
		}

		void CVoxelView::getToolbar(GtkWidget*& pToolbarWidget)
		{
			pToolbarWidget = glade_xml_get_widget(m_pGladeInterface, "Toolbar");
		}

		void CVoxelView::setVoxelObjectCB(GtkWidget* pWidget)
		{
			if(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(pWidget)) == FALSE)
			{
				return;
			}

			if(pWidget == GTK_WIDGET(m_pCubeButton))
			{
				m_rVoxelDisplay.setVoxelObject(Standard3DObject_Cube);
			}
			else if(pWidget == GTK_WIDGET(m_pSphereButton))
			{
				m_rVoxelDisplay.setVoxelObject(Standard3DObject_Sphere);
			}
		}

		void CVoxelView::toggleColorModificationCB(boolean bModifyColor)
		{
			m_rVoxelDisplay.toggleColorModification(bModifyColor);
		}

		void CVoxelView::toggleTransparencyModificationCB(boolean bModifyTransparency)
		{
			m_rVoxelDisplay.toggleTransparencyModification(bModifyTransparency);
		}

		void CVoxelView::toggleSizeModificationCB(boolean bModifySize)
		{
			m_rVoxelDisplay.toggleSizeModification(bModifySize);
		}

		void CVoxelView::setMinVoxelScaleFactorCB(GtkSpinButton* pButton)
		{
			//ensure minimum value is smaller than maximum value
			float64 l_f64MinScaleFactor = gtk_spin_button_get_value(pButton);
			GtkSpinButton* l_pMaxButton = GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "MaxScaleFactorSpinButton"));
			if(l_f64MinScaleFactor > gtk_spin_button_get_value(l_pMaxButton))
			{
				l_f64MinScaleFactor = gtk_spin_button_get_value(l_pMaxButton);
			}

			//set min value
			m_rVoxelDisplay.setMinScaleFactor(l_f64MinScaleFactor);
		}

		void CVoxelView::setMaxVoxelScaleFactorCB(GtkSpinButton* pButton)
		{
			//ensure maximum value is bigger than minimum value
			float64 l_f64MaxScaleFactor = gtk_spin_button_get_value(pButton);
			GtkSpinButton* l_pMinButton = GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "MinScaleFactorSpinButton"));
			if(l_f64MaxScaleFactor < gtk_spin_button_get_value(l_pMinButton))
			{
				l_f64MaxScaleFactor = gtk_spin_button_get_value(l_pMinButton);
			}

			//set max value
			m_rVoxelDisplay.setMaxScaleFactor(l_f64MaxScaleFactor);
		}

		void CVoxelView::setVoxelDisplayThresholdCB(float64 f64Threshold)
		{
			m_rVoxelDisplay.setVoxelDisplayThreshold(f64Threshold);
		}

		void CVoxelView::setSkullOpacityCB(float64 f64Opacity)
		{
			m_rVoxelDisplay.setSkullOpacity(f64Opacity);
		}

		void CVoxelView::setPausedCB(boolean bPaused)
		{
			m_rVoxelDisplay.setPaused(bPaused);
		}

		void CVoxelView::repositionCameraCB()
		{
			m_rVoxelDisplay.repositionCamera();
		}

		//CALLBACKS

		void setVoxelObjectCallback(GtkButton *pButton, gpointer data)
		{
			CVoxelView* l_pVoxelView = reinterpret_cast<CVoxelView*>(data);
			l_pVoxelView->setVoxelObjectCB(GTK_WIDGET(pButton));
		}

		void toggleColorModificationCallback(GtkWidget* pWidget, gpointer data)
		{
			CVoxelView* l_pVoxelView = reinterpret_cast<CVoxelView*>(data);
			l_pVoxelView->toggleColorModificationCB(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(pWidget)) != 0);
		}

		void toggleTransparencyModificationCallback(GtkWidget* pWidget, gpointer data)
		{
			CVoxelView* l_pVoxelView = reinterpret_cast<CVoxelView*>(data);
			l_pVoxelView->toggleTransparencyModificationCB(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(pWidget)) != 0);
		}

		void toggleSizeModificationCallback(GtkWidget* pWidget, gpointer data)
		{
			CVoxelView* l_pVoxelView = reinterpret_cast<CVoxelView*>(data);
			l_pVoxelView->toggleSizeModificationCB(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(pWidget)) != 0);
		}

		gboolean setMinScaleFactorCallback(GtkWidget* pWidget, gpointer data)
		{
			CVoxelView* l_pView = reinterpret_cast<CVoxelView*>(data);
			l_pView->setMinVoxelScaleFactorCB(GTK_SPIN_BUTTON(pWidget));
			return FALSE;
		}

		gboolean setMaxScaleFactorCallback(GtkWidget* pWidget, gpointer data)
		{
			CVoxelView* l_pView = reinterpret_cast<CVoxelView*>(data);
			l_pView->setMaxVoxelScaleFactorCB(GTK_SPIN_BUTTON(pWidget));
			return FALSE;
		}

		void setVoxelDisplayThresholdCallback(::GtkRange *pRange, gpointer data)
		{
			CVoxelView* l_pVoxelView = reinterpret_cast<CVoxelView*>(data);
			l_pVoxelView->setVoxelDisplayThresholdCB(gtk_range_get_value(pRange));
		}

		void setSkullOpacityCallback(::GtkRange *pRange, gpointer data)
		{
			CVoxelView* l_pVoxelView = reinterpret_cast<CVoxelView*>(data);
			l_pVoxelView->setSkullOpacityCB(gtk_range_get_value(pRange));
		}

		void toggleFreezeCallback(GtkButton *button, gpointer data)
		{
			CVoxelView* l_pVoxelView = reinterpret_cast<CVoxelView*>(data);
			l_pVoxelView->setPausedCB(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(button)) != 0);
		}

		void repositionCameraCallback(GtkButton *pButton, gpointer data)
		{
			CVoxelView* l_pVoxelView = reinterpret_cast<CVoxelView*>(data);
			l_pVoxelView->repositionCameraCB();
		}
	};
};
