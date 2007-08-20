#ifndef __OpenViBEPlugins_SimpleVisualisation_CSignalDisplay_H__
#define __OpenViBEPlugins_SimpleVisualisation_CSignalDisplay_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>

#include <vector>
#include <string>

#include "ovpCBufferDatabase.h"
#include "ovpCSignalDisplay/ovpCSignalDisplayView.h"

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{

		/**
		* This plugin opens a new GTK window and displays the incoming signals. The user may change the zoom level,
		* the width of the time window displayed, ...
		*/
		class CSignalDisplay : virtual public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>,
		virtual public OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback::ICallback
		{
		public:

			CSignalDisplay(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize();
			virtual OpenViBE::boolean uninitialize();
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process();
			
			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_SignalDisplay)
			
			virtual void setMatrixDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount);
			virtual void setMatrixDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize);
			virtual void setMatrixDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel);
			virtual void setMatrixBuffer(const OpenViBE::float64* pBuffer);

		public:
			
			
			//ebml 
			EBML::IReader* m_pReader;
			OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback* m_pStreamedMatrixReaderCallBack;
			
			//The main object used for the display (contains all the GUI code)
			CSignalDisplayDrawable * m_pSignalDisplayView;

			//Contains all the data about the incoming signal
			CBufferDatabase * m_pBufferDatabase;

			//Start and end time of the last buffer
			OpenViBE::uint64 m_ui64StartTime;
			OpenViBE::uint64 m_ui64EndTime;
		};

		/**
		* Plugin's description
		*/
		class CSignalDisplayDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Signal display"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Bruno Renier"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Displays the incoming signal"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Displays the incoming signal"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Simple visualisation"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_SignalDisplay; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SimpleVisualisation::CSignalDisplay(); }

			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Plugins::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Signal", OV_TypeId_StreamedMatrix);

				return true;
			}

			
			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_SignalDisplayDesc)

			
		};


	};
};

#endif
