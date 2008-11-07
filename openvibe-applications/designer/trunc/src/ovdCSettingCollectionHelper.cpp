#include "ovdCSettingCollectionHelper.h"

#include <vector>
#include <string>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEDesigner;
using namespace std;

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

static void collect_widget_cb(::GtkWidget* pWidget, gpointer pUserData)
{
	static_cast< vector< ::GtkWidget* > *>(pUserData)->push_back(pWidget);
}

static void on_button_setting_filename_browse_pressed(::GtkButton* pButton, gpointer pUserData)
{
	vector< ::GtkWidget* > l_vWidget;
	gtk_container_foreach(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(pButton))), collect_widget_cb, &l_vWidget);
	::GtkEntry* l_pWidget=GTK_ENTRY(l_vWidget[0]);

	::GtkWidget* l_pWidgetDialogOpen=gtk_file_chooser_dialog_new(
		"Select file to open...",
		NULL,
		GTK_FILE_CHOOSER_ACTION_SAVE,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);

	const char* l_sInitialFileName=gtk_entry_get_text(l_pWidget);
	if(g_path_is_absolute(l_sInitialFileName))
	{
		gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(l_pWidgetDialogOpen), l_sInitialFileName);
	}
	else
	{
		char* l_sFullPath=g_build_filename(g_get_current_dir(), l_sInitialFileName, NULL);
		gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(l_pWidgetDialogOpen), l_sFullPath);
		g_free(l_sFullPath);
	}

	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(l_pWidgetDialogOpen), false);

	if(gtk_dialog_run(GTK_DIALOG(l_pWidgetDialogOpen))==GTK_RESPONSE_ACCEPT)
	{
		char* l_sFileName=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(l_pWidgetDialogOpen));
		gtk_entry_set_text(l_pWidget, l_sFileName);
		g_free(l_sFileName);
	}
	gtk_widget_destroy(l_pWidgetDialogOpen);
}

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

CSettingCollectionHelper::CSettingCollectionHelper(const IKernelContext& rKernelContext, const char* sGUIFilename)
	:m_rKernelContext(rKernelContext)
	,m_sGUIFilename(sGUIFilename)
{
}

CSettingCollectionHelper::~CSettingCollectionHelper(void)
{
}

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

CString CSettingCollectionHelper::getSettingWidgetName(const CIdentifier& rTypeIdentifier)
{
	if(rTypeIdentifier==OV_TypeId_Boolean)  return "settings_collection-check_button_setting_boolean";
	if(rTypeIdentifier==OV_TypeId_Integer)  return "settings_collection-spin_button_setting_integer";
	if(rTypeIdentifier==OV_TypeId_Float)    return "settings_collection-spin_button_setting_float";
	if(rTypeIdentifier==OV_TypeId_String)   return "settings_collection-entry_setting_string";
	if(rTypeIdentifier==OV_TypeId_Filename) return "settings_collection-hbox_setting_filename";
	if(m_rKernelContext.getTypeManager().isEnumeration(rTypeIdentifier)) return "settings_collection-combobox_setting_enumeration";
	if(m_rKernelContext.getTypeManager().isBitMask(rTypeIdentifier))     return "settings_collection-table_setting_bitmask";
	return "settings_collection-entry_setting_string";
}

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

CString CSettingCollectionHelper::getValue(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget)
{
	if(rTypeIdentifier==OV_TypeId_Boolean)  return getValueBoolean(pWidget);
	if(rTypeIdentifier==OV_TypeId_Integer)  return getValueInteger(pWidget);
	if(rTypeIdentifier==OV_TypeId_Float)    return getValueFloat(pWidget);
	if(rTypeIdentifier==OV_TypeId_String)   return getValueString(pWidget);
	if(rTypeIdentifier==OV_TypeId_Filename) return getValueFilename(pWidget);
	if(m_rKernelContext.getTypeManager().isEnumeration(rTypeIdentifier)) return getValueEnumeration(rTypeIdentifier, pWidget);
	if(m_rKernelContext.getTypeManager().isBitMask(rTypeIdentifier))     return getValueBitMask(rTypeIdentifier, pWidget);
	return getValueString(pWidget);
}

CString CSettingCollectionHelper::getValueBoolean(::GtkWidget* pWidget)
{
	::GtkToggleButton* l_pWidget=GTK_TOGGLE_BUTTON(pWidget);
	boolean l_bActive=gtk_toggle_button_get_active(l_pWidget)?true:false;
	return CString(l_bActive?"true":"false");
}

CString CSettingCollectionHelper::getValueInteger(::GtkWidget* pWidget)
{
	::GtkSpinButton* l_pWidget=GTK_SPIN_BUTTON(pWidget);
	int l_iValue=gtk_spin_button_get_value_as_int(l_pWidget);
	char l_sValue[1024];
	sprintf(l_sValue, "%i", l_iValue);
	return CString(l_sValue);
}

CString CSettingCollectionHelper::getValueFloat(::GtkWidget* pWidget)
{
	::GtkSpinButton* l_pWidget=GTK_SPIN_BUTTON(pWidget);
	double l_fValue=gtk_spin_button_get_value_as_float(l_pWidget);
	char l_sValue[1024];
	sprintf(l_sValue, "%f", l_fValue);
	return CString(l_sValue);
}

CString CSettingCollectionHelper::getValueString(::GtkWidget* pWidget)
{
	::GtkEntry* l_pWidget=GTK_ENTRY(pWidget);
	return CString(gtk_entry_get_text(l_pWidget));
}

CString CSettingCollectionHelper::getValueFilename(::GtkWidget* pWidget)
{
	vector< ::GtkWidget* > l_vWidget;
	gtk_container_foreach(GTK_CONTAINER(pWidget), collect_widget_cb, &l_vWidget);
	::GtkEntry* l_pWidget=GTK_ENTRY(l_vWidget[0]);
	return CString(gtk_entry_get_text(l_pWidget));
}

CString CSettingCollectionHelper::getValueEnumeration(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget)
{
	::GtkComboBox* l_pWidget=GTK_COMBO_BOX(pWidget);
	return CString(gtk_combo_box_get_active_text(l_pWidget));
}

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

CString CSettingCollectionHelper::getValueBitMask(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget)
{
	vector< ::GtkWidget* > l_vWidget;
	gtk_container_foreach(GTK_CONTAINER(pWidget), collect_widget_cb, &l_vWidget);
	string l_sResult;
	for(unsigned int i=0; i<l_vWidget.size(); i++)
	{
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_vWidget[i])))
		{
			if(l_sResult!="")
			{
				l_sResult+=string(1, OV_Value_EnumeratedStringSeparator);
			}
			l_sResult+=gtk_button_get_label(GTK_BUTTON(l_vWidget[i]));
		}
	}

	return CString(l_sResult.c_str());
}

void CSettingCollectionHelper::setValue(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget, const CString& rValue)
{
	if(rTypeIdentifier==OV_TypeId_Boolean)  return setValueBoolean(pWidget, rValue);
	if(rTypeIdentifier==OV_TypeId_Integer)  return setValueInteger(pWidget, rValue);
	if(rTypeIdentifier==OV_TypeId_Float)    return setValueFloat(pWidget, rValue);
	if(rTypeIdentifier==OV_TypeId_String)   return setValueString(pWidget, rValue);
	if(rTypeIdentifier==OV_TypeId_Filename) return setValueFilename(pWidget, rValue);
	if(m_rKernelContext.getTypeManager().isEnumeration(rTypeIdentifier)) return setValueEnumeration(rTypeIdentifier, pWidget, rValue);
	if(m_rKernelContext.getTypeManager().isBitMask(rTypeIdentifier))     return setValueBitMask(rTypeIdentifier, pWidget, rValue);
	return setValueString(pWidget, rValue);
}

void CSettingCollectionHelper::setValueBoolean(::GtkWidget* pWidget, const CString& rValue)
{
	::GtkToggleButton* l_pWidget=GTK_TOGGLE_BUTTON(pWidget);
	if(rValue==CString("true"))
	{
		gtk_toggle_button_set_active(l_pWidget, true);
	}
	else if(rValue==CString("false"))
	{
		gtk_toggle_button_set_active(l_pWidget, false);
	}
	else
	{
		gtk_toggle_button_set_active(l_pWidget, false);
	}
}

void CSettingCollectionHelper::setValueInteger(::GtkWidget* pWidget, const CString& rValue)
{
	::GtkSpinButton* l_pWidget=GTK_SPIN_BUTTON(pWidget);
	int l_iValue=0;
	sscanf(rValue, "%i", &l_iValue);
	gtk_spin_button_set_range(l_pWidget, -G_MAXDOUBLE, G_MAXDOUBLE);
	gtk_spin_button_set_value(l_pWidget, l_iValue);
}

void CSettingCollectionHelper::setValueFloat(::GtkWidget* pWidget, const CString& rValue)
{
	::GtkSpinButton* l_pWidget=GTK_SPIN_BUTTON(pWidget);
	double l_fValue=0;
	sscanf(rValue, "%lf", &l_fValue);
	gtk_spin_button_set_range(l_pWidget, -G_MAXDOUBLE, G_MAXDOUBLE);
	gtk_spin_button_set_value(l_pWidget, l_fValue);
}

void CSettingCollectionHelper::setValueString(::GtkWidget* pWidget, const CString& rValue)
{
	::GtkEntry* l_pWidget=GTK_ENTRY(pWidget);
	gtk_entry_set_text(l_pWidget, rValue);
}

void CSettingCollectionHelper::setValueFilename(::GtkWidget* pWidget, const CString& rValue)
{
	vector< ::GtkWidget* > l_vWidget;
	gtk_container_foreach(GTK_CONTAINER(pWidget), collect_widget_cb, &l_vWidget);
	::GtkEntry* l_pWidget=GTK_ENTRY(l_vWidget[0]);

	g_signal_connect(G_OBJECT(l_vWidget[1]), "clicked", G_CALLBACK(on_button_setting_filename_browse_pressed), NULL);

	gtk_entry_set_text(l_pWidget, rValue);
}

void CSettingCollectionHelper::setValueEnumeration(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget, const CString& rValue)
{
	::GtkTreeIter l_oListIter;
	::GtkComboBox* l_pWidget=GTK_COMBO_BOX(pWidget);
	::GtkListStore* l_pList=GTK_LIST_STORE(gtk_combo_box_get_model(l_pWidget));
	uint64 l_ui64Value=m_rKernelContext.getTypeManager().getEnumerationEntryValueFromName(rTypeIdentifier, rValue);

	gtk_list_store_clear(l_pList);
	for(uint64 i=0; i<m_rKernelContext.getTypeManager().getEnumerationEntryCount(rTypeIdentifier); i++)
	{
		CString l_sEntryName;
		uint64 l_ui64EntryValue;
		if(m_rKernelContext.getTypeManager().getEnumerationEntry(rTypeIdentifier, i, l_sEntryName, l_ui64EntryValue))
		{
			gtk_list_store_append(l_pList, &l_oListIter);
			gtk_list_store_set(l_pList, &l_oListIter, 0, (const char*)l_sEntryName, -1);

			if(l_ui64Value==l_ui64EntryValue)
			{
				gtk_combo_box_set_active(l_pWidget, i);
			}
		}
	}
	if(gtk_combo_box_get_active(l_pWidget)==-1)
	{
		gtk_combo_box_set_active(l_pWidget, 0);
	}
}

void CSettingCollectionHelper::setValueBitMask(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget, const CString& rValue)
{
	string l_sValue(rValue);
	::GtkTable* l_pBitMaskTable=GTK_TABLE(pWidget);
	gtk_table_resize(l_pBitMaskTable, 2, (m_rKernelContext.getTypeManager().getBitMaskEntryCount(rTypeIdentifier)+1)>>1);

	for(uint64 i=0; i<m_rKernelContext.getTypeManager().getBitMaskEntryCount(rTypeIdentifier); i++)
	{
		CString l_sEntryName;
		uint64 l_ui64EntryValue;
		if(m_rKernelContext.getTypeManager().getBitMaskEntry(rTypeIdentifier, i, l_sEntryName, l_ui64EntryValue))
		{
			::GladeXML* l_pGladeInterfaceDummy=glade_xml_new(m_sGUIFilename.toASCIIString(), "settings_collection-check_button_setting_boolean", NULL);
			::GtkWidget* l_pSettingButton=glade_xml_get_widget(l_pGladeInterfaceDummy, "settings_collection-check_button_setting_boolean");
			g_object_unref(l_pGladeInterfaceDummy);

			gtk_widget_ref(l_pSettingButton);
			gtk_widget_unparent(l_pSettingButton);
			gtk_table_attach_defaults(l_pBitMaskTable, l_pSettingButton, (i&1), (i&1)+1, (i>>1), (i>>1)+1);
			gtk_widget_unref(l_pSettingButton);

			gtk_button_set_label(GTK_BUTTON(l_pSettingButton), (const char*)l_sEntryName);

			if(l_sValue.find((const char*)l_sEntryName)!=string::npos)
			{
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pSettingButton), true);
			}
		}
	}
}
