#include "ovdCBoxConfigurationDialog.h"
#include "ovdCSettingCollectionHelper.h"

#include <vector>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEDesigner;
using namespace std;

static void on_file_override_check_toggled(::GtkToggleButton* pToggleButton, gpointer pUserData)
{
	gtk_widget_set_sensitive((::GtkWidget*)pUserData, !gtk_toggle_button_get_active(pToggleButton));
}

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

CBoxConfigurationDialog::CBoxConfigurationDialog(IKernel& rKernel, IBox& rBox, const char* sGUIFilename)
	:m_rKernel(rKernel)
	,m_rBox(rBox)
	,m_sGUIFilename(sGUIFilename)
{
}

CBoxConfigurationDialog::~CBoxConfigurationDialog(void)
{
}

void CBoxConfigurationDialog::run(void)
{
	CSettingCollectionHelper l_oHelper(m_rKernel, m_sGUIFilename.toASCIIString());
	if(m_rBox.getSettingCount())
	{
		uint32 i;
		CString l_oSettingName;
		CString l_oSettingValue;
		CIdentifier l_oSettingType;

		::GladeXML* l_pGladeInterfaceSetting=glade_xml_new(m_sGUIFilename.toASCIIString(), "box_configuration", NULL);
		::GtkWidget* l_pSettingDialog=glade_xml_get_widget(l_pGladeInterfaceSetting, "box_configuration");
		::GtkTable* l_pSettingTable=GTK_TABLE(glade_xml_get_widget(l_pGladeInterfaceSetting, "box_configuration-table"));
		::GtkContainer* l_pFileOverrideContainer=GTK_CONTAINER(glade_xml_get_widget(l_pGladeInterfaceSetting, "box_configuration-hbox_filename_override"));
		::GtkCheckButton* l_pFileOverrideCheck=GTK_CHECK_BUTTON(glade_xml_get_widget(l_pGladeInterfaceSetting, "box_configuration-checkbutton_filename_override"));
		g_object_unref(l_pGladeInterfaceSetting);

		gtk_table_resize(l_pSettingTable, m_rBox.getSettingCount(), 3);
		gtk_window_set_title(GTK_WINDOW(l_pSettingDialog), m_rBox.getName());

		vector< ::GtkWidget* > l_vSettingValue;
		for(i=0; i<m_rBox.getSettingCount(); i++)
		{
			m_rBox.getSettingName(i, l_oSettingName);
			m_rBox.getSettingValue(i, l_oSettingValue);
			m_rBox.getSettingType(i, l_oSettingType);

			::GladeXML* l_pGladeInterfaceDummy=glade_xml_new(m_sGUIFilename.toASCIIString(), "settings_collection-dummy_setting_content", NULL);
			::GtkWidget* l_pSettingName=glade_xml_get_widget(l_pGladeInterfaceDummy, "settings_collection-label_setting_name");
			::GtkWidget* l_pSettingRevert=glade_xml_get_widget(l_pGladeInterfaceDummy, "settings_collection-button_setting_revert");
			g_object_unref(l_pGladeInterfaceDummy);

			string l_sWidgetName=l_oHelper.getSettingWidgetName(l_oSettingType).toASCIIString();
			::GladeXML* l_pGladeInterfaceSettingCollection=glade_xml_new(m_sGUIFilename.toASCIIString(), l_sWidgetName.c_str(), NULL);
			::GtkWidget* l_pSettingValue=glade_xml_get_widget(l_pGladeInterfaceSettingCollection, l_sWidgetName.c_str());
			g_object_unref(l_pGladeInterfaceSettingCollection);

			gtk_widget_ref(l_pSettingName);
			gtk_widget_ref(l_pSettingValue);
			gtk_widget_ref(l_pSettingRevert);
			gtk_widget_unparent(l_pSettingName);
			gtk_widget_unparent(l_pSettingValue);
			gtk_widget_unparent(l_pSettingRevert);
			gtk_table_attach(l_pSettingTable, l_pSettingName,   0, 1, i, i+1, ::GtkAttachOptions(GTK_FILL),            ::GtkAttachOptions(GTK_FILL),            0, 0);
			gtk_table_attach(l_pSettingTable, l_pSettingValue,  1, 2, i, i+1, ::GtkAttachOptions(GTK_FILL|GTK_EXPAND), ::GtkAttachOptions(GTK_FILL|GTK_EXPAND), 0, 0);
			gtk_table_attach(l_pSettingTable, l_pSettingRevert, 2, 3, i, i+1, ::GtkAttachOptions(GTK_SHRINK),          ::GtkAttachOptions(GTK_SHRINK),          0, 0);
			gtk_widget_unref(l_pSettingRevert);
			gtk_widget_unref(l_pSettingValue);
			gtk_widget_unref(l_pSettingName);

			l_vSettingValue.push_back(l_pSettingValue);

			l_oHelper.setValue(l_oSettingType, l_pSettingValue, l_oSettingValue);
			gtk_label_set_text(GTK_LABEL(l_pSettingName), l_oSettingName);
		}

#if 1
		g_signal_connect(G_OBJECT(l_pFileOverrideCheck), "toggled", G_CALLBACK(on_file_override_check_toggled), GTK_WIDGET(l_pSettingTable));

		string l_sSettingOverrideWidgetName=l_oHelper.getSettingWidgetName(OV_TypeId_Filename).toASCIIString();
		::GladeXML* l_pGladeInterfaceSettingCollection=glade_xml_new(m_sGUIFilename.toASCIIString(), l_sSettingOverrideWidgetName.c_str(), NULL);
		::GtkWidget* l_pSettingOverrideValue=glade_xml_get_widget(l_pGladeInterfaceSettingCollection, l_sSettingOverrideWidgetName.c_str());
		g_object_unref(l_pGladeInterfaceSettingCollection);

		gtk_widget_ref(l_pSettingOverrideValue);
		gtk_widget_unparent(l_pSettingOverrideValue);
		gtk_container_add(l_pFileOverrideContainer, l_pSettingOverrideValue);
		gtk_widget_unref(l_pSettingOverrideValue);

		if(m_rBox.hasAttribute(OV_AttributeId_Box_SettingOverrideFilename))
		{
			l_oHelper.setValue(OV_TypeId_Filename, l_pSettingOverrideValue, m_rBox.getAttributeValue(OV_AttributeId_Box_SettingOverrideFilename));
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pFileOverrideCheck), true);
			gtk_widget_set_sensitive(GTK_WIDGET(l_pSettingTable), false);
		}
		else
		{
			l_oHelper.setValue(OV_TypeId_Filename, l_pSettingOverrideValue, "");
		}
#endif

		boolean l_bFinished=false;
		while(!l_bFinished)
		{
			gint l_iResult=gtk_dialog_run(GTK_DIALOG(l_pSettingDialog));
			if(l_iResult==0) // revert
			{
				for(i=0; i<m_rBox.getSettingCount(); i++)
				{
					m_rBox.getSettingType(i, l_oSettingType);
					m_rBox.getSettingDefaultValue(i, l_oSettingValue);
					l_oHelper.setValue(l_oSettingType, l_vSettingValue[i], l_oSettingValue);
				}
				if(m_rBox.hasAttribute(OV_AttributeId_Box_SettingOverrideFilename))
				{
					l_oHelper.setValue(OV_TypeId_Filename, l_pSettingOverrideValue, m_rBox.getAttributeValue(OV_AttributeId_Box_SettingOverrideFilename));
					gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pFileOverrideCheck), true);
					gtk_widget_set_sensitive(GTK_WIDGET(l_pSettingTable), false);
				}
			}
			else if(l_iResult==GTK_RESPONSE_APPLY)
			{
				for(i=0; i<m_rBox.getSettingCount(); i++)
				{
					m_rBox.getSettingType(i, l_oSettingType);
					m_rBox.setSettingValue(i, l_oHelper.getValue(l_oSettingType, l_vSettingValue[i]));
				}
				if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pFileOverrideCheck)))
				{
					if(m_rBox.hasAttribute(OV_AttributeId_Box_SettingOverrideFilename))
					{
						m_rBox.setAttributeValue(OV_AttributeId_Box_SettingOverrideFilename, l_oHelper.getValue(OV_TypeId_Filename, l_pSettingOverrideValue));
					}
					else
					{
						m_rBox.addAttribute(OV_AttributeId_Box_SettingOverrideFilename, l_oHelper.getValue(OV_TypeId_Filename, l_pSettingOverrideValue));
					}
				}
				else
				{
					if(m_rBox.hasAttribute(OV_AttributeId_Box_SettingOverrideFilename))
					{
						m_rBox.removeAttribute(OV_AttributeId_Box_SettingOverrideFilename);
					}
				}

				l_bFinished=true;
			}
			else
			{
				l_bFinished=true;
			}
		}

		gtk_widget_destroy(l_pSettingDialog);
	}
}
