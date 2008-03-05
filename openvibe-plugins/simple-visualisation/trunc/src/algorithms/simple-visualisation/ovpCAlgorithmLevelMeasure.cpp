#include "ovpCAlgorithmLevelMeasure.h"

#include <string>
#include <sstream>
#include <iomanip>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

namespace
{
	void reset_scores_button_cb(::GtkToolButton* pButton, gpointer pUserData)
	{
		CAlgorithmLevelMeasure* l_pLevelMeasure=reinterpret_cast<CAlgorithmLevelMeasure*>(pUserData);
		std::vector < CAlgorithmLevelMeasure::SProgressBar >::iterator it;
		for(it=l_pLevelMeasure->m_vProgressBar.begin(); it!=l_pLevelMeasure->m_vProgressBar.end(); it++)
		{
			it->m_ui32Score=0;
		}
	}

	void threshold_spinbutton_cb(::GtkSpinButton* pButton, gpointer pUserData)
	{
		CAlgorithmLevelMeasure* l_pLevelMeasure=reinterpret_cast<CAlgorithmLevelMeasure*>(pUserData);
		l_pLevelMeasure->m_f64Threshold=.01*gtk_spin_button_get_value(pButton);
	}

	void show_percentages_toggle_button_cb(::GtkToggleToolButton* pButton, gpointer pUserData)
	{
		CAlgorithmLevelMeasure* l_pLevelMeasure=reinterpret_cast<CAlgorithmLevelMeasure*>(pUserData);
		l_pLevelMeasure->m_bShowPercentages=gtk_toggle_tool_button_get_active(pButton);
	}
};

boolean CAlgorithmLevelMeasure::initialize(void)
{
	ip_pMatrix.initialize(getInputParameter(OVP_Algorithm_LevelMeasure_InputParameterId_Matrix));

	op_pMainWidget.initialize(getOutputParameter(OVP_Algorithm_LevelMeasure_OutputParameterId_MainWidget));
	op_pToolbarWidget.initialize(getOutputParameter(OVP_Algorithm_LevelMeasure_OutputParameterId_ToolbarWidget));

	m_pMainWidgetInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-LevelMeasure.glade", "level-measure-table", NULL);
	m_pToolbarWidgetInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-LevelMeasure.glade", "level-measure-toolbar", NULL);

	glade_xml_signal_autoconnect(m_pMainWidgetInterface);
	glade_xml_signal_autoconnect(m_pToolbarWidgetInterface);

	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pToolbarWidgetInterface, "reset-score-button")),             "clicked",       G_CALLBACK(::reset_scores_button_cb),            this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pToolbarWidgetInterface, "show-percentages-toggle-button")), "toggled",       G_CALLBACK(::show_percentages_toggle_button_cb), this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pToolbarWidgetInterface, "threshold-spinbutton")),           "value-changed", G_CALLBACK(::threshold_spinbutton_cb),           this);
	g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pToolbarWidgetInterface, "level-measure-toolbar")),          "delete_event",  G_CALLBACK(gtk_widget_hide),                     NULL);

	m_pMainWindow=glade_xml_get_widget(m_pMainWidgetInterface, "level-measure-table");
	m_pToolbarWidget=glade_xml_get_widget(m_pToolbarWidgetInterface, "level-measure-toolbar");

	m_bShowPercentages=gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pToolbarWidgetInterface, "show-percentages-toggle-button")));
	m_f64Threshold=.01*gtk_spin_button_get_value(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pToolbarWidgetInterface, "threshold-spinbutton")));

	return true;
}

boolean CAlgorithmLevelMeasure::uninitialize(void)
{
	g_object_unref(m_pToolbarWidgetInterface);
	m_pToolbarWidgetInterface=NULL;

	g_object_unref(m_pMainWidgetInterface);
	m_pMainWidgetInterface=NULL;

	op_pToolbarWidget.uninitialize();
	op_pMainWidget.uninitialize();

	ip_pMatrix.uninitialize();

	return true;
}

boolean CAlgorithmLevelMeasure::process(void)
{
	if(this->isInputTriggerActive(OVP_Algorithm_LevelMeasure_InputTriggerId_Reset))
	{
		if(ip_pMatrix->getDimensionCount()!=1 && ip_pMatrix->getDimensionCount()!=2)
		{
			getLogManager() << LogLevel_ImportantWarning << "Input matrix does not have 1 or 2 dimensions (" << ip_pMatrix->getDimensionCount() << ")\n";
			return false;
		}

		uint32 l_ui32RowCount=(ip_pMatrix->getDimensionCount()==2?ip_pMatrix->getDimensionSize(0):1);
		uint32 l_ui32ColumnCount=(ip_pMatrix->getDimensionCount()==2?ip_pMatrix->getDimensionSize(1):ip_pMatrix->getDimensionSize(0));

		::GtkTable* l_pTable=GTK_TABLE(glade_xml_get_widget(m_pMainWidgetInterface, "level-measure-table"));
		gtk_table_resize(l_pTable, l_ui32RowCount, l_ui32ColumnCount);

		for(uint32 i=0; i<l_ui32RowCount; i++)
		{
			for(uint32 j=0; j<l_ui32ColumnCount; j++)
			{
				::GladeXML* l_pGladeXML=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-LevelMeasure.glade", "progress-bar-level", NULL);
				::GtkWidget* l_pWidget=glade_xml_get_widget(l_pGladeXML, "progress-bar-level");
				gtk_table_attach(
					l_pTable, l_pWidget,
					j, j+1, i, i+1,
					(::GtkAttachOptions)(GTK_EXPAND|GTK_FILL),
					(::GtkAttachOptions)(GTK_EXPAND|GTK_FILL),
					0, 0);
				g_object_unref(l_pGladeXML);

				CAlgorithmLevelMeasure::SProgressBar l_oProgressBar;
				l_oProgressBar.m_pProgressBar=GTK_PROGRESS_BAR(l_pWidget);
				l_oProgressBar.m_ui32Score=0;
				l_oProgressBar.m_bLastWasOverThreshold=false;
				m_vProgressBar.push_back(l_oProgressBar);
			}
		}

		op_pMainWidget=m_pMainWindow;
		op_pToolbarWidget=m_pToolbarWidget;
	}

	if(this->isInputTriggerActive(OVP_Algorithm_LevelMeasure_InputTriggerId_Refresh))
	{
		std::vector < CAlgorithmLevelMeasure::SProgressBar >::iterator it=m_vProgressBar.begin();

		float64* l_pInBuffer=ip_pMatrix->getBuffer();
		float64 l_f64Percent;

		uint32 l_ui32ElementCount=ip_pMatrix->getBufferElementCount();
		while(l_ui32ElementCount--)
		{
			l_f64Percent=*l_pInBuffer;
			if(l_f64Percent>1) l_f64Percent=1;
			if(l_f64Percent<0) l_f64Percent=0;

			if(l_f64Percent>m_f64Threshold && !it->m_bLastWasOverThreshold)
			{
				it->m_ui32Score++;
				it->m_bLastWasOverThreshold=true;
			}
			if(l_f64Percent<=m_f64Threshold)
			{
				it->m_bLastWasOverThreshold=false;
			}

			std::stringstream ss;
			ss << std::fixed;
			ss << std::setprecision(2);
			ss << "score : " << it->m_ui32Score << "\n";

			if(m_bShowPercentages)
			{
				ss << "level : " << l_f64Percent*100 << "%\n";
			}

			gtk_progress_bar_set_fraction(it->m_pProgressBar, l_f64Percent);
			gtk_progress_bar_set_text(it->m_pProgressBar, ss.str().c_str());

			l_pInBuffer++;
			it++;
		}

		this->activateOutputTrigger(OVP_Algorithm_LevelMeasure_OutputTriggerId_Refreshed, true);
	}

	return true;
}
