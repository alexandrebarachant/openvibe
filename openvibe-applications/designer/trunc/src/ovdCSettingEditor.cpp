#include "ovdCSettingEditor.h"

#include <vector>
#include <sstream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEDesigner;
using namespace std;

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

static void getValueBitMaskCB(::GtkWidget* pWidget, gpointer pUserData)
{
	static_cast< vector< ::GtkWidget* > *>(pUserData)->push_back(pWidget);
}

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

CSettingEditor::CSettingEditor(IKernel& rKernel, IBox& rBox, const char* sGUIFilename)
	:m_rKernel(rKernel)
	,m_rBox(rBox)
	,m_sGUIFilename(sGUIFilename)
{
}

CSettingEditor::~CSettingEditor(void)
{
}

void CSettingEditor::run(void)
{
	if(m_rBox.getSettingCount())
	{
		uint32 i;
		CString l_oSettingName;
		CString l_oSettingValue;
		CIdentifier l_oSettingType;

		::GladeXML* l_pGladeInterfaceSetting=glade_xml_new(m_sGUIFilename.c_str(), "setting", NULL);
		::GtkWidget* l_pSettingDialog=glade_xml_get_widget(l_pGladeInterfaceSetting, "setting");
		::GtkTable* l_pSettingTable=GTK_TABLE(glade_xml_get_widget(l_pGladeInterfaceSetting, "table_setting"));
		g_object_unref(l_pGladeInterfaceSetting);

		gtk_table_resize(l_pSettingTable, m_rBox.getSettingCount(), 3);
		gtk_window_set_title(GTK_WINDOW(l_pSettingDialog), m_rBox.getName());

		vector< ::GtkWidget* > l_vSettingValue;
		for(i=0; i<m_rBox.getSettingCount(); i++)
		{
			m_rBox.getSettingName(i, l_oSettingName);
			m_rBox.getSettingValue(i, l_oSettingValue);
			m_rBox.getSettingType(i, l_oSettingType);

			::GladeXML* l_pGladeInterfaceDummy=glade_xml_new(m_sGUIFilename.c_str(), "dummy_setting_content", NULL);
			::GtkWidget* l_pSettingName=glade_xml_get_widget(l_pGladeInterfaceDummy, "label_setting_name");
			::GtkWidget* l_pSettingRevert=glade_xml_get_widget(l_pGladeInterfaceDummy, "button_setting_revert");
			g_object_unref(l_pGladeInterfaceDummy);

			string l_sWidgetName=getSettingWidgetName(l_oSettingType);
			::GladeXML* l_pGladeInterfaceSettingCollection=glade_xml_new(m_sGUIFilename.c_str(), l_sWidgetName.c_str(), NULL);
			::GtkWidget* l_pSettingValue=glade_xml_get_widget(l_pGladeInterfaceSettingCollection, l_sWidgetName.c_str());
			g_object_unref(l_pGladeInterfaceSettingCollection);

			gtk_widget_ref(l_pSettingName);
			gtk_widget_ref(l_pSettingValue);
			gtk_widget_ref(l_pSettingRevert);
			gtk_widget_unparent(l_pSettingName);
			gtk_widget_unparent(l_pSettingValue);
			gtk_widget_unparent(l_pSettingRevert);
			gtk_table_attach_defaults(l_pSettingTable, l_pSettingName, 0, 1, i, i+1);
			gtk_table_attach_defaults(l_pSettingTable, l_pSettingValue, 1, 2, i, i+1);
			gtk_table_attach_defaults(l_pSettingTable, l_pSettingRevert, 2, 3, i, i+1);
			gtk_widget_unref(l_pSettingRevert);
			gtk_widget_unref(l_pSettingValue);
			gtk_widget_unref(l_pSettingName);

			setValue(l_oSettingType, l_pSettingValue, l_oSettingValue);
			gtk_label_set_text(GTK_LABEL(l_pSettingName), l_oSettingName);

			l_vSettingValue.push_back(l_pSettingValue);
		}

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
					setValue(l_oSettingType, l_vSettingValue[i], l_oSettingValue);
				}
			}
			else if(l_iResult==GTK_RESPONSE_APPLY)
			{
				for(i=0; i<m_rBox.getSettingCount(); i++)
				{
					m_rBox.getSettingType(i, l_oSettingType);
					m_rBox.setSettingValue(i, getValue(l_oSettingType, l_vSettingValue[i]));
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

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

string CSettingEditor::getSettingWidgetName(const CIdentifier& rTypeIdentifier)
{
	if(rTypeIdentifier==OV_TypeId_Boolean) return "check_button_setting_boolean";
	if(rTypeIdentifier==OV_TypeId_Integer) return "spin_button_setting_integer";
	if(rTypeIdentifier==OV_TypeId_Float)   return "spin_button_setting_float";
	if(rTypeIdentifier==OV_TypeId_String)  return "entry_setting_string";
	if(m_rKernel.getContext()->getTypeManager().isEnumeration(rTypeIdentifier)) return "combobox_setting_enumeration";
	if(m_rKernel.getContext()->getTypeManager().isBitMask(rTypeIdentifier))     return "table_setting_bitmask";
	return "entry_setting_string";
}

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

CString CSettingEditor::getValue(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget)
{
	if(rTypeIdentifier==OV_TypeId_Boolean) return getValueBoolean(pWidget);
	if(rTypeIdentifier==OV_TypeId_Integer) return getValueInteger(pWidget);
	if(rTypeIdentifier==OV_TypeId_Float)   return getValueFloat(pWidget);
	if(rTypeIdentifier==OV_TypeId_String)  return getValueString(pWidget);
	if(m_rKernel.getContext()->getTypeManager().isEnumeration(rTypeIdentifier)) return getValueEnumeration(rTypeIdentifier, pWidget);
	if(m_rKernel.getContext()->getTypeManager().isBitMask(rTypeIdentifier))     return getValueBitMask(rTypeIdentifier, pWidget);
	return getValueString(pWidget);
}

CString CSettingEditor::getValueBoolean(::GtkWidget* pWidget)
{
	::GtkToggleButton* l_pWidget=GTK_TOGGLE_BUTTON(pWidget);
	boolean l_bActive=gtk_toggle_button_get_active(l_pWidget)?true:false;
	return CString(l_bActive?"true":"false");
}

CString CSettingEditor::getValueInteger(::GtkWidget* pWidget)
{
	::GtkSpinButton* l_pWidget=GTK_SPIN_BUTTON(pWidget);
	int l_iValue=gtk_spin_button_get_value_as_int(l_pWidget);
	char l_sValue[1024];
	sprintf(l_sValue, "%i", l_iValue);
	return CString(l_sValue);
}

CString CSettingEditor::getValueFloat(::GtkWidget* pWidget)
{
	::GtkSpinButton* l_pWidget=GTK_SPIN_BUTTON(pWidget);
	double l_fValue=gtk_spin_button_get_value_as_float(l_pWidget);
	char l_sValue[1024];
	sprintf(l_sValue, "%f", l_fValue);
	return CString(l_sValue);
}

CString CSettingEditor::getValueString(::GtkWidget* pWidget)
{
	::GtkEntry* l_pWidget=GTK_ENTRY(pWidget);
	return CString(gtk_entry_get_text(l_pWidget));
}

CString CSettingEditor::getValueEnumeration(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget)
{
	::GtkComboBox* l_pWidget=GTK_COMBO_BOX(pWidget);
	return CString(gtk_combo_box_get_active_text(l_pWidget));
}

// ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------

CString CSettingEditor::getValueBitMask(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget)
{
	vector<string> l_vBitMaskValue;
	string l_sBitMaskString(m_rKernel.getContext()->getTypeManager().getBitMaskString(rTypeIdentifier));
	string l_sBitMaskValue;
	istringstream l_oISS(l_sBitMaskString.c_str());
	while(!l_oISS.eof())
	{
		getline(l_oISS, l_sBitMaskValue, OV_Value_EnumeratedStringSeparator);
		if(l_sBitMaskValue.length()!=0)
		{
			l_vBitMaskValue.push_back(l_sBitMaskValue);
		};
	}

	::GtkTable* l_pBitMaskTable=GTK_TABLE(pWidget);
	string l_sResult;
	vector< ::GtkWidget* > l_vWidget;
	gtk_container_foreach(GTK_CONTAINER(l_pBitMaskTable), getValueBitMaskCB, &l_vWidget);
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

void CSettingEditor::setValue(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget, const CString& rValue)
{
	if(rTypeIdentifier==OV_TypeId_Boolean) return setValueBoolean(pWidget, rValue);
	if(rTypeIdentifier==OV_TypeId_Integer) return setValueInteger(pWidget, rValue);
	if(rTypeIdentifier==OV_TypeId_Float)   return setValueFloat(pWidget, rValue);
	if(rTypeIdentifier==OV_TypeId_String)  return setValueString(pWidget, rValue);
	if(m_rKernel.getContext()->getTypeManager().isEnumeration(rTypeIdentifier)) return setValueEnumeration(rTypeIdentifier, pWidget, rValue);
	if(m_rKernel.getContext()->getTypeManager().isBitMask(rTypeIdentifier))     return setValueBitMask(rTypeIdentifier, pWidget, rValue);
	return setValueString(pWidget, rValue);
}

void CSettingEditor::setValueBoolean(::GtkWidget* pWidget, const CString& rValue)
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

void CSettingEditor::setValueInteger(::GtkWidget* pWidget, const CString& rValue)
{
	::GtkSpinButton* l_pWidget=GTK_SPIN_BUTTON(pWidget);
	int l_iValue=0;
	sscanf(rValue, "%i", &l_iValue);
	gtk_spin_button_set_range(l_pWidget, -G_MAXDOUBLE, G_MAXDOUBLE);
	gtk_spin_button_set_value(l_pWidget, l_iValue);
}

void CSettingEditor::setValueFloat(::GtkWidget* pWidget, const CString& rValue)
{
	::GtkSpinButton* l_pWidget=GTK_SPIN_BUTTON(pWidget);
	double l_fValue=0;
	sscanf(rValue, "%lf", &l_fValue);
	gtk_spin_button_set_range(l_pWidget, -G_MAXDOUBLE, G_MAXDOUBLE);
	gtk_spin_button_set_value(l_pWidget, l_fValue);
}

void CSettingEditor::setValueString(::GtkWidget* pWidget, const CString& rValue)
{
	::GtkEntry* l_pWidget=GTK_ENTRY(pWidget);
	gtk_entry_set_text(l_pWidget, rValue);
}

void CSettingEditor::setValueEnumeration(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget, const CString& rValue)
{
	vector<string> l_vEnumerationValue;
	string l_sEnumerationString(m_rKernel.getContext()->getTypeManager().getEnumerationString(rTypeIdentifier));
	string l_sEnumerationValue;
	string l_sSettingValue(rValue);
	istringstream l_oISS(l_sEnumerationString.c_str());
	while(!l_oISS.eof())
	{
		getline(l_oISS, l_sEnumerationValue, OV_Value_EnumeratedStringSeparator);
		if(l_sEnumerationValue.length()!=0)
		{
			l_vEnumerationValue.push_back(l_sEnumerationValue);
		};
	}

	::GtkTreeIter l_oListIter;
	::GtkComboBox* l_pWidget=GTK_COMBO_BOX(pWidget);
	::GtkListStore* l_pList=GTK_LIST_STORE(gtk_combo_box_get_model(l_pWidget));

	uint32 l_ui32FoundIndex=0;
	boolean l_bFoundValue=false;
	gtk_list_store_clear(l_pList);
	for(unsigned int i=0; i<l_vEnumerationValue.size(); i++)
	{
		gtk_list_store_append(l_pList, &l_oListIter);
		gtk_list_store_set(l_pList, &l_oListIter, 0, l_vEnumerationValue[i].c_str(), -1);
		if(!l_bFoundValue)
		{
			if(l_vEnumerationValue[i]==l_sSettingValue)
			{
				l_bFoundValue=true;
			}
			else
			{
				l_ui32FoundIndex++;
			}
		}
	}

	if(l_bFoundValue)
	{
		gtk_combo_box_set_active(l_pWidget, l_ui32FoundIndex);
	}
}

void CSettingEditor::setValueBitMask(const CIdentifier& rTypeIdentifier, ::GtkWidget* pWidget, const CString& rValue)
{
	vector<string> l_vBitMaskValue;
	string l_sBitMaskString(m_rKernel.getContext()->getTypeManager().getBitMaskString(rTypeIdentifier));
	string l_sBitMaskValue;
	string l_sSettingValue(rValue);
	istringstream l_oISS(l_sBitMaskString.c_str());
	while(!l_oISS.eof())
	{
		getline(l_oISS, l_sBitMaskValue, OV_Value_EnumeratedStringSeparator);
		if(l_sBitMaskValue.length()!=0)
		{
			l_vBitMaskValue.push_back(l_sBitMaskValue);
		};
	}

	::GtkTable* l_pBitMaskTable=GTK_TABLE(pWidget);
	gtk_table_resize(l_pBitMaskTable, 2, (l_vBitMaskValue.size()+1)>>1);
	for(unsigned int i=0; i<l_vBitMaskValue.size(); i++)
	{
		::GladeXML* l_pGladeInterfaceDummy=glade_xml_new(m_sGUIFilename.c_str(), "check_button_setting_boolean", NULL);
		::GtkWidget* l_pSettingButton=glade_xml_get_widget(l_pGladeInterfaceDummy, "check_button_setting_boolean");
		g_object_unref(l_pGladeInterfaceDummy);

		gtk_widget_ref(l_pSettingButton);
		gtk_widget_unparent(l_pSettingButton);
		gtk_table_attach_defaults(l_pBitMaskTable, l_pSettingButton, (i&1), (i&1)+1, (i>>1), (i>>1)+1);
		gtk_widget_unref(l_pSettingButton);

		gtk_button_set_label(GTK_BUTTON(l_pSettingButton), l_vBitMaskValue[i].c_str());
		if(l_sSettingValue.find(l_vBitMaskValue[i])!=string::npos)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pSettingButton), true);
		}
	}
}
