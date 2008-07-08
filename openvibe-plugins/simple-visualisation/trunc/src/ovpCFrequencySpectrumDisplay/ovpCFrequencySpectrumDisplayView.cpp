#include "ovpCFrequencySpectrumDisplayView.h"

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
/*
		void toggleLeftRulerButtonCallback(::GtkWidget *widget, gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);
			l_pView->toggleLeftRulers(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(widget))?true:false);
		}
*/
		void frequencySpectrumToggleBottomRulerButtonCallback(::GtkWidget *widget, gpointer data)
		{
			CFrequencySpectrumDisplayView* l_pView = reinterpret_cast<CFrequencySpectrumDisplayView*>(data);
			l_pView->toggleBottomRulers(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(widget))?true:false);
		}

		gboolean frequencySpectrumSpinButtonValueChangedCallback(::GtkSpinButton *widget,  gpointer data)
		{
			CFrequencySpectrumDisplayView* l_pView = reinterpret_cast<CFrequencySpectrumDisplayView*>(data);

			//Compute and save the nuew number of buffers to display
			OpenViBE::boolean l_bNumberOfDisplayedBufferChanged = l_pView->m_pBufferDatabase->adjustNumberOfDisplayedBuffers(gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget)));

			if(l_bNumberOfDisplayedBufferChanged)
			{
				//Redraw the window
				l_pView->redraw();
			}

			return FALSE;
		}

		//called when the channel select button is pressed (opens the channel selection dialog)
		void frequencySpectrumChannelSelectButtonCallback(::GtkButton *button, gpointer data)
		{
			CFrequencySpectrumDisplayView* l_pView = reinterpret_cast<CFrequencySpectrumDisplayView*>(data);
			::GtkWidget * l_pChannelSelectDialog = glade_xml_get_widget(l_pView->m_pGladeInterface, "DisplayChannelSelectDialog");

			//reset all checkbuttons
			for(size_t i=0 ; i<l_pView->m_vChannelsCheckButtons.size() ; i++)
			{
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pView->m_vChannelsCheckButtons[i]), false);
			}

			//sets only channels in the list
			for(size_t i=0 ; i<l_pView->m_vSelectedChannels.size() ; i++)
			{
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pView->m_vChannelsCheckButtons[(size_t)l_pView->m_vSelectedChannels[i]]), true);
			}

			//finally, show the information dialog
			gtk_widget_show_all(l_pChannelSelectDialog);
		}

		//Called when the user press the apply button of the channel selection dialog
		void frequencySpectrumChannelSelectDialogApplyButtonCallback(::GtkButton *button, gpointer data)
		{
			CFrequencySpectrumDisplayView* l_pView = reinterpret_cast<CFrequencySpectrumDisplayView*>(data);

			//clears the list of selected channels
			l_pView->m_vSelectedChannels.clear();

			for(unsigned int i=0 ; i<l_pView->m_vChannelsCheckButtons.size() ; i++)
			{
				if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pView->m_vChannelsCheckButtons[i])))
				{
					//if a button is checked, add the corresponding channel to the list of selected ones
					l_pView->m_vSelectedChannels.push_back(i);
					//show the channel's display
					l_pView->showChannel(i);
				}
				else
				{
					//this channel is not selected, hides it
					l_pView->hideChannel(i);
				}
			}

			l_pView->updateMainTableStatus();

			//hides the channel selection dialog
			gtk_widget_hide(glade_xml_get_widget(l_pView->m_pGladeInterface, "DisplayChannelSelectDialog"));
		}

		gboolean minMaxAttenuationSpinButtonValueChangedCallback(::GtkSpinButton *widget,  gpointer data)
		{
			reinterpret_cast<CFrequencySpectrumDisplayView*>(data)->setMinMaxAttenuation(static_cast<float64>(gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget))));

			return FALSE;
		}

		CFrequencySpectrumDisplayView::CFrequencySpectrumDisplayView(CBufferDatabase& oBufferDatabase)
			:m_pGladeInterface(NULL)
			,m_pBufferDatabase(&oBufferDatabase)
			,m_f64Attenuation(0)
			,m_pBottomRuler(NULL)
		{
			//load the glade interface
			m_pGladeInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-FrequencySpectrumDisplay.glade", NULL, NULL);

			if(!m_pGladeInterface)
			{
				g_warning("Couldn't load the interface!");
				return;
			}

			glade_xml_signal_autoconnect(m_pGladeInterface);

			//same for the other buttons
			// g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayToggleLeftRuler")),     "toggled",       G_CALLBACK(toggleLeftRulerButtonCallback),   this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayToggleBottomRuler")),   "toggled",       G_CALLBACK(frequencySpectrumToggleBottomRulerButtonCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectButton")), "clicked",       G_CALLBACK(frequencySpectrumChannelSelectButtonCallback),     this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayTimeScale")),           "value-changed", G_CALLBACK(frequencySpectrumSpinButtonValueChangedCallback),  this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayMinMaxAttenuationSpin")), "value-changed", G_CALLBACK(minMaxAttenuationSpinButtonValueChangedCallback),  this);

			//channel select dialog's signals
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectApplyButton")), "clicked", G_CALLBACK(frequencySpectrumChannelSelectDialogApplyButtonCallback), this);

			//connect the cancel button to the dialog's hide command
			g_signal_connect_swapped(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectCancelButton")),
				"clicked",
				G_CALLBACK(gtk_widget_hide),
				G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectDialog")));

			//hides the dialog if the user tries to close it
			 g_signal_connect (G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectDialog")),
				"delete_event", G_CALLBACK(gtk_widget_hide), NULL);

			//does nothing on the main window if the user tries to close it
			g_signal_connect (G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "DisplayMainWindow")),
				"delete_event", G_CALLBACK(gtk_widget_hide), NULL);

			// gets attenuation
			m_f64Attenuation=(gtk_spin_button_get_value(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "DisplayMinMaxAttenuationSpin"))));

			// sets duration
			m_pBufferDatabase->adjustNumberOfDisplayedBuffers(gtk_spin_button_get_value(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "DisplayTimeScale"))));

#if 0
			//creates the window
			m_pMainWindow = glade_xml_get_widget(m_pGladeInterface, "DisplayMainWindow");

			gtk_widget_show(m_pMainWindow);
#endif
		}

		CFrequencySpectrumDisplayView::~CFrequencySpectrumDisplayView()
		{
#if 0
			//destroy the window and its children
			if(m_pMainWindow)
			{
				gtk_widget_destroy(m_pMainWindow);
				m_pMainWindow = NULL;
			}
#endif

			for(uint32 i=0 ; i<m_oChannelDisplay.size() ; i++)
			{
				delete m_oChannelDisplay[i];
			}

			/* unref the xml file as it's not needed anymore */
			g_object_unref(G_OBJECT(m_pGladeInterface));
			m_pGladeInterface=NULL;
		}

		void CFrequencySpectrumDisplayView::getWidgets(::GtkWidget*& pWidget, ::GtkWidget*& pToolbarWidget)
		{
			pWidget=glade_xml_get_widget(m_pGladeInterface, "FrequencySpectrumDisplayScrolledWindow");
			pToolbarWidget=glade_xml_get_widget(m_pGladeInterface, "Toolbar");
		}

		void CFrequencySpectrumDisplayView::init(void)
		{
			OpenViBE::uint32 l_ui32ChannelCount = static_cast<uint32>(m_pBufferDatabase->m_pDimmensionSizes[0]);
			vector<string>& l_oChannelName = m_pBufferDatabase->m_pDimmesionLabels[0];

			stringstream l_oLabelString;
			::GtkWidget * l_pChannelSelectList = glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectList");

			//size group for the channel labels and the empty widget in the bottom bar
			//(useful to position the bottom ruler correctly)
			::GtkSizeGroup * l_pSizeGroup = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

			//creates the array of Channel displays
			m_oChannelDisplay.resize(l_ui32ChannelCount);
			m_oChannelLabel.resize(l_ui32ChannelCount);

			//gets the pointer to the channels table
			m_pDisplayTable = glade_xml_get_widget(m_pGladeInterface, "DisplayMainTable");

			//resize this table according to the number of channels
			gtk_table_resize(GTK_TABLE(m_pDisplayTable), (l_ui32ChannelCount)*2, 3);

			//sets a minimum size for the table (needed to scroll)
			gtk_widget_set_size_request(m_pDisplayTable, 20, (l_ui32ChannelCount)*20);

			//Add a vertical separator
			::GtkWidget* l_pSeparator = gtk_vseparator_new();
			gtk_table_attach(GTK_TABLE(m_pDisplayTable), l_pSeparator,
				1, 2, 0, (l_ui32ChannelCount)*2, GTK_SHRINK, static_cast< ::GtkAttachOptions >(GTK_EXPAND | GTK_FILL), 0, 0);
			gtk_widget_show(l_pSeparator);

			//creates as many widget as there are channels and adds them to the table
			for(uint32 i=0 ; i<l_ui32ChannelCount ; i++)
			{
				//adds a label with the channel's name
				if(l_oChannelName[i] == "")
				{
					//If no name has been set, just keep the channel index
					l_oLabelString<<"Channel "<<i;
				}
				else
				{	//else keep the index plus the name
					l_oLabelString<<i<<" : "<<l_oChannelName[i];
				}
				::GtkWidget * l_pLabel =  gtk_label_new(l_oLabelString.str().c_str());

				m_oChannelLabel[i] = l_pLabel;

				gtk_table_attach(GTK_TABLE(m_pDisplayTable),l_pLabel,
					0, 1, i*2, (i*2)+1,
					GTK_FILL, GTK_SHRINK,
					0, 0);

				//
				gtk_widget_show(l_pLabel);

				//adds the label to the size group
				gtk_size_group_add_widget(l_pSizeGroup, l_pLabel);

				//creates the channel display widget
				m_oChannelDisplay[i] = new CFrequencySpectrumChannelDisplay(i, *m_pBufferDatabase);

				//adds it to the table
				gtk_table_attach(GTK_TABLE(m_pDisplayTable), m_oChannelDisplay[i]->getWidget(),
				2, 3, (i*2), (i*2)+1,
				static_cast< ::GtkAttachOptions >(GTK_EXPAND | GTK_FILL),
				static_cast< ::GtkAttachOptions >(GTK_EXPAND | GTK_FILL), 0, 0);

				//gtk_widget_show_all(m_oChannelDisplay[i]);

				//Add an horizontal separator under it
				l_pSeparator = gtk_hseparator_new();
				gtk_table_attach(GTK_TABLE(m_pDisplayTable), l_pSeparator,
				0, 3, (i*2)+1, (i*2)+2,	static_cast< ::GtkAttachOptions >(GTK_EXPAND | GTK_FILL), GTK_SHRINK, 0, 0);
				gtk_widget_show(l_pSeparator);

				//Adds a checkbox in the channel select window
				::GtkWidget * l_pChannelCheckButton = gtk_check_button_new_with_label(l_oLabelString.str().c_str());
				m_vChannelsCheckButtons.push_back(l_pChannelCheckButton);

				gtk_box_pack_start_defaults(GTK_BOX(l_pChannelSelectList), l_pChannelCheckButton);

				l_oLabelString.str("");

				//a channel is selected by default
				m_vSelectedChannels.push_back(i);
			}

			//Adds the bottom ruler
			m_pBottomRuler = new CBottomTimeRuler(*m_pBufferDatabase);

			//adds the empty label to the size group (it will request the same height than the channel labels
			gtk_size_group_add_widget(l_pSizeGroup, glade_xml_get_widget(m_pGladeInterface, "DisplayBottomBarEmptyLabel1"));

			//adds the bottom ruler to the bottom bar (after the empty label)
			gtk_box_pack_start(GTK_BOX(glade_xml_get_widget(m_pGladeInterface, "DisplayBottomBar")),
					m_pBottomRuler->getWidget(),
					false, false, 0);

			// tells the ruler that it has to resize when the channel displays are resized
			if(m_oChannelDisplay.size() != 0)
			{
				m_pBottomRuler->linkWidthToWidget(m_oChannelDisplay[0]->getWidget());
			}

#if 0
			//finally, show the window
			gtk_widget_show_all(m_pMainWindow);
#endif

			//Don't display left ruler (default)
			toggleLeftRulers(false);

			//Dislay bottom ruler
			toggleBottomRulers(true);

			activateToolbarButtons(true);
		}

		void CFrequencySpectrumDisplayView::redraw()
		{
			for(size_t i=0 ; i<m_oChannelDisplay.size() ; i++)
			{
				m_oChannelDisplay[i]->setMinMaxAttenuation(m_f64Attenuation);
				m_oChannelDisplay[i]->update();
			}

			//nothing to redraw if the table isn't visible
			if(m_pDisplayTable && GTK_WIDGET_VISIBLE(m_pDisplayTable))
			{
				boolean l_bNotInitialized = false;

				if(!l_bNotInitialized && m_pBufferDatabase->m_pDimmensionSizes[0]!=0)
				{

					//The ruler too
					gdk_window_invalidate_rect(GTK_WIDGET(m_pBottomRuler->getWidget())->window,
							NULL,
							true);

					//The channels table needs to be redrawn
					gdk_window_invalidate_rect(GTK_WIDGET(m_pDisplayTable)->window,
							NULL,
							true);
				}
			}
		}

		void CFrequencySpectrumDisplayView::toggleLeftRulers(OpenViBE::boolean bActive)
		{/*
			for(size_t i=0 ; i<m_oChannelDisplay.size() ; i++)
			{
				CHANNEL_DISPLAY(m_oChannelDisplay[i])->m_pChannelDisplay->toggleLeftRuler(bActive);
			}
		*/}

		void CFrequencySpectrumDisplayView::toggleBottomRulers(OpenViBE::boolean bActive)
		{
			m_pBottomRuler->toggle(bActive);
		}

		void CFrequencySpectrumDisplayView::hideChannel(OpenViBE::uint32 ui32ChannelIndex)
		{
			m_oChannelDisplay[ui32ChannelIndex]->toggle(false);
			gtk_widget_hide( m_oChannelLabel[ui32ChannelIndex] );
		}

		void CFrequencySpectrumDisplayView::showChannel(OpenViBE::uint32 ui32ChannelIndex)
		{
			m_oChannelDisplay[ui32ChannelIndex]->toggle(true);
			gtk_widget_show( m_oChannelLabel[ui32ChannelIndex] );
		}

		void CFrequencySpectrumDisplayView::updateMainTableStatus()
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

		void CFrequencySpectrumDisplayView::activateToolbarButtons(boolean bActive)
		{
			// gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayToggleLeftRuler"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "DisplayToggleBottomRuler"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "DisplayTimeScaleItem"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "DisplayChannelSelectButton"), bActive);
		}

		void CFrequencySpectrumDisplayView::setMinMaxAttenuation(OpenViBE::float64 f64Attenuation)
		{
			m_f64Attenuation=f64Attenuation;
		}
	};
};
