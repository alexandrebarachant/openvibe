#ifndef __OpenViBEPlugins_Acquisition_CStandardAcquisitionBehavior_H__
#define __OpenViBEPlugins_Acquisition_CStandardAcquisitionBehavior_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

namespace OpenViBEPlugins
{
	namespace Acquisition
	{
		class CStandardAcquisitionBehavior : virtual public OpenViBE::Plugins::IBoxBehavior
		{
		public:
			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean processEvent(
				OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
				OpenViBE::CMessageEvent& rMessageEvent);
			virtual OpenViBE::boolean processSignal(
				OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
				OpenViBE::CMessageSignal& rMessageSignal);
			virtual OpenViBE::boolean processClock(
				OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
				OpenViBE::CMessageClock& rMessageClock);
			virtual OpenViBE::boolean processInput(
				OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
				OpenViBE::uint32 ui32InputIndex);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxBehavior, OVP_ClassId_StandardAcquisitionBehavior)
		};

		class CStandardAcquisitionBehaviorDesc : virtual public OpenViBE::Plugins::IBoxBehaviorDesc
		{
		public:
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Standard acquisition behavior"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("A standard acquisition for most acquisition algorithm"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("This behavior is called on clock every $$$ ms"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Acquisition"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.1"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_StandardAcquisitionBehavior; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Acquisition::CStandardAcquisitionBehavior(); }
			virtual OpenViBE::uint32 getClockFrequency(void) const;

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxBehaviorDesc, OVP_ClassId_StandardAcquisitionBehaviorDesc)
		};
	};

};

#endif // __OpenViBEPlugins_Acquisition_CAcquisitionBehavior_H__
