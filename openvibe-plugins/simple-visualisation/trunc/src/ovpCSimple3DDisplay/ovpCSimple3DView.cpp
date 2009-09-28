#include "ovpCSimple3DView.h"

#include <iostream>

#include <sstream>

#include <cmath>
#include <cstdlib>
#include <cstring>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;
using namespace OpenViBE::Kernel;
using namespace OpenViBEToolkit;
using namespace std;

//REMOVE ME
GtkWidget* customWidgetHandler(GladeXML *xml,gchar *func_name,gchar *name,gchar *string1,gchar *string2,gint int1,gint int2,gpointer user_data)
{
	return gtk_button_new();
}

extern "C" GtkWidget* custom_func(char*, char*, char*, int, int)
{
	return gtk_label_new("Toto");
	/*
	try
	{
		Gtk::Widget *const widget = new Regexxer::FileTree();
		widget->show();
		return Gtk::manage(widget)->gobj();
	}
	catch (...)
	{
		g_return_val_if_reached(0);
	}*/
}
///////////////

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		void createObjectButtonCallback(GtkButton *button, gpointer data);
		void deleteObjectButtonCallback(GtkButton *button, gpointer data);
		void setPositionButtonCallback(GtkButton *button, gpointer data);
		void setOrientationButtonCallback(GtkButton *button, gpointer data);
		void setScaleButtonCallback(GtkButton *button, gpointer data);
		void setColorButtonCallback(GtkButton *button, gpointer data);
		void repositionCameraButtonCallback(GtkButton *button, gpointer data);

		CSimple3DView::CSimple3DView(CSimple3DDatabase& rSimple3DDatabase) :
			m_rSimple3DDatabase(rSimple3DDatabase),
			m_pGladeInterface(NULL),
			m_pCreateObjectTable(NULL),
			m_pCreateObjectName(NULL),
			m_pCreateObjectStandardRadioButton(NULL),
			m_pStandardObjectComboBox(NULL),
			m_pCustomObjectEntry(NULL),
			m_pDeleteObjectTable(NULL),
			m_pDeleteObjectName(NULL),
			m_pSetPositionTable(NULL),
			m_pPositionObjectName(NULL),
			m_pXPositionEntry(NULL),
			m_pYPositionEntry(NULL),
			m_pZPositionEntry(NULL),
			m_pSetScaleTable(NULL),
			m_pScaleObjectName(NULL),
			m_pXScaleEntry(NULL),
			m_pYScaleEntry(NULL),
			m_pZScaleEntry(NULL),
			m_pSetColorTable(NULL),
			m_pColorObjectName(NULL),
			m_pRedEntry(NULL),
			m_pGreenEntry(NULL),
			m_pBlueEntry(NULL),
			m_pTransparencyEntry(NULL)
		{
			//REMOVE ME
			//glade_set_custom_handler(customWidgetHandler, NULL);
			/////////////

			//load the glade interface
			m_pGladeInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-Simple3DDisplay.glade", NULL, NULL);

			if(!m_pGladeInterface)
			{
				g_warning("Couldn't load the interface!");
				return;
			}

			glade_xml_signal_autoconnect(m_pGladeInterface);

			//toolbar buttons connections
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "CreateObject")),
				"clicked", G_CALLBACK(createObjectButtonCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DeleteObject")),
				"clicked", G_CALLBACK(deleteObjectButtonCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SetPosition")),
				"clicked", G_CALLBACK(setPositionButtonCallback), this);/*
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SetOrientation")),
				"clicked", G_CALLBACK(setOrientationButtonCallback), this);*/
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SetScale")),
				"clicked", G_CALLBACK(setScaleButtonCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SetColor")),
				"clicked", G_CALLBACK(setColorButtonCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "RepositionCamera")),
				"clicked", G_CALLBACK(repositionCameraButtonCallback), this);

			//create object connections
			//-------------------------
			m_pCreateObjectTable = GTK_TABLE(glade_xml_get_widget(m_pGladeInterface, "CreateObjectTable"));
			//remove table from its parent
			gtk_object_ref(GTK_OBJECT(m_pCreateObjectTable));
			GtkWidget* l_pTableParent = gtk_widget_get_parent(GTK_WIDGET(m_pCreateObjectTable));
			if(l_pTableParent != NULL && GTK_IS_CONTAINER(l_pTableParent))
			{
				gtk_container_remove(GTK_CONTAINER(l_pTableParent), GTK_WIDGET(m_pCreateObjectTable));
			}
			m_pCreateObjectName = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "CreateObjectName"));
			m_pCreateObjectStandardRadioButton = GTK_RADIO_BUTTON(glade_xml_get_widget(m_pGladeInterface, "CreateObjectStandard"));
			std::vector<std::string> l_oStandardObjectComboBoxEntries;
			l_oStandardObjectComboBoxEntries.push_back("Sphere");
			l_oStandardObjectComboBoxEntries.push_back("Cone");
			m_pStandardObjectComboBox = createTextComboBox("StandardObjectComboBoxTable", l_oStandardObjectComboBoxEntries);
			m_pCustomObjectEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "CustomObjectEntry"));

			//delete object connections
			//-------------------------
			m_pDeleteObjectTable = GTK_TABLE(glade_xml_get_widget(m_pGladeInterface, "DeleteObjectTable"));
			//remove table from its parent
			gtk_object_ref(GTK_OBJECT(m_pDeleteObjectTable));
			l_pTableParent = gtk_widget_get_parent(GTK_WIDGET(m_pDeleteObjectTable));
			if(l_pTableParent != NULL && GTK_IS_CONTAINER(l_pTableParent))
			{
				gtk_container_remove(GTK_CONTAINER(l_pTableParent), GTK_WIDGET(m_pDeleteObjectTable));
			}
			m_pDeleteObjectName = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "DeleteObjectName"));

			//set position connections
			//------------------------
			m_pSetPositionTable = GTK_TABLE(glade_xml_get_widget(m_pGladeInterface, "SetPositionTable"));
			//remove table from its parent
			gtk_object_ref(GTK_OBJECT(m_pSetPositionTable));
			l_pTableParent = gtk_widget_get_parent(GTK_WIDGET(m_pSetPositionTable));
			if(l_pTableParent != NULL && GTK_IS_CONTAINER(l_pTableParent))
			{
				gtk_container_remove(GTK_CONTAINER(l_pTableParent), GTK_WIDGET(m_pSetPositionTable));
			}
			m_pPositionObjectName = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "PositionObjectName"));
			m_pXPositionEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "XPositionEntry"));
			m_pYPositionEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "YPositionEntry"));
			m_pZPositionEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "ZPositionEntry"));

			//set orientation connections
			//---------------------------
			//TODO!

			//set scale connections
			//---------------------
			m_pSetScaleTable = GTK_TABLE(glade_xml_get_widget(m_pGladeInterface, "SetScaleTable"));
			//remove table from its parent
			gtk_object_ref(GTK_OBJECT(m_pSetScaleTable));
			l_pTableParent = gtk_widget_get_parent(GTK_WIDGET(m_pSetScaleTable));
			if(l_pTableParent != NULL && GTK_IS_CONTAINER(l_pTableParent))
			{
				gtk_container_remove(GTK_CONTAINER(l_pTableParent), GTK_WIDGET(m_pSetScaleTable));
			}
			m_pScaleObjectName = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "ScaleObjectName"));
			m_pXScaleEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "XScaleEntry"));
			m_pYScaleEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "YScaleEntry"));
			m_pZScaleEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "ZScaleEntry"));

			//set color connections
			//---------------------
			m_pSetColorTable = GTK_TABLE(glade_xml_get_widget(m_pGladeInterface, "SetColorTable"));
			//remove table from its parent
			gtk_object_ref(GTK_OBJECT(m_pSetColorTable));
			l_pTableParent = gtk_widget_get_parent(GTK_WIDGET(m_pSetColorTable));
			if(l_pTableParent != NULL && GTK_IS_CONTAINER(l_pTableParent))
			{
				gtk_container_remove(GTK_CONTAINER(l_pTableParent), GTK_WIDGET(m_pSetColorTable));
			}
			m_pColorObjectName = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "ColorObjectName"));
			m_pSetMaterialColorRadioButton = GTK_RADIO_BUTTON(glade_xml_get_widget(m_pGladeInterface, "SetMaterialColorRadioButton"));
			m_pRedEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "RedEntry"));
			m_pGreenEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "GreenEntry"));
			m_pBlueEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "BlueEntry"));
			m_pTransparencyEntry = GTK_ENTRY(glade_xml_get_widget(m_pGladeInterface, "TransparencyEntry"));

			//REMOVE ME
			//GtkWidget* custom = glade_xml_get_widget(m_pGladeInterface, "customwidget");
			///////////
		}

		CSimple3DView::~CSimple3DView()
		{
			//unref the xml file as it's not needed anymore
			g_object_unref(G_OBJECT(m_pGladeInterface));
			m_pGladeInterface=NULL;
		}

		void CSimple3DView::init()
		{
			/*
			m_pDrawingArea = glade_xml_get_widget(m_pGladeInterface, "TopographicMap2DDrawingArea");

			gtk_widget_set_double_buffered(m_pDrawingArea, TRUE);

			g_signal_connect(G_OBJECT(m_pDrawingArea), "expose_event", G_CALLBACK(redrawCB), this);
			g_signal_connect(G_OBJECT(m_pDrawingArea), "size-allocate", G_CALLBACK(sizeAllocateCB), this);

			gtk_widget_show(m_pDrawingArea);
			*/
		}

		void CSimple3DView::redraw()
		{
			//if(m_pDrawingArea != NULL && GTK_WIDGET_VISIBLE(m_pDrawingArea))
			{
				//gdk_window_invalidate_rect(m_pDrawingArea->window,	NULL,	true);
			}
		}

		void CSimple3DView::getToolbar(GtkWidget*& pToolbarWidget)
		{
			pToolbarWidget = glade_xml_get_widget(m_pGladeInterface, "Toolbar");
		}
/*
		void CSimple3DView::refreshObjectComboBox(GtkComboBox*& pComboBox, guint left_attach, guint right_attach,	guint top_attach, guint bottom_attach)
		{
			//clear combo box
			//replaceComboBoxWithTextComboBox(pComboBox, left_attach, right_attach, top_attach, bottom_attach);

			//fill it again
			std::map<OpenViBE::CString, OpenViBE::CIdentifier>::iterator it;
			for(it = m_mObjects.begin(); it != m_mObjects.end(); it++)
			{
				gtk_combo_box_append_text(pComboBox, it->first);
			}
		}

		void CSimple3DView::refreshObjectComboBoxes()
		{
			refreshObjectComboBox(m_pDeleteObjectComboBox, 1, 2, 0, 1);
		}*/

		GtkComboBox* CSimple3DView::createTextComboBox(const gchar* pParentTableName, const std::vector<std::string>& rComboBoxEntries)
		{
			//retrieve parent table
			GtkTable* l_pTable = GTK_TABLE(glade_xml_get_widget(m_pGladeInterface, pParentTableName));
			if(l_pTable == NULL)
			{
				return NULL;
			}

			GtkComboBox* l_pComboBox = GTK_COMBO_BOX(gtk_combo_box_new_text());
			unsigned int i=0;
			while(i<rComboBoxEntries.size())
			{
				gtk_combo_box_append_text(l_pComboBox, rComboBoxEntries[i].c_str());
				i++;
			}

			gtk_table_attach_defaults(l_pTable, GTK_WIDGET(l_pComboBox), 0, 1, 0, 1);

			return l_pComboBox;
		}

		gint CSimple3DView::showDialog(GtkTable* pTable, const gchar* pTitle)
		{
			if(pTable == NULL)
			{
				return GTK_RESPONSE_NONE;
			}

			//create input dialog
			GtkWidget* l_pDialog = gtk_dialog_new_with_buttons(
				pTitle,
				NULL, //parentwindow
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_STOCK_OK,
				GTK_RESPONSE_ACCEPT,
				GTK_STOCK_CANCEL,
				GTK_RESPONSE_REJECT,
				NULL);

			//display it at cursor position
			//gtk_window_set_position(GTK_WINDOW(l_pDialog), GTK_WIN_POS_CENTER_ON_PARENT);

			//insert table and show it
			gtk_container_add(GTK_CONTAINER (GTK_DIALOG(l_pDialog)->vbox), GTK_WIDGET(pTable));

			gtk_widget_show_all(GTK_WIDGET(pTable));

			gint l_i32Response = gtk_dialog_run(GTK_DIALOG(l_pDialog));

			//remove table from dialog box
			gtk_container_remove(GTK_CONTAINER(GTK_DIALOG(l_pDialog)->vbox), GTK_WIDGET(pTable));

			//destroy dialog
			gtk_widget_destroy(GTK_WIDGET(l_pDialog));

			return l_i32Response;
		}

		void CSimple3DView::createVisualObjectCB()
		{
			gint l_i32Response = showDialog(m_pCreateObjectTable, "Create a new object");

			if(l_i32Response == GTK_RESPONSE_ACCEPT)
			{
				const gchar* l_pCreateObjectName = gtk_entry_get_text(m_pCreateObjectName);

				//create standard object
				if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_pCreateObjectStandardRadioButton)) == (gboolean)TRUE)
				{
					if(gtk_combo_box_get_active(m_pStandardObjectComboBox) == 0)
					{
						m_rSimple3DDatabase.createVisualObject(l_pCreateObjectName, Standard3DObject_Sphere);
					}
					else if(gtk_combo_box_get_active(m_pStandardObjectComboBox) == 1)
					{
						m_rSimple3DDatabase.createVisualObject(l_pCreateObjectName, Standard3DObject_Cone);
					}
				}
				else //custom object
				{
					const gchar* l_pCustomObjectText = gtk_entry_get_text(m_pCustomObjectEntry);
					m_rSimple3DDatabase.createVisualObject(l_pCreateObjectName, l_pCustomObjectText);
				}
			}
		}

		void CSimple3DView::deleteVisualObjectCB()
		{
			gint l_i32Response = showDialog(m_pDeleteObjectTable, "Delete an object");

			if(l_i32Response == GTK_RESPONSE_ACCEPT)
			{
				const gchar* l_pDeleteObjectName = gtk_entry_get_text(m_pDeleteObjectName);

				m_rSimple3DDatabase.deleteVisualObject(l_pDeleteObjectName);
			}
		}

		void CSimple3DView::setVisualObjectPositionCB()
		{
			gint l_i32Response = showDialog(m_pSetPositionTable, "Set the position of an object");

			if(l_i32Response == GTK_RESPONSE_ACCEPT)
			{
				const gchar* l_pPositionObjectName = gtk_entry_get_text(m_pPositionObjectName);
				float32 l_pPosition[3];
				memset(l_pPosition, 0, 3*sizeof(float32));
				const gchar* text = gtk_entry_get_text(m_pXPositionEntry);
				sscanf(text, "%f", l_pPosition);
				text = gtk_entry_get_text(m_pYPositionEntry);
				sscanf(text, "%f", l_pPosition+1);
				text = gtk_entry_get_text(m_pZPositionEntry);
				sscanf(text, "%f", l_pPosition+2);
				m_rSimple3DDatabase.setVisualObjectPosition(l_pPositionObjectName, l_pPosition);
			}
		}

		void CSimple3DView::setVisualObjectOrientationCB()
		{
			//TODO
		}

		void CSimple3DView::setVisualObjectScaleCB()
		{
			gint l_i32Response = showDialog(m_pSetScaleTable, "Set the scale of an object");

			if(l_i32Response == GTK_RESPONSE_ACCEPT)
			{
				const gchar* l_pScaleObjectName = gtk_entry_get_text(m_pScaleObjectName);
				float32 l_f32ScaleX = 1.f;
				float32 l_f32ScaleY = 1.f;
				float32 l_f32ScaleZ = 1.f;
				const gchar* text = gtk_entry_get_text(m_pXScaleEntry);
				sscanf(text, "%f", &l_f32ScaleX);
				text = gtk_entry_get_text(m_pYScaleEntry);
				sscanf(text, "%f", &l_f32ScaleY);
				text = gtk_entry_get_text(m_pZScaleEntry);
				sscanf(text, "%f", &l_f32ScaleZ);
				m_rSimple3DDatabase.setVisualObjectScale(l_pScaleObjectName, l_f32ScaleX, l_f32ScaleY, l_f32ScaleZ);
			}
		}

		void CSimple3DView::setVisualObjectColorCB()
		{
			gint l_i32Response = showDialog(m_pSetColorTable, "Set the color and transparency of an object");

			if(l_i32Response == GTK_RESPONSE_ACCEPT)
			{
				const gchar* l_pColorObjectName = gtk_entry_get_text(m_pColorObjectName);
				float32 l_pColor[4];
				l_pColor[0] = l_pColor[1] = l_pColor[2] = l_pColor[3] = 1.f;
				const gchar* text = gtk_entry_get_text(m_pRedEntry);
				l_pColor[0] = (float32)atof(text);
				text = gtk_entry_get_text(m_pGreenEntry);
				l_pColor[1] = (float32)atof(text);
				text = gtk_entry_get_text(m_pBlueEntry);
				l_pColor[2] = (float32)atof(text);
				text = gtk_entry_get_text(m_pTransparencyEntry);
				l_pColor[3] = (float32)atof(text);

				//if setting material color
				if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_pSetMaterialColorRadioButton)) == (gboolean)TRUE)
				{
					//set new object color
					m_rSimple3DDatabase.setVisualObjectMaterialColor(
						l_pColorObjectName,
						l_pColor);
				}
				else //setting vertex color
				{
					//set new object color
					m_rSimple3DDatabase.setVisualObjectVertexColor(
						l_pColorObjectName,
						l_pColor);
				}
			}
		}

		void CSimple3DView::repositionCameraCB()
		{
			m_rSimple3DDatabase.repositionCamera();
		}

		gboolean CSimple3DView::redrawCB(GtkWidget *widget, GdkEventExpose *event, gpointer data)
		{
			reinterpret_cast<CSimple3DView*>(data)->redraw();
			return TRUE;
		}

		gboolean CSimple3DView::sizeAllocateCB(GtkWidget *widget, GtkAllocation *allocation, gpointer data)
		{
			reinterpret_cast<CSimple3DView*>(data)->resize((uint32)allocation->width, (uint32)allocation->height);
			return FALSE;
		}

		void CSimple3DView::resize(uint32 ui32Width, uint32 ui32Height)
		{
		}

		void createObjectButtonCallback(GtkButton *button, gpointer data)
		{
			CSimple3DView* l_pSimple3DView = reinterpret_cast<CSimple3DView*>(data);
			l_pSimple3DView->createVisualObjectCB();
		}

		void deleteObjectButtonCallback(GtkButton *button, gpointer data)
		{
			CSimple3DView* l_pSimple3DView = reinterpret_cast<CSimple3DView*>(data);
			l_pSimple3DView->deleteVisualObjectCB();
		}

		void setPositionButtonCallback(GtkButton *button, gpointer data)
		{
			CSimple3DView* l_pSimple3DView = reinterpret_cast<CSimple3DView*>(data);
			l_pSimple3DView->setVisualObjectPositionCB();
		}

		//TODO : orientation

		void setScaleButtonCallback(GtkButton *button, gpointer data)
		{
			CSimple3DView* l_pSimple3DView = reinterpret_cast<CSimple3DView*>(data);
			l_pSimple3DView->setVisualObjectScaleCB();
		}

		void setColorButtonCallback(GtkButton *button, gpointer data)
		{
			CSimple3DView* l_pSimple3DView = reinterpret_cast<CSimple3DView*>(data);
			l_pSimple3DView->setVisualObjectColorCB();
		}

		void repositionCameraButtonCallback(GtkButton *button, gpointer data)
		{
			CSimple3DView* l_pSimple3DView = reinterpret_cast<CSimple3DView*>(data);
			l_pSimple3DView->repositionCameraCB();
		}
	};
};
