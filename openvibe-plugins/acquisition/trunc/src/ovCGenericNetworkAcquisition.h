#ifndef __OpenViBEPlugins_Acquisition_CGenericNetworkAcquisition_H__
#define __OpenViBEPlugins_Acquisition_CGenericNetworkAcquisition_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

namespace OpenViBEPlugins
{
	namespace Acquisition
	{
		class CGenericNetworkAcquisition : virtual public OpenViBE::Plugins::IBoxAlgorithm
		{
		public:
			virtual void release(void) { delete this; }
			virtual OpenViBE::boolean processClock(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext,
				OpenViBE::CMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(
				OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_GenericNetworkAcquisition)
		};

		class CGenericNetworkAcquisitionDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Generic network acquisition"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("A generic network based acquisition algorithm"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This algorithm waits for EEG data from the network and distributes it into the scenario"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Acquisition"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_GenericNetworkAcquisition; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Acquisition::CGenericNetworkAcquisition(); }
			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Plugins::IBoxProto& rPrototype) const;
			virtual OpenViBE::uint32 getClockFrequency(void) const;

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_GenericNetworkAcquisitionDesc)
		};
	};
};

#endif // __OpenViBEPlugins_Acquisition_CGenericNetworkAcquisition_H__
