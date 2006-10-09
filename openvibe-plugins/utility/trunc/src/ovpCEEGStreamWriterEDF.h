#ifndef __OpenViBEPlugins_Acquisition_CEEGStreamWriterEDF_H__
#define __OpenViBEPlugins_Acquisition_CEEGStreamWriterEDF_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

namespace OpenViBEPlugins
{
	namespace Utility
	{
		class CEEGStreamWriterEDF : virtual public OpenViBE::Plugins::IBoxAlgorithm
		{
		public:
			virtual void release(void) { delete this; }
			virtual OpenViBE::boolean processInput(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_EEGStreamWriterEDF)
		};

		class CEEGStreamWriterEDFDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("EEG stream writer - EDF"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("This algorithm records on disk what comes from a specific output"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This algorithm dumps on disk a stream from a specific output in the standard EDF file format"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Utility"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_EEGStreamWriterEDF; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Utility::CEEGStreamWriterEDF(); }
			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Plugins::IBoxProto& rPrototype) const;
			virtual OpenViBE::uint32 getClockFrequency(void) const;

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_EEGStreamWriterEDFDesc)
		};
	};
};

#endif // __OpenViBEPlugins_Acquisition_CEEGStreamWriterEDF_H__
