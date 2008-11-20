#include "ovdCSettingEditorDialog.h"
#include "ovdCSettingCollectionHelper.h"

#include <map>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEDesigner;
using namespace std;

static void type_changed_cb(::GtkComboBox* pWidget, gpointer pUserData)
{
	static_cast<CSettingEditorDialog*>(pUserData)->typeChangedCB();
}

CSettingEditorDialog::CSettingEditorDialog(const IKernelContext& rKernelContext, IBox& rBox, uint32 ui32SettingIndex, const char* sTitle, const char* sGUIFilename)
	:m_rKernelContext(rKernelContext)
	,m_rBox(rBox)
	,m_ui32SettingIndex(ui32SettingIndex)
	,m_sGUIFilename(sGUIFilename)
	,m_sTitle(sTitle)
	,m_pDefaultValue(NULL)
{
}

CSettingEditorDialog::~CSettingEditorDialog(void)
{
}

boolean CSettingEditorDialog::run(void)
{
	::GladeXML* l_pGladeInterfaceSetting=glade_xml_new(m_sGUIFilename.toASCIIString(), "setting_editor", NULL);
	::GtkWidget* l_pDialog=glade_xml_get_widget(l_pGladeInterfaceSetting, "setting_editor");
	::GtkWidget* l_pName=glade_xml_get_widget(l_pGladeInterfaceSetting, "setting_editor-setting_name_entry");
	m_pTable=glade_xml_get_widget(l_pGladeInterfaceSetting, "setting_editor-table");
	m_pType=glade_xml_get_widget(l_pGladeInterfaceSetting, "setting_editor-setting_type_combobox");
	gtk_list_store_clear(GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(m_pType))));
	g_object_unref(l_pGladeInterfaceSetting);

	gtk_window_set_title(GTK_WINDOW(l_pDialog), m_sTitle.c_str());

	g_signal_connect(G_OBJECT(m_pType), "changed", G_CALLBACK(type_changed_cb), this);

	CString l_sSettingName;
	CIdentifier l_oSettingType;
	m_rBox.getSettingName(m_ui32SettingIndex, l_sSettingName);
	m_rBox.getSettingType(m_ui32SettingIndex, l_oSettingType);

	gtk_entry_set_text(GTK_ENTRY(l_pName), l_sSettingName.toASCIIString());

	CIdentifier l_oCurrentTypeIdentifier;
	gint l_iActive=-1;
	while((l_oCurrentTypeIdentifier=m_rKernelContext.getTypeManager().getNextTypeIdentifier(l_oCurrentTypeIdentifier))!=OV_UndefinedIdentifier)
	{
		if(!m_rKernelContext.getTypeManager().isStream(l_oCurrentTypeIdentifier))
		{
			gtk_combo_box_append_text(GTK_COMBO_BOX(m_pType), m_rKernelContext.getTypeManager().getTypeName(l_oCurrentTypeIdentifier).toASCIIString());
			if(l_oCurrentTypeIdentifier==l_oSettingType)
			{
				l_iActive=m_vSettingTypes.size();
			}
			m_vSettingTypes[m_rKernelContext.getTypeManager().getTypeName(l_oCurrentTypeIdentifier).toASCIIString()]=l_oCurrentTypeIdentifier;
		}
	}

	if(l_iActive!=-1)
	{
		gtk_combo_box_set_active(GTK_COMBO_BOX(m_pType), l_iActive);
	}

	boolean l_bFinished=false;
	boolean l_bResult=false;
	while(!l_bFinished)
	{
		gint l_iResult=gtk_dialog_run(GTK_DIALOG(l_pDialog));
		if(l_iResult==GTK_RESPONSE_APPLY)
		{
			char* l_sActiveText=gtk_combo_box_get_active_text(GTK_COMBO_BOX(m_pType));
			if(l_sActiveText)
			{
				CIdentifier l_oSettingType=m_vSettingTypes[l_sActiveText];
				CSettingCollectionHelper l_oHelper(m_rKernelContext, m_sGUIFilename.toASCIIString());
				m_rBox.setSettingName(m_ui32SettingIndex, gtk_entry_get_text(GTK_ENTRY(l_pName)));
				m_rBox.setSettingType(m_ui32SettingIndex, l_oSettingType);
				m_rBox.setSettingValue(m_ui32SettingIndex, l_oHelper.getValue(l_oSettingType, m_pDefaultValue));
				m_rBox.setSettingDefaultValue(m_ui32SettingIndex, l_oHelper.getValue(l_oSettingType, m_pDefaultValue));
				l_bFinished=true;
				l_bResult=true;
			}
		}
		else if(l_iResult==2) // revert
		{
			gtk_entry_set_text(GTK_ENTRY(l_pName), l_sSettingName.toASCIIString());

			if(l_iActive!=-1)
			{
				gtk_combo_box_set_active(GTK_COMBO_BOX(m_pType), l_iActive);
			}
		}
		else
		{
			l_bFinished=true;
			l_bResult=false;
		}
	}

	gtk_widget_destroy(l_pDialog);

	return l_bResult;
}

void CSettingEditorDialog::typeChangedCB(void)
{
	CSettingCollectionHelper l_oHelper(m_rKernelContext, m_sGUIFilename.toASCIIString());

	CIdentifier l_oSettingType=m_vSettingTypes[gtk_combo_box_get_active_text(GTK_COMBO_BOX(m_pType))];

	CString l_sWidgetName=l_oHelper.getSettingWidgetName(l_oSettingType).toASCIIString();
	::GladeXML* l_pGladeInterfaceDefaultValueDummy=glade_xml_new(m_sGUIFilename.toASCIIString(), l_sWidgetName.toASCIIString(), NULL);
	if(m_pDefaultValue) gtk_container_remove(GTK_CONTAINER(m_pTable), m_pDefaultValue);
	m_pDefaultValue=glade_xml_get_widget(l_pGladeInterfaceDefaultValueDummy, l_sWidgetName.toASCIIString());
	gtk_table_attach(GTK_TABLE(m_pTable), m_pDefaultValue, 1, 2, 2, 3, ::GtkAttachOptions(GTK_FILL|GTK_EXPAND), ::GtkAttachOptions(GTK_FILL|GTK_EXPAND), 0, 0);
	g_object_unref(l_pGladeInterfaceDefaultValueDummy);

	CString l_sDefaultValue;
	m_rBox.getSettingDefaultValue(m_ui32SettingIndex, l_sDefaultValue);
	l_oHelper.setValue(l_oSettingType, m_pDefaultValue, l_sDefaultValue);
}
