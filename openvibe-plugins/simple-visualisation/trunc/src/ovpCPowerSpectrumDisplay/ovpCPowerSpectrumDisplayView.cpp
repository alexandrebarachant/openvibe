#include "ovpCPowerSpectrumDisplayView.h"

#include <iostream>

#include <sstream>

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
		void powerSpectrumToggleGlobalBestFitButtonCallback(GtkWidget* widget, gpointer data);
		void powerSpectrumToggleLocalBestFitButtonCallback(GtkWidget* widget, gpointer data);
		void powerSpectrumToggleLeftRulerButtonCallback(GtkWidget* widget, gpointer data);
		void powerSpectrumToggleBottomRulerButtonCallback(GtkWidget *widget, gpointer data);
		void powerSpectrumChannelSelectButtonCallback(GtkButton *button, gpointer data);
		void powerSpectrumChannelSelectDialogApplyButtonCallback(GtkButton *button, gpointer data);

		CPowerSpectrumDisplayView::CPowerSpectrumDisplayView(CPowerSpectrumDatabase& oPowerSpectrumDatabase) :
			m_pGladeInterface(NULL),
			m_pDisplayTable(NULL),
			m_ui32CurrentDisplayMode(ESpectrumDisplayMode_GlobalBestFit),
			m_pPowerSpectrumDatabase(&oPowerSpectrumDatabase)
		{
			//load the glade interface
			m_pGladeInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-PowerSpectrumDisplay.glade", NULL, NULL);

			if(!m_pGladeInterface)
			{
				g_warning("Couldn't load the interface!");
				return;
			}

			glade_xml_signal_autoconnect(m_pGladeInterface);

			//toolbar
			//-------

			//get pointers to display mode buttons
			m_pDisplayModeButtons[ESpectrumDisplayMode_LocalBestFit] =
				GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "BestFitPerChannel"));
			m_pDisplayModeButtons[ESpectrumDisplayMode_GlobalBestFit] =
				GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "BestFit"));

			//connect callbacks
			enableDisplayModeButtonSignals(true);

			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "ShowPowers")),
				"toggled", G_CALLBACK(powerSpectrumToggleLeftRulerButtonCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "ShowFrequencies")),
				"toggled", G_CALLBACK(powerSpectrumToggleBottomRulerButtonCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SelectChannels")),
				"clicked", G_CALLBACK(powerSpectrumChannelSelectButtonCallback), this);
			//hide toolbar on delete event
			g_signal_connect (G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "Toolbar")),
				"delete_event", G_CALLBACK(gtk_widget_hide), NULL);

			//channels selection
			//------------------

			//apply button click
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectApplyButton")),
				"clicked", G_CALLBACK(powerSpectrumChannelSelectDialogApplyButtonCallback), this);
			//cancel button click
			g_signal_connect_swapped(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectCancelButton")),
				"clicked", G_CALLBACK(gtk_widget_hide), G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectDialog")));
			//hide dialog on delete event
			g_signal_connect (G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectDialog")),
				"delete_event", G_CALLBACK(gtk_widget_hide), NULL);
		}

		void CPowerSpectrumDisplayView::getWidgets(GtkWidget*& pWidget, GtkWidget*& pToolbarWidget)
		{
			pWidget = glade_xml_get_widget(m_pGladeInterface, "DisplayScrolledWindow");
			pToolbarWidget = glade_xml_get_widget(m_pGladeInterface, "Toolbar");
		}

		ESpectrumDisplayMode CPowerSpectrumDisplayView::getCurrentDisplayMode()
		{
			return m_ui32CurrentDisplayMode;
		}

		CPowerSpectrumDisplayView::~CPowerSpectrumDisplayView()
		{
			//destroy channel widgets
			for(uint32 i=0 ; i<m_oChannelDisplays.size() ; i++)
				delete m_oChannelDisplays[i];

			//unref the xml file as it's not needed anymore
			g_object_unref(G_OBJECT(m_pGladeInterface));
			m_pGladeInterface=NULL;
		}

		void CPowerSpectrumDisplayView::init()
		{
			//retrieve channels count
			OpenViBE::uint32 l_ui32ChannelCount = m_pPowerSpectrumDatabase->m_pChannelLabels.size();

			//allocate channel labels and channel displays arrays accordingly
			m_oChannelLabels.resize(l_ui32ChannelCount);
			m_oChannelDisplays.resize(l_ui32ChannelCount);

			//retrieve and allocate main table accordingly
			m_pDisplayTable = glade_xml_get_widget(m_pGladeInterface, "DisplayMainTable");
			//rows : for each channel, [0] channel data, [1] horizontal separator
			//columns : [0] labels, [1] vertical separator, [2] spectrum displays
			gtk_table_resize(GTK_TABLE(m_pDisplayTable), (l_ui32ChannelCount)*2, 3);
			//set a minimum size for the table (needed to scroll)
			gtk_widget_set_size_request(m_pDisplayTable, 200, (l_ui32ChannelCount)*30);
			//add a vertical separator
			GtkWidget* l_pSeparator = gtk_vseparator_new();
			gtk_table_attach(GTK_TABLE(m_pDisplayTable), l_pSeparator,
				1, 2, //2nd column
				0, (l_ui32ChannelCount)*2, //whole table height
				GTK_SHRINK, static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), 0, 0);
			gtk_widget_show(l_pSeparator);

			//create a size group for channel labels and the empty bottom left widget
			GtkSizeGroup* l_pSizeGroup = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

			//create channel widgets and add them to display table
			stringstream l_oLabelString;
			GtkWidget* l_pChannelSelectList = glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectList");

			for(uint32 i=0 ; i<l_ui32ChannelCount ; i++)
			{
				//create channel name
				if(m_pPowerSpectrumDatabase->m_pChannelLabels[i] == "")
				{
					//if no name was set, use channel index
					l_oLabelString << "Channel " << i;
				}
				else //prepend name with channel index
				{
					l_oLabelString << i <<" : " << m_pPowerSpectrumDatabase->m_pChannelLabels[i];
				}

				//create channel label widget and store it
				GtkWidget* l_pLabel = gtk_label_new(l_oLabelString.str().c_str());
				m_oChannelLabels[i] = l_pLabel;

				//attach label to table
				gtk_table_attach(GTK_TABLE(m_pDisplayTable),l_pLabel,
					0, 1, //1st column
					i*2, (i*2)+1, //ith line
					GTK_FILL, GTK_SHRINK,	0, 0);

				gtk_widget_show(l_pLabel);

				//add label to size group
				gtk_size_group_add_widget(l_pSizeGroup, l_pLabel);

				//create channel display widget
				m_oChannelDisplays[i] = new CPowerSpectrumChannelDisplay(this, i, *m_pPowerSpectrumDatabase);

				//attach it to table
				gtk_table_attach(GTK_TABLE(m_pDisplayTable), m_oChannelDisplays[i]->getWidget(),
				2, 3, //3rd column
				(i*2), (i*2)+1, //ith line (top)
				static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), 0, 0);

				gtk_widget_show_all(m_oChannelDisplays[i]->getWidget());

				//add a horizontal separator under it
				l_pSeparator = gtk_hseparator_new();
				gtk_table_attach(GTK_TABLE(m_pDisplayTable), l_pSeparator,
					0, 3, //whole width of the table
					(i*2)+1, (i*2)+2,	//ith line (bottom)
					static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), GTK_SHRINK,	0, 0);
				gtk_widget_show(l_pSeparator);

				//add a checkbox to channel selection window
				GtkWidget* l_pChannelCheckButton = gtk_check_button_new_with_label(l_oLabelString.str().c_str());
				gtk_box_pack_start_defaults(GTK_BOX(l_pChannelSelectList), l_pChannelCheckButton);

				//store a pointer to channel checkbox
				m_vChannelsCheckButtons.push_back(l_pChannelCheckButton);

				//select channel by default
				m_vSelectedChannels.push_back(i);

				l_oLabelString.str("");
			}

			//activate buttons
			activateToolbarButtons(true);

			//use global best fit by default
			setDisplayMode(ESpectrumDisplayMode_GlobalBestFit);

			//display left ruler by default
			toggleLeftRulers(true);

			//display bottom ruler by default
			toggleBottomRulers(true);
		}

		void CPowerSpectrumDisplayView::redraw()
		{
			//ensure display table is created and visible
			if(m_pDisplayTable && GTK_WIDGET_VISIBLE(m_pDisplayTable))
			{
				//update channel widgets
				for(size_t i=0 ; i<m_vSelectedChannels.size() ; i++)
				{
					m_oChannelDisplays[m_vSelectedChannels[i]]->update();
				}

				if(m_vSelectedChannels.size() > 0)
				{
					//redraw table
					gdk_window_invalidate_rect(GTK_WIDGET(m_pDisplayTable)->window,	NULL,	true);
				}
			}
		}

		void CPowerSpectrumDisplayView::enableDisplayModeButtonSignals(OpenViBE::boolean enable)
		{
			if(enable == true)
			{
				g_signal_connect(G_OBJECT(m_pDisplayModeButtons[ESpectrumDisplayMode_LocalBestFit]),
					"toggled", G_CALLBACK (powerSpectrumToggleLocalBestFitButtonCallback), this);
				g_signal_connect(G_OBJECT(m_pDisplayModeButtons[ESpectrumDisplayMode_GlobalBestFit]),
					"toggled", G_CALLBACK (powerSpectrumToggleGlobalBestFitButtonCallback), this);
			}
			else
			{
				g_signal_handlers_disconnect_by_func(G_OBJECT(m_pDisplayModeButtons[ESpectrumDisplayMode_LocalBestFit]),
					reinterpret_cast<void*>(G_CALLBACK (powerSpectrumToggleLocalBestFitButtonCallback)), this);
				g_signal_handlers_disconnect_by_func(G_OBJECT(m_pDisplayModeButtons[ESpectrumDisplayMode_GlobalBestFit]),
					reinterpret_cast<void*>(G_CALLBACK (powerSpectrumToggleGlobalBestFitButtonCallback)), this);
			}
		}

		void CPowerSpectrumDisplayView::setDisplayMode(ESpectrumDisplayMode eDisplayMode)
		{
			m_ui32CurrentDisplayMode = eDisplayMode;

			enableDisplayModeButtonSignals(false);

			for(ESpectrumDisplayMode eSDM = ESpectrumDisplayMode_LocalBestFit;
				eSDM != ESpectrumDisplayMode_NumDisplayMode;
				eSDM = (ESpectrumDisplayMode)(eSDM + 1))
			{
				gtk_toggle_tool_button_set_active(
					m_pDisplayModeButtons[eSDM],
					(eSDM == m_ui32CurrentDisplayMode) ? TRUE : FALSE);
			}

			enableDisplayModeButtonSignals(true);
		}

		void CPowerSpectrumDisplayView::toggleLeftRulers(OpenViBE::boolean bActive)
		{
			//forward toggle flag to channel displays
			for(size_t i=0 ; i<m_oChannelDisplays.size() ; i++)
				m_oChannelDisplays[i]->toggleLeftRuler(bActive);

			//update button state
			gtk_toggle_tool_button_set_active(
				GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "ShowPowers")),
				bActive ? TRUE : FALSE);
		}

		void CPowerSpectrumDisplayView::toggleBottomRulers(OpenViBE::boolean bActive)
		{
			//forward toggle flag to channel displays
			for(size_t i=0; i<m_oChannelDisplays.size(); i++)
				m_oChannelDisplays[i]->toggleBottomRuler(bActive);

			//update button state
			gtk_toggle_tool_button_set_active(
				GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "ShowFrequencies")),
				bActive ? TRUE : FALSE);
		}

		void CPowerSpectrumDisplayView::hideChannel(OpenViBE::uint32 ui32ChannelIndex)
		{
			if(ui32ChannelIndex < m_oChannelDisplays.size())
			{
				m_oChannelDisplays[ui32ChannelIndex]->toggle(false);
				gtk_widget_hide(m_oChannelLabels[ui32ChannelIndex]);
			}
		}

		void CPowerSpectrumDisplayView::showChannel(OpenViBE::uint32 ui32ChannelIndex)
		{
			if(ui32ChannelIndex < m_oChannelDisplays.size())
			{
				m_oChannelDisplays[ui32ChannelIndex]->toggle(true);
				gtk_widget_show(m_oChannelLabels[ui32ChannelIndex]);
			}
		}

		void CPowerSpectrumDisplayView::updateMainTableStatus()
		{
			//if nothing has been selected
			if(m_vSelectedChannels.empty())
			{
				//hide the whole table
				gtk_widget_hide(GTK_WIDGET(m_pDisplayTable));
			}
			else
			{
				if(!GTK_WIDGET_VISIBLE(GTK_WIDGET(m_pDisplayTable)))
				{
					//if there were no selected channels before, but now there are, show the table again
					gtk_widget_show(GTK_WIDGET(m_pDisplayTable));
				}
			}
		}

		void CPowerSpectrumDisplayView::activateToolbarButtons(boolean bActive)
		{
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "BestFit"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "BestFitPerChannel"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "Info"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SelectChannels"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "ShowPowers"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "ShowFrequencies"), bActive);
		}

		void CPowerSpectrumDisplayView::showChannelSelectionDialogCB()
		{
			GtkWidget * l_pChannelSelectDialog = glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectDialog");

			//reset all checkbuttons
			for(size_t i=0 ; i<m_vChannelsCheckButtons.size() ; i++)
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_vChannelsCheckButtons[i]), false);

			//check visible channels
			for(size_t i=0 ; i<m_vSelectedChannels.size() ; i++)
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_vChannelsCheckButtons[(size_t)m_vSelectedChannels[i]]), true);

			gtk_widget_show_all(l_pChannelSelectDialog);
		}

		void CPowerSpectrumDisplayView::applyChannelSelectionCB()
		{
			//clears the list of selected channels
			m_vSelectedChannels.clear();

			for(unsigned int i=0 ; i<m_vChannelsCheckButtons.size() ; i++)
			{
				if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_vChannelsCheckButtons[i])))
				{
					//if a button is checked, add the corresponding channel to the list of selected ones
					m_vSelectedChannels.push_back(i);
					//show the channel's display
					showChannel(i);
				}
				else
				{
					//this channel is not selected, hides it
					hideChannel(i);
				}
			}

			updateMainTableStatus();

			//hides the channel selection dialog
			gtk_widget_hide(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectDialog"));
		}

		//CALLBACKS

		void powerSpectrumToggleGlobalBestFitButtonCallback(GtkWidget* widget, gpointer data)
		{
			CPowerSpectrumDisplayView* l_pPowerSpectrumDisplayView = reinterpret_cast<CPowerSpectrumDisplayView*>(data);
			l_pPowerSpectrumDisplayView->setDisplayMode(ESpectrumDisplayMode_GlobalBestFit);
		}

		void powerSpectrumToggleLocalBestFitButtonCallback(GtkWidget* widget, gpointer data)
		{
			CPowerSpectrumDisplayView* l_pPowerSpectrumDisplayView = reinterpret_cast<CPowerSpectrumDisplayView*>(data);
			l_pPowerSpectrumDisplayView->setDisplayMode(ESpectrumDisplayMode_LocalBestFit);
		}

		void powerSpectrumToggleLeftRulerButtonCallback(GtkWidget* widget, gpointer data)
		{
			CPowerSpectrumDisplayView* l_pPowerSpectrumDisplayView = reinterpret_cast<CPowerSpectrumDisplayView*>(data);
			l_pPowerSpectrumDisplayView->toggleLeftRulers(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(widget))?true:false);
		}

		void powerSpectrumToggleBottomRulerButtonCallback(GtkWidget *widget, gpointer data)
		{
			CPowerSpectrumDisplayView* l_pPowerSpectrumDisplayView = reinterpret_cast<CPowerSpectrumDisplayView*>(data);
			l_pPowerSpectrumDisplayView->toggleBottomRulers(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(widget))?true:false);
		}

		//called when the channel select button is pressed (opens the channel selection dialog)
		void powerSpectrumChannelSelectButtonCallback(GtkButton *button, gpointer data)
		{
			CPowerSpectrumDisplayView* l_pPowerSpectrumDisplayView = reinterpret_cast<CPowerSpectrumDisplayView*>(data);
			l_pPowerSpectrumDisplayView->showChannelSelectionDialogCB();
		}

		//Called when the user presses the apply button of the channel selection dialog
		void powerSpectrumChannelSelectDialogApplyButtonCallback(GtkButton *button, gpointer data)
		{
			CPowerSpectrumDisplayView* l_pPowerSpectrumDisplayView = reinterpret_cast<CPowerSpectrumDisplayView*>(data);
			l_pPowerSpectrumDisplayView->applyChannelSelectionCB();
		}
	};
};
