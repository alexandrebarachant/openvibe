#include "ovpCSignalDisplayView.h"

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
		void scrollModeButtonCallback(GtkWidget *widget, gpointer data)
		{
			reinterpret_cast<CSignalDisplayView*>(data)->onDisplayModeToggledCB(
				gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(widget)) == TRUE ?
					OVP_TypeId_SignalDisplayMode_Scroll : OVP_TypeId_SignalDisplayMode_Scan);
		}

		void scanModeButtonCallback(GtkWidget *widget, gpointer data)
		{
			reinterpret_cast<CSignalDisplayView*>(data)->onDisplayModeToggledCB(
				gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(widget)) == TRUE ?
					OVP_TypeId_SignalDisplayMode_Scan : OVP_TypeId_SignalDisplayMode_Scroll);
		}

		void toggleLeftRulerButtonCallback(GtkWidget *widget, gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);
			l_pView->toggleLeftRulers(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(widget))?true:false);
		}

		void toggleBottomRulerButtonCallback(GtkWidget *widget, gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);
			l_pView->toggleBottomRuler(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(widget))?true:false);
		}

		void toggleAutoVerticalScaleButtonCallback(GtkToggleButton *togglebutton, gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);
			l_pView->onVerticalScaleModeToggledCB(togglebutton);
		}

		void customVerticalScaleChangedCallback(::GtkSpinButton* pSpinButton, gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);
			l_pView->onCustomVerticalScaleChangedCB(pSpinButton);
		}

		gboolean spinButtonValueChangedCallback(GtkSpinButton *widget,  gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);

			//Compute and save the new number of buffers to display
			OpenViBE::boolean l_bNumberOfDisplayedBufferChanged = l_pView->m_pBufferDatabase->adjustNumberOfDisplayedBuffers(gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget)));

			if(l_bNumberOfDisplayedBufferChanged)
			{
				//reserve the maximum space needed for computing the points to display
				//(when cropping the lines, there can be up to two times the number of original points)
				l_pView->m_pPoints.reserve((size_t)(l_pView->m_pBufferDatabase->m_pDimmensionSizes[1] * l_pView->m_pBufferDatabase->m_ui64NumberOfBufferToDisplay * 2));

				//resize the vector of raw points (before cropping)
				l_pView->m_pRawPoints.resize((size_t)(l_pView->m_pBufferDatabase->m_pDimmensionSizes[1] * l_pView->m_pBufferDatabase->m_ui64NumberOfBufferToDisplay));

				//force full redraw of all channels when time scale changes
				for(size_t i=0 ; i<l_pView->m_oChannelDisplay.size(); i++)
				{
					l_pView->getChannelDisplay(i)->updateScale();
				}

				//redraw channels
				l_pView->redraw();
			}

			return FALSE;
		}

		//called when the channel select button is pressed (opens the channel selection dialog)
		void channelSelectButtonCallback(GtkButton *button, gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);
			GtkWidget * l_pChannelSelectDialog = glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplayChannelSelectDialog");

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

		//Called when the user presses the apply button of the channel selection dialog
		void channelSelectDialogApplyButtonCallback(GtkButton *button, gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);

			//clears the list of selected channels
			l_pView->m_vSelectedChannels.clear();

			for(unsigned int i=0 ; i<l_pView->m_vChannelsCheckButtons.size() ; i++)
			{
				if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pView->m_vChannelsCheckButtons[i])))
				{
					//if a button is checked, add the corresponding channel to the list of selected ones
					l_pView->m_vSelectedChannels.push_back(i);
					//show the channel's display
					l_pView->toggleChannel(i, true);
				}
				else
				{
					//this channel is not selected, hides it
					l_pView->toggleChannel(i, false);
				}
			}

			l_pView->updateMainTableStatus();

			//hides the channel selection dialog
			gtk_widget_hide(glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplayChannelSelectDialog"));
		}

		//Called when the user presses the Information button (opens the information dialog)
		void informationButtonCallback(GtkButton *button, gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);

			//gets the different values from the database and updates the corresponding label's text field
			stringstream l_oValueString;
			l_oValueString<<l_pView->m_pBufferDatabase->m_pDimmensionSizes[0];
			gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplayNumberOfChannels")),
				 l_oValueString.str().c_str() );

			l_oValueString.str("");
			l_oValueString<<l_pView->m_pBufferDatabase->m_ui32SamplingFrequency;
			gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplaySamplingFrequency")),
				l_oValueString.str().c_str() );

			l_oValueString.str("");
			l_oValueString<<l_pView->m_pBufferDatabase->m_pDimmensionSizes[1];
			gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplaySamplesPerBuffer")),
				l_oValueString.str().c_str() );

			l_oValueString.str("");
			l_oValueString<<l_pView->m_pBufferDatabase->m_f64MinimumValue;
			gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplayMinimumValue")),
				l_oValueString.str().c_str() );

			l_oValueString.str("");
			l_oValueString<<l_pView->m_pBufferDatabase->m_f64MaximumValue;
			gtk_label_set_text(GTK_LABEL(glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplayMaximumValue")),
				   l_oValueString.str().c_str() );

			GtkWidget * l_pInformationDialog = glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplayInformationDialog");

			//connect the close button to the dialog's hide command
			g_signal_connect_swapped(G_OBJECT(glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplayInformationClose")),
					"clicked",
					G_CALLBACK(gtk_widget_hide),
					G_OBJECT(l_pInformationDialog));

			//finally, show the information dialog
			gtk_widget_show(l_pInformationDialog);
		}

		//called when the channel select button is pressed (opens the channel selection dialog)
		void multiViewButtonCallback(GtkButton *button, gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);

			GtkWidget * l_pMultiViewDialog = glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplayMultiViewDialog");

			//reset all checkbuttons
			for(size_t i=0 ; i<l_pView->m_vMultiViewChannelsCheckButtons.size() ; i++)
			{
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pView->m_vMultiViewChannelsCheckButtons[i]), false);
			}

			//sets only channels in the list
			for(size_t i=0 ; i<l_pView->m_vMultiViewSelectedChannels.size() ; i++)
			{
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(l_pView->m_vMultiViewChannelsCheckButtons[(size_t)l_pView->m_vMultiViewSelectedChannels[i]]), true);
			}

			//finally, show the information dialog
			gtk_widget_show_all(l_pMultiViewDialog);
		}

		//Called when the user presses the apply button of the channel selection dialog
		void multiViewDialogApplyButtonCallback(GtkButton *button, gpointer data)
		{
			CSignalDisplayView* l_pView = reinterpret_cast<CSignalDisplayView*>(data);

			//clears the list of selected channels
			l_pView->m_vMultiViewSelectedChannels.clear();

			for(unsigned int i=0 ; i<l_pView->m_vMultiViewChannelsCheckButtons.size() ; i++)
			{
				if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pView->m_vMultiViewChannelsCheckButtons[i])))
				{
					//if a button is checked, add the corresponding channel to the list of selected ones
					l_pView->m_vMultiViewSelectedChannels.push_back(i);
				}
			}

			l_pView->changeMultiView();

			l_pView->updateMainTableStatus();

			//hides the channel selection dialog
			gtk_widget_hide(glade_xml_get_widget(l_pView->m_pGladeInterface, "SignalDisplayMultiViewDialog"));
		}

		CSignalDisplayView::CSignalDisplayView(CBufferDatabase& oBufferDatabase, float64 f64TimeScale, CIdentifier oDisplayMode)
			:m_pGladeInterface(NULL)
			,m_bShowLeftRulers(false)
			,m_bShowBottomRuler(true)
			,m_ui64LeftmostDisplayedTime(0)
			,m_f64LargestDisplayedValueRange(0)
			,m_f64ValueRangeMargin(0)
			,m_f64MarginFactor(0.4f) //add 40% space above and below extremums
			,m_bVerticalScaleChanged(false)
			,m_bAutoVerticalScale(true)
			,m_f64CustomVerticalScaleValue(1.)
			,m_pBufferDatabase(&oBufferDatabase)
			,m_bMultiViewInitialized(false)
			,m_pBottomBox(NULL)
			,m_pBottomRuler(NULL)
		{
			//load the glade interface
			m_pGladeInterface=glade_xml_new("../share/openvibe-plugins/simple-visualisation/openvibe-simple-visualisation-SignalDisplay.glade", NULL, NULL);

			if(!m_pGladeInterface)
			{
				g_warning("Couldn't load the interface!");
				return;
			}

			glade_xml_signal_autoconnect(m_pGladeInterface);

			//show widgets hierarchy
			//gtk_widget_show_all(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayScrolledWindow"));

			//initialize display mode
			m_pBufferDatabase->setDisplayMode(oDisplayMode);
			gtk_toggle_tool_button_set_active(
				GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayScrollModeButton")),
				oDisplayMode == OVP_TypeId_SignalDisplayMode_Scroll);

			//connect display mode callbacks
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayScrollModeButton")), "toggled", G_CALLBACK (scrollModeButtonCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayScanModeButton")), "toggled", G_CALLBACK (scanModeButtonCallback), this);

			//creates the cursors
			m_pCursor[0] = gdk_cursor_new(GDK_LEFT_PTR);
			m_pCursor[1] = gdk_cursor_new(GDK_SIZING);

			//button callbacks
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayChannelSelectButton")), "clicked",       G_CALLBACK(channelSelectButtonCallback),     this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayMultiViewButton")),     "clicked",       G_CALLBACK(multiViewButtonCallback),         this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayInformationButton")),   "clicked",       G_CALLBACK(informationButtonCallback),       this);

			//initialize vertical scale
			m_bAutoVerticalScale = true;
			m_f64CustomVerticalScaleValue = 1;
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayVerticalScaleToggleButton")), m_bAutoVerticalScale);
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayCustomVerticalScaleSpinButton")), m_f64CustomVerticalScaleValue);

			//connect vertical scale callbacks
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayVerticalScaleToggleButton")),     "toggled",       G_CALLBACK(toggleAutoVerticalScaleButtonCallback), this);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayCustomVerticalScaleSpinButton")), "value-changed", G_CALLBACK(customVerticalScaleChangedCallback), this);

			//time scale
			GtkSpinButton* l_pSpinButton = GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayTimeScale"));
			gtk_spin_button_set_value(l_pSpinButton, f64TimeScale);
			g_signal_connect(G_OBJECT(l_pSpinButton), "value-changed", G_CALLBACK(spinButtonValueChangedCallback),  this);
			//notify database of current time scale
			m_pBufferDatabase->adjustNumberOfDisplayedBuffers(gtk_spin_button_get_value(l_pSpinButton));

			//channel select dialog's signals
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayChannelSelectApplyButton")), "clicked", G_CALLBACK(channelSelectDialogApplyButtonCallback), this);

			//connect the cancel button to the dialog's hide command
			g_signal_connect_swapped(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayChannelSelectCancelButton")),
					"clicked",
					G_CALLBACK(gtk_widget_hide),
					G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayChannelSelectDialog")));

			//hides the dialog if the user tries to close it
			g_signal_connect (G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayChannelSelectDialog")),
					"delete_event",
					G_CALLBACK(gtk_widget_hide), NULL);

			//multiview signals
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayMultiViewApplyButton")), "clicked", G_CALLBACK(multiViewDialogApplyButtonCallback), this);

			//connect the cancel button to the dialog's hide command
			g_signal_connect_swapped(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayMultiViewCancelButton")),
				"clicked",
				G_CALLBACK(gtk_widget_hide),
				G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayMultiViewDialog")));

			//hides the dialog if the user tries to close it
			 g_signal_connect (G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayMultiViewDialog")),
				 "delete_event",
				 G_CALLBACK(gtk_widget_hide), NULL);

			//bottom box
			m_pBottomBox = GTK_BOX(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayBottomBox"));
		}

		CSignalDisplayView::~CSignalDisplayView()
		{
			//destroy the window and its children
			gtk_widget_destroy(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayMainWindow"));

			//destroy the rest
			for(int i=0 ; i<2 ; i++)
			{
				gdk_cursor_unref(m_pCursor[i]);
			}

			//unref the xml file as it's not needed anymore
			g_object_unref(G_OBJECT(m_pGladeInterface));
			m_pGladeInterface=NULL;
		}

		void CSignalDisplayView::getWidgets(GtkWidget*& pWidget, GtkWidget*& pToolbarWidget)
		{
			pWidget=glade_xml_get_widget(m_pGladeInterface, "SignalDisplayScrolledWindow");
			pToolbarWidget=glade_xml_get_widget(m_pGladeInterface, "Toolbar");
		}

		void CSignalDisplayView::changeMultiView()
		{
			if(!m_bMultiViewInitialized)
			{
				return;
			}

			CSignalChannelDisplay* l_pChannelDisplay = getChannelDisplay(m_oChannelDisplay.size()-1);

			//if there are no channels to display in the multiview
			if(m_vMultiViewSelectedChannels.empty())
			{
				//hides the multiview display (last one in the list)
				l_pChannelDisplay->resetChannelList();
				toggleChannel(m_oChannelDisplay.size() - 1, false);
			}
			//there are channels to display in the multiview
			else
			{
				if(!GTK_WIDGET_VISIBLE(GTK_WIDGET(m_pSignalDisplayTable)))
				{
					//if there were no selected channels before, but now there are, show the table again
					gtk_widget_show(GTK_WIDGET(m_pSignalDisplayTable));
				}

				if(isChannelDisplayVisible(m_oChannelDisplay.size()-1) == false)
				{
					toggleChannel(m_oChannelDisplay.size() - 1, true);
				}

				//updates channels to display list
				l_pChannelDisplay->resetChannelList();

				for(size_t i=0 ; i<m_vMultiViewSelectedChannels.size() ; i++)
				{
					l_pChannelDisplay->addChannel(m_vMultiViewSelectedChannels[i]);
				}

				//request a redraw
				gdk_window_invalidate_rect(l_pChannelDisplay->getSignalDisplayWidget()->window, NULL, false);
			}
		}

		void CSignalDisplayView::init()
		{
			OpenViBE::uint32 l_ui32ChannelCount = (uint32)m_pBufferDatabase->m_pDimmensionSizes[0];
			vector<string>& l_oChannelName = m_pBufferDatabase->m_pDimmesionLabels[0];

			stringstream l_oLabelString;
			GtkWidget * l_pChannelSelectList = glade_xml_get_widget(m_pGladeInterface, "SignalDisplayChannelSelectList");
			GtkWidget * l_pMultiViewSelectList = glade_xml_get_widget(m_pGladeInterface, "SignalDisplayMultiViewSelectList");

			//creates the array of Channel displays
			m_oChannelDisplay.resize(l_ui32ChannelCount+1);
			m_oChannelLabel.resize(l_ui32ChannelCount+1);

			//gets the pointer to the channels table
			m_pSignalDisplayTable = glade_xml_get_widget(m_pGladeInterface, "SignalDisplayMainTable");

			//resize this table according to the number of channels
			gtk_table_resize(GTK_TABLE(m_pSignalDisplayTable), l_ui32ChannelCount*2+1, 4);

			int32 l_i32LeftRulerWidthRequest = 50;
			int32 l_i32ChannelDisplayWidthRequest = 20;
			int32 l_i32BottomRulerWidthRequest = 0;

			int32 l_i32LeftRulerHeightRequest = 0;
			int32 l_i32ChannelDisplayHeightRequest = 20;
			int32 l_i32BottomRulerHeightRequest = 20;
			/*
			//resize dialogs
			int32 l_i32DialogFixedSizeRequest = 100;
			int32 l_i32DialogChannelHeightRequest = 15;
			//int32 l_i32DialogWidthRequest = 150;

			gtk_widget_set_size_request(
				glade_xml_get_widget(m_pGladeInterface, "SignalDisplayChannelSelectDialog"),
				-1,
				l_i32DialogFixedSizeRequest + l_ui32ChannelCount * l_i32DialogChannelHeightRequest);

			gtk_widget_set_size_request(
				glade_xml_get_widget(m_pGladeInterface, "SignalDisplayMultiViewDialog"),
				-1,
				l_i32DialogFixedSizeRequest + l_ui32ChannelCount * l_i32DialogChannelHeightRequest);
			*/
			//sets a minimum size for the table (needed to scroll)
			gtk_widget_set_size_request(
				m_pSignalDisplayTable,
				l_i32LeftRulerWidthRequest + l_i32ChannelDisplayHeightRequest,
				(l_ui32ChannelCount*2+1)*l_i32ChannelDisplayHeightRequest
			);

			//add a vertical separator
			GtkWidget* l_pSeparator = gtk_vseparator_new();
			gtk_table_attach(GTK_TABLE(m_pSignalDisplayTable), l_pSeparator,
				1, 2, //second column
				0, l_ui32ChannelCount*2+1, //run over the whole table height
				GTK_SHRINK, static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), 0, 0);
			gtk_widget_show(l_pSeparator);

			GtkSizeGroup* l_pSizeGroup = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

			//creates as many ChannelDisplay widget as there are channels and adds them to the table
			for(uint32 i=0 ; i<l_ui32ChannelCount ; i++)
			{
				//add channel label
				//-----------------
				if(l_oChannelName[i] == "")
				{
					//If no name has been set, just keep the channel index
					l_oLabelString<<"Channel "<<i;
				}
				else
				{	//else keep the index plus the name
					l_oLabelString<<i<<" : "<<l_oChannelName[i];
				}
				GtkWidget* l_pLabel = gtk_label_new(l_oLabelString.str().c_str());
				m_oChannelLabel[i] = l_pLabel;
				gtk_table_attach(GTK_TABLE(m_pSignalDisplayTable),l_pLabel,
					0, 1, //first column
					i*2, (i*2)+1,
					GTK_FILL, GTK_SHRINK,
					0, 0);
				gtk_widget_show(l_pLabel);
				gtk_size_group_add_widget(l_pSizeGroup, l_pLabel);

				//create channel display widget
				//-----------------------------
				m_oChannelDisplay[i] = new CSignalChannelDisplay(
					this,
					l_i32ChannelDisplayWidthRequest, l_i32ChannelDisplayHeightRequest,
					l_i32LeftRulerWidthRequest, l_i32LeftRulerHeightRequest);
				m_oChannelDisplay[i]->addChannel(i);

				gtk_table_attach(GTK_TABLE(m_pSignalDisplayTable),
					m_oChannelDisplay[i]->getRulerWidget(),
					2, 3, //third column
					i*2, (i*2)+1,
					GTK_SHRINK, static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL),
					0, 0);
				gtk_widget_show(m_oChannelDisplay[i]->getRulerWidget());
				gtk_table_attach(GTK_TABLE(m_pSignalDisplayTable),
					m_oChannelDisplay[i]->getSignalDisplayWidget(),
					3, 4, //fourth column
					i*2, (i*2)+1,
					static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL),
					0, 0);
				gtk_widget_show(m_oChannelDisplay[i]->getSignalDisplayWidget());

				//add horizontal separator
				//------------------------
				l_pSeparator = gtk_hseparator_new();
				gtk_table_attach(GTK_TABLE(m_pSignalDisplayTable), l_pSeparator,
					0, 4,
					(i*2)+1, (i*2)+2,
					static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), GTK_SHRINK,
					0, 0);
				gtk_widget_show(l_pSeparator);

				//add checkbox in channel select window
				//-------------------------------------
				GtkWidget * l_pChannelCheckButton = gtk_check_button_new_with_label(l_oLabelString.str().c_str());
				m_vChannelsCheckButtons.push_back(l_pChannelCheckButton);
				gtk_box_pack_start_defaults(GTK_BOX(l_pChannelSelectList), l_pChannelCheckButton);

				//same for the multiview dialog
				//-----------------------------
				l_pChannelCheckButton = gtk_check_button_new_with_label(l_oLabelString.str().c_str());
				m_vMultiViewChannelsCheckButtons.push_back(l_pChannelCheckButton);
				gtk_box_pack_start_defaults(GTK_BOX(l_pMultiViewSelectList), l_pChannelCheckButton);
				l_oLabelString.str("");

				//a channel is selected by default
				m_vSelectedChannels.push_back(i);
			}

			//multiview channel
			//-----------------
			//create and attach label
			GtkWidget * l_pLabel =  gtk_label_new("Multi-View");
			m_oChannelLabel[l_ui32ChannelCount] = l_pLabel;
			gtk_table_attach(GTK_TABLE(m_pSignalDisplayTable),l_pLabel,
				0, 1,
				l_ui32ChannelCount*2, (l_ui32ChannelCount*2)+1,
				GTK_FILL, GTK_SHRINK,
				0, 0);

			//create and attach display widget
			CSignalChannelDisplay* l_pChannelDisplay = new CSignalChannelDisplay(
				this,
				l_i32ChannelDisplayWidthRequest, l_i32ChannelDisplayHeightRequest,
				l_i32LeftRulerWidthRequest, l_i32LeftRulerHeightRequest);
			m_oChannelDisplay[l_ui32ChannelCount] = l_pChannelDisplay;
			l_pChannelDisplay->addChannel(0);
			gtk_table_attach(GTK_TABLE(m_pSignalDisplayTable),
				l_pChannelDisplay->getRulerWidget(),
				2, 3, //third column
				(l_ui32ChannelCount*2), (l_ui32ChannelCount*2)+1,
				GTK_SHRINK, static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL),
				0, 0);
			gtk_table_attach(GTK_TABLE(m_pSignalDisplayTable),
				l_pChannelDisplay->getSignalDisplayWidget(),
				3, 4, //fourth column
				(l_ui32ChannelCount*2), (l_ui32ChannelCount*2)+1,
				static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL), static_cast<GtkAttachOptions>(GTK_EXPAND | GTK_FILL),
				0, 0);

			m_bMultiViewInitialized = true;

			//create bottom ruler
			//-------------------
			m_pBottomRuler = new CBottomTimeRuler(*m_pBufferDatabase, l_i32BottomRulerWidthRequest, l_i32BottomRulerHeightRequest);
			gtk_size_group_add_widget(l_pSizeGroup, glade_xml_get_widget(m_pGladeInterface, "SignalDisplayEmptyLabel1"));
			gtk_box_pack_start(m_pBottomBox, m_pBottomRuler->getWidget(), false, false, 0);
			// tell ruler has to resize when channel displays are resized
			if(m_oChannelDisplay.size() != 0)
			{
				m_pBottomRuler->linkWidthToWidget(m_oChannelDisplay[0]->getSignalDisplayWidget());
			}
			gtk_widget_show_all(m_pBottomRuler->getWidget());

			//allocate memory to store sample points
			//--------------------------------------
			//reserve the maximum space needed for computing the points to display
			//(when cropping the lines, there can be up to two times the number of original points)
			m_pPoints.reserve((size_t)(m_pBufferDatabase->m_pDimmensionSizes[1]*m_pBufferDatabase->m_ui64NumberOfBufferToDisplay * 2));
			//resize the vector of raw points
			m_pRawPoints.resize((size_t)(m_pBufferDatabase->m_pDimmensionSizes[1]*m_pBufferDatabase->m_ui64NumberOfBufferToDisplay));

			//Don't display left ruler (default)
			m_bShowLeftRulers = false;
			toggleLeftRulers(m_bShowLeftRulers);
			gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayToggleLeftRulerButton")), m_bShowLeftRulers);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayToggleLeftRulerButton")),     "toggled",       G_CALLBACK(toggleLeftRulerButtonCallback),   this);

			//Display bottom ruler
			m_bShowBottomRuler = true;
			toggleBottomRuler(m_bShowBottomRuler);
			gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayToggleBottomRulerButton")), m_bShowBottomRuler);
			g_signal_connect(G_OBJECT(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayToggleBottomRulerButton")),   "toggled",       G_CALLBACK(toggleBottomRulerButtonCallback), this);

			activateToolbarButtons(true);
		}

		void CSignalDisplayView::redraw()
		{
			//nothing to redraw if the table isn't visible or no data was received
			if(m_pSignalDisplayTable == NULL || !GTK_WIDGET_VISIBLE(m_pSignalDisplayTable) || m_pBufferDatabase->isFirstBufferReceived() == false)
			{
				return;
			}

			float64 l_f64LargestDisplayedValueRange = 0;

			//update channels display parameters based on current signal data
			for(uint32 i=0 ; i<m_oChannelDisplay.size(); i++)
			{
				if(m_oChannelDisplay[i] == NULL)
				{
					return;
				}

				//FIXME : should hidden channels be taken into account when computing largest value range?
				if(GTK_WIDGET_VISIBLE(m_oChannelDisplay[i]->getSignalDisplayWidget()))
				{
					float64 l_f64ValueRange;
					m_oChannelDisplay[i]->checkTranslation(l_f64ValueRange);
					if(l_f64ValueRange > l_f64LargestDisplayedValueRange)
					{
						l_f64LargestDisplayedValueRange = l_f64ValueRange;
					}
				}
			}

			if(m_bAutoVerticalScale == true) //global best fit
			{
				//if we just switched back to auto scale, or if range increased or decreased beyond margin boundaries
				if(m_bVerticalScaleChanged == true ||
					l_f64LargestDisplayedValueRange > (m_f64LargestDisplayedValueRange + m_f64ValueRangeMargin) ||
					l_f64LargestDisplayedValueRange < (m_f64LargestDisplayedValueRange - m_f64ValueRangeMargin))
				{
					m_f64LargestDisplayedValueRange = l_f64LargestDisplayedValueRange;
					m_f64ValueRangeMargin = m_f64MarginFactor * l_f64LargestDisplayedValueRange;

					for(uint32 i=0; i<m_oChannelDisplay.size(); i++)
					{
						//set new parameters
						m_oChannelDisplay[i]->setGlobalBestFitParameters(m_f64LargestDisplayedValueRange, m_f64ValueRangeMargin);
					}
				}
			}
			else //fixed scale
			{
				//tell all channels about new fixed range if it just changed
				if(m_bVerticalScaleChanged == true)
				{
					for(uint32 i=0; i<m_oChannelDisplay.size(); i++)
					{
						//set new parameters
						float64 l_f64Margin = 0;
						m_oChannelDisplay[i]->setGlobalBestFitParameters(m_f64CustomVerticalScaleValue, l_f64Margin);
					}
				}
			}

			//if in scan mode, check whether time scale needs to be updated
			if(m_pBufferDatabase->getDisplayMode() == OVP_TypeId_SignalDisplayMode_Scan && m_ui64LeftmostDisplayedTime < m_pBufferDatabase->m_oStartTime[0])
			{
				//printf("Time basis needs to be updated\n");
				if(m_pBufferDatabase->m_oSampleBuffers.size() < m_pBufferDatabase->m_ui64NumberOfBufferToDisplay)
				{
					m_ui64LeftmostDisplayedTime = m_pBufferDatabase->m_oStartTime[0];
				}
				else //catch up with current time interval
				{
					if(m_pBufferDatabase->m_ui64TotalStep == 0)
					{
						//error
					}
					else
					{
						m_ui64LeftmostDisplayedTime += m_pBufferDatabase->m_ui64TotalStep;

						//while there is time to catch up
						while(m_ui64LeftmostDisplayedTime < (m_pBufferDatabase->m_oStartTime[0] - m_pBufferDatabase->m_ui64BufferStep))
						{
							m_ui64LeftmostDisplayedTime += m_pBufferDatabase->m_ui64TotalStep;
						}

						//round leftmost displayed time to start of closest data buffer
						for(uint32 i=0; i<m_pBufferDatabase->m_oStartTime.size(); i++)
						{
							if(m_pBufferDatabase->m_oEndTime[i] > m_ui64LeftmostDisplayedTime)
							{
								m_ui64LeftmostDisplayedTime = m_pBufferDatabase->m_oStartTime[i];
							}
						}

						//if drawing is not up to date, force a full redraw
						if(m_oChannelDisplay[0]->m_ui64LatestDisplayedTime != m_ui64LeftmostDisplayedTime)
						{
							for(size_t i=0; i<m_oChannelDisplay.size(); i++)
							{
								m_oChannelDisplay[i]->redrawAllAtNextRefresh();
							}
						}
					}
				}
			}

			//redraw channels
			for(size_t i=0 ; i<m_oChannelDisplay.size(); i++)
			{
				if(GTK_WIDGET_VISIBLE(m_oChannelDisplay[i]->getSignalDisplayWidget()))
				{
					/*
					//if in scroll mode, or if time basis changed, redraw all
					if(m_pBufferDatabase->getDisplayMode() == OVP_TypeId_SignalDisplayMode_Scroll || l_pChannelDisplay->mustRedrawAll() == true)
					{
						printf("full redraw\n");*/
						GdkRectangle l_oUpdateRect;
						m_oChannelDisplay[i]->getUpdateRectangle(l_oUpdateRect);
						gdk_window_invalidate_rect(m_oChannelDisplay[i]->getSignalDisplayWidget()->window, &l_oUpdateRect, false);
					/*}
					else
					{
						GdkRectangle l_oUpdateRect;
						m_oChannelDisplay[i]->getUpdateRectangle(l_oUpdateRect);
						//printf("partial redraw : x=%d, w=%d\n", l_oUpdateRect.x, l_oUpdateRect.width);
						gdk_window_clear_area_e(m_oChannelDisplay[i]->getSignalDisplayWidget()->window, l_oUpdateRect.x, l_oUpdateRect.y, l_oUpdateRect.width, l_oUpdateRect.height);
					}*/
				}
			}

			//redraw ruler
			m_pBottomRuler->setLeftmostDisplayedTime(m_ui64LeftmostDisplayedTime);
			gdk_window_invalidate_rect(GTK_WIDGET(m_pBottomRuler->getWidget())->window, NULL, true);

			m_bVerticalScaleChanged = false;
		}

		void CSignalDisplayView::toggleLeftRulers(boolean bActive)
		{
			m_bShowLeftRulers = bActive;

			for(size_t i=0 ; i<m_oChannelDisplay.size() ; i++)
			{
				if(isChannelDisplayVisible(i) == true)
				{
					if(bActive)
					{
						gtk_widget_show(m_oChannelDisplay[i]->getRulerWidget());
					}
					else
					{
						gtk_widget_hide(m_oChannelDisplay[i]->getRulerWidget());
					}
				}
			}
		}

		void CSignalDisplayView::toggleBottomRuler(boolean bActive)
		{
			m_bShowBottomRuler = bActive;

			if(bActive)
			{
				gtk_widget_show_all(GTK_WIDGET(m_pBottomBox));
			}
			else
			{
				gtk_widget_hide_all(GTK_WIDGET(m_pBottomBox));
			}
		}

		void CSignalDisplayView::toggleChannel(uint32 ui32ChannelIndex, boolean bActive)
		{
			CSignalChannelDisplay* l_pChannelDisplay = getChannelDisplay(ui32ChannelIndex);

			if(bActive)
			{
				gtk_widget_show( m_oChannelLabel[ui32ChannelIndex] );
				if(m_bShowLeftRulers == true)
				{
					gtk_widget_show( l_pChannelDisplay->getRulerWidget() );
				}
				gtk_widget_show( l_pChannelDisplay->getSignalDisplayWidget() );
			}
			else
			{
				gtk_widget_hide( m_oChannelLabel[ui32ChannelIndex] );
				gtk_widget_hide( l_pChannelDisplay->getRulerWidget() );
				gtk_widget_hide( l_pChannelDisplay->getSignalDisplayWidget() );
			}
		}

		void CSignalDisplayView::updateMainTableStatus()
		{
			//if nothing has been selected
			if(m_vSelectedChannels.empty() && m_vMultiViewSelectedChannels.empty())
			{
				//hide the whole table
				gtk_widget_hide(GTK_WIDGET(m_pSignalDisplayTable));
			}
			else
			{
				if(!GTK_WIDGET_VISIBLE(GTK_WIDGET(m_pSignalDisplayTable)))
				{
					//if there were no selected channels before, but now there are, show the table again
					gtk_widget_show(GTK_WIDGET(m_pSignalDisplayTable));
				}
			}
		}

		void CSignalDisplayView::activateToolbarButtons(boolean bActive)
		{
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayScrollModeButton"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayScanModeButton"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayToggleLeftRulerButton"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayToggleBottomRulerButton"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayTimeScaleItem"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayChannelSelectButton"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayMultiViewButton"), bActive);
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayInformationButton"), bActive);
		}

		boolean CSignalDisplayView::onDisplayModeToggledCB(CIdentifier oDisplayMode)
		{
			m_pBufferDatabase->setDisplayMode(oDisplayMode);

			//force full redraw of all channels when display mode changes
			for(size_t i=0 ; i<m_oChannelDisplay.size(); i++)
			{
				m_oChannelDisplay[i]->redrawAllAtNextRefresh();
			}

			//redraw channels
			redraw();

			return true;
		}

		boolean CSignalDisplayView::onVerticalScaleModeToggledCB(GtkToggleButton* pToggleButton)
		{
			m_bVerticalScaleChanged = true;
			m_bAutoVerticalScale = gtk_toggle_button_get_active(pToggleButton);
#if 0
			if(m_bAutoVerticalScale)
			{
				gtk_widget_hide(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayCustomVerticalScaleSpinButton"));
			}
			else
			{
				gtk_spin_button_set_value(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayCustomVerticalScaleSpinButton")), m_f64LargestDisplayedValueRange);
				gtk_widget_show(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayCustomVerticalScaleSpinButton"));
			}
#else
			gtk_widget_set_sensitive(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayCustomVerticalScaleSpinButton"), !m_bAutoVerticalScale);
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(glade_xml_get_widget(m_pGladeInterface, "SignalDisplayCustomVerticalScaleSpinButton")), m_f64LargestDisplayedValueRange);
#endif
			return true;
		}

		boolean CSignalDisplayView::onCustomVerticalScaleChangedCB(::GtkSpinButton *pSpinButton)
		{
			m_bVerticalScaleChanged = true;
			m_f64CustomVerticalScaleValue = gtk_spin_button_get_value(pSpinButton);
			return true;
		}

		CSignalChannelDisplay* CSignalDisplayView::getChannelDisplay(uint32 ui32ChannelIndex)
		{
			if(ui32ChannelIndex < m_oChannelDisplay.size())
			{
				return m_oChannelDisplay[ui32ChannelIndex];
			}
			else
			{
				return NULL;
			}
		}

		boolean CSignalDisplayView::isChannelDisplayVisible(uint32 ui32ChannelIndex)
		{
			return GTK_WIDGET_VISIBLE(getChannelDisplay(ui32ChannelIndex)->getSignalDisplayWidget());
		}
	}
}
