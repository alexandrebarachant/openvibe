#include "ovpCKeyboardStimulator.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Stimulation;

using namespace OpenViBEToolkit;

using namespace std;

namespace OpenViBEPlugins
{
	namespace Stimulation
	{
		// Called when a key is pressed on the keyboard
		gboolean KeyboardStimulator_KeyPressedCallback(GtkWidget *widget, GdkEventKey *event, gpointer data)
		{
			reinterpret_cast<CKeyboardStimulator*>(data)->processKey(event->keyval);
			return true;
		}
		
		// Called when the keyboard isn't grabbed anymore by our application
		gboolean KeyboardStimulator_GrabBrokenCallback(GtkWidget *widget, GdkEventGrabBroken *event, gpointer data)
		{
			//Try to grab the keyboard again
			if(gdk_keyboard_grab(event->window, false, GDK_CURRENT_TIME) != GDK_GRAB_SUCCESS)
			{
				cout<<"Lost the grab on the keyboard!"<<endl
				       <<"Won't be able to send keyboard generated stimulations from now on!!"<<endl;
			}

			return true;
		}

		/**
		 * Called when a key has been pressed.
		 * \param uiKey The gdk value to the pressed key.
		 * */
		void CKeyboardStimulator::processKey(guint uiKey)
		{
			//if there is one entry, adds the stimulation to the list of stims to be sent
			if(m_oKeyToStimulation.count(uiKey) != 0)
			{
				m_oStimulationToSend.push_back(m_oKeyToStimulation[uiKey]);
			}
		}

		/**
		 * Parse the configuration file and creates the Key/Stimulation associations.
		 * \param pFilename The name of the configuration file.
		 * \return True if the file was correctly parsed.
		 * */
		boolean CKeyboardStimulator::parseConfigurationFile(const char * pFilename)
		{
			ifstream l_oFile;
			l_oFile.open(pFilename);

			if(!l_oFile)
			{
				return false;
			}

			string l_oKeyName;
			string l_oStimulation;
			uint64 l_ui64Stimulation;

			//reads all the couples key name/stim
			while(!l_oFile.eof() && !l_oFile.fail())
			{
				l_oFile>>l_oKeyName>>l_oStimulation;

				sscanf(l_oStimulation.c_str(), "0x%08Lx", &l_ui64Stimulation);

				m_oKeyToStimulation[gdk_keyval_from_name(l_oKeyName.c_str())] = l_ui64Stimulation;
			}

			l_oFile.close();

			return true;
		}	

		void CKeyboardStimulator::writeStimulationOutput(const void* pBuffer, const EBML::uint64 ui64BufferSize)
		{
			appendOutputChunkData<0>(pBuffer, ui64BufferSize);
		}


		CKeyboardStimulator::CKeyboardStimulator(void) :
			m_pWriter(NULL),
			m_pOutputWriterCallbackProxy(NULL),
			m_pStimulationOutputWriterHelper(NULL),
			m_pDummyWidget(NULL),
			m_ui64PreviousActivationTime(0)
		{	
		}

		OpenViBE::boolean CKeyboardStimulator::initialize()
		{
			const IStaticBoxContext* l_pBoxContext=getBoxAlgorithmContext()->getStaticBoxContext();
			OpenViBE::CString l_sFileName;

			// Parses box settings to find input file's name
			l_pBoxContext->getSettingValue(0, l_sFileName);
			
			if(!parseConfigurationFile((const char*)l_sFileName))
			{
				cout<<"Problem while parsing configuration file!"<<endl;
				return false;
			}

			//EBML writer, ...
			m_pOutputWriterCallbackProxy = new EBML::TWriterCallbackProxy1<OpenViBEPlugins::Stimulation::CKeyboardStimulator>(*this, &CKeyboardStimulator::writeStimulationOutput);

			m_pWriter=EBML::createWriter(*m_pOutputWriterCallbackProxy);

			m_pStimulationOutputWriterHelper=createBoxAlgorithmStimulationOutputWriter();
		
			//dummy widget for grabbing events
			m_pDummyWidget = gtk_invisible_new();

			//redirect key pressed event
			g_signal_connect(G_OBJECT(m_pDummyWidget), "key-press-event", 
					G_CALLBACK(KeyboardStimulator_KeyPressedCallback), this);

			//redirect grab lost event
			g_signal_connect(G_OBJECT(m_pDummyWidget), "grab-broken-event", 
					G_CALLBACK(KeyboardStimulator_GrabBrokenCallback), this);

			gtk_widget_show(m_pDummyWidget);

			//Grab keyboard
			if(gdk_keyboard_grab(m_pDummyWidget->window, false, GDK_CURRENT_TIME) != GDK_GRAB_SUCCESS)
			{
				cout<<"Couldn't grab keyboard!"<<endl;
				return false;
			}

			//write stimulation stream header		
			m_pStimulationOutputWriterHelper->writeHeader(*m_pWriter);
			getBoxAlgorithmContext()->getDynamicBoxContext()->markOutputAsReadyToSend(0, 0, 0);

			return true;
		}

		OpenViBE::boolean CKeyboardStimulator::uninitialize()
		{
			delete m_pOutputWriterCallbackProxy;
			m_pOutputWriterCallbackProxy= NULL;
			
			if(m_pWriter)
			{
				m_pWriter->release();
				m_pWriter = NULL;
			}

			if(m_pStimulationOutputWriterHelper)
			{
				releaseBoxAlgorithmStimulationOutputWriter(m_pStimulationOutputWriterHelper);
				m_pStimulationOutputWriterHelper=NULL;
			}

			//ungrab the keyboard
			gdk_keyboard_ungrab(GDK_CURRENT_TIME);
			
			return true;
		}
			
		OpenViBE::boolean CKeyboardStimulator::processClock(OpenViBE::CMessageClock &rMessageClock)
		{
			uint64 l_ui64CurrentTime = rMessageClock.getTime();

			IDynamicBoxContext * l_pDynamicBoxContext = getBoxAlgorithmContext()->getDynamicBoxContext();

			//if there are stimulations to send
			if(!m_oStimulationToSend.empty())
			{
				//writes them
				m_pStimulationOutputWriterHelper->setStimulationCount(m_oStimulationToSend.size());

				for(size_t i=0 ; i<m_oStimulationToSend.size() ; i++)
				{
					m_pStimulationOutputWriterHelper->setStimulation(i, m_oStimulationToSend[i], l_ui64CurrentTime);
				}

				m_oStimulationToSend.clear();

				m_pStimulationOutputWriterHelper->writeBuffer(*m_pWriter);
				l_pDynamicBoxContext->markOutputAsReadyToSend(0, m_ui64PreviousActivationTime, l_ui64CurrentTime);
			}

			m_ui64PreviousActivationTime = l_ui64CurrentTime;

			getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
			
			return true;
		}

		OpenViBE::boolean CKeyboardStimulator::process()
		{

			return true;
		}

	};

};

