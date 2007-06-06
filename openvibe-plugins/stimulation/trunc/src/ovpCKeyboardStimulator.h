#ifndef __OpenViBEPlugins_Stimulation_CKeyboardStimulator_H__
#define __OpenViBEPlugins_Stimulation_CKeyboardStimulator_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include <ebml/IWriter.h>
#include <ebml/TWriterCallbackProxy.h>

#include <vector>
#include <map>

namespace OpenViBEPlugins
{
	namespace Stimulation
	{

		class CKeyboardStimulator : virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{
		public:

			CKeyboardStimulator(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize();
			virtual OpenViBE::boolean uninitialize();
		
			virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock &rMessageClock);

			virtual OpenViBE::boolean process();

			virtual void writeStimulationOutput(const void* pBuffer, const EBML::uint64 ui64BufferSize);

			OpenViBE::boolean parseConfigurationFile(const char * pFilename);

			virtual void processKey(guint uiKey);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_KeyboardStimulator)

		public:
				//EBML writing stuff
				EBML::IWriter* m_pWriter;
				EBML::TWriterCallbackProxy1<OpenViBEPlugins::Stimulation::CKeyboardStimulator> * m_pOutputWriterCallbackProxy;
				OpenViBEToolkit::IBoxAlgorithmStimulationOutputWriter * m_pStimulationOutputWriterHelper;

				//! Invisible widget used to handle keypresses				
				GtkWidget * m_pDummyWidget;

				//! Stores keyvalue/stimulation couples
				std::map<guint, OpenViBE::uint64> m_oKeyToStimulation;
		
				//! Vector of the stimulations to send when possible
				std::vector<OpenViBE::uint64> m_oStimulationToSend;

				//! Plugin's previous activation date
				OpenViBE::uint64 m_ui64PreviousActivationTime;
		};

		/**
		* Plugin's description
		*/
		class CKeyboardStimulatorDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Keyboard stimulator"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Bruno Renier"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Stimulation generator"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Sends stimulations according to key presses"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Stimulation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_KeyboardStimulator; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Stimulation::CKeyboardStimulator(); }

			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addOutput("Outgoing Stimulations", OV_TypeId_Stimulations);

				rPrototype.addSetting("Filename", OV_TypeId_String, "");

				return true;
			}

			
			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_KeyboardStimulatorDesc)

			
		};


	};
};

#endif

