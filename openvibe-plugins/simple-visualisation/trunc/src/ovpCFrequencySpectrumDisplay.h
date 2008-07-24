
#ifndef __OpenViBEPlugins_SimpleVisualisation_CFrequencySpectrumDisplay_H__
#define __OpenViBEPlugins_SimpleVisualisation_CFrequencySpectrumDisplay_H__
#include "ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include "ovpCBufferDatabase.h"

#include "ovpCFrequencySpectrumDisplay/ovpCFrequencySpectrumDisplayView.h"

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{

		class CFrequencySpectrumDisplay : public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>,
		virtual public OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback::ICallback
		{
		public:

			CFrequencySpectrumDisplay(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_FrequencySpectrumDisplay)

			virtual void setMatrixDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount);
			virtual void setMatrixDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize);
			virtual void setMatrixDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel);
			virtual void setMatrixBuffer(const OpenViBE::float64* pBuffer);

		public:

			//ebml
			EBML::IReader* m_pReader;
			OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback* m_pStreamedMatrixReaderCallBack;

			//The main object used for the display (contains all the GUI code)
			CSignalDisplayDrawable * m_pFrequencySpectrumDisplayView;

			//Contains all the data about the incoming signal
			CBufferDatabase * m_pFrequencySpectrumDisplayDatabase;

			//Start and end time of the last buffer
			OpenViBE::uint64 m_ui64StartTime;
			OpenViBE::uint64 m_ui64EndTime;
		};

#endif

#include <openvibe/plugins/ovIPluginObjectDesc.h>

		class CFrequencySpectrumDisplayDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Time-frequency map display"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Bruno Renier"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Time-frequency map display."); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("2D visualisation/Basic"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_FrequencySpectrumDisplay; }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-zoom-fit"); }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SimpleVisualisation::CFrequencySpectrumDisplay(); }

			virtual OpenViBE::boolean hasFunctionality(OpenViBE::Kernel::EPluginFunctionality ePF) const
			{
				return ePF == OpenViBE::Kernel::PluginFunctionality_Visualization;
			}

			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Signal", OV_TypeId_StreamedMatrix);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_FrequencySpectrumDisplayDesc)
		};
	};
};

