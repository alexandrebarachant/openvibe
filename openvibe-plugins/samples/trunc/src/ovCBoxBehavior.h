#ifndef __SamplePlugin_CBoxBehavior_H__
#define __SamplePlugin_CBoxBehavior_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

#define ClassIdentifier_SampleBoxBehaviorDesc OpenViBE::CIdentifier(0x5B2B5C67, 0x78EA7539)
#define ClassIdentifier_SampleBoxBehavior     OpenViBE::CIdentifier(0x38B3E172, 0x5D1A7FDE)

namespace OpenViBESamples
{
	class CBoxBehavior : virtual public OpenViBE::Plugins::IBoxBehavior
	{
	public:
		virtual void release(void)
		{
			delete this;
		}
		virtual OpenViBE::boolean processEvent(
			OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
			OpenViBE::CMessageEvent& rMessageEvent)
		{
			return false;
		}
		virtual OpenViBE::boolean processSignal(
			OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
			OpenViBE::CMessageSignal& rMessageSignal)
		{
			return false;
		}
		virtual OpenViBE::boolean processClock(
			OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
			OpenViBE::CMessageClock& rMessageClock)
		{
			return false;
		}
		virtual OpenViBE::boolean processInput(
			OpenViBE::Plugins::IBoxBehaviorContext& rBoxBehaviorContext,
			OpenViBE::uint32 ui32InputIndex)
		{
			return false;
		}

		_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxBehavior, ClassIdentifier_SampleBoxBehavior)
	};

	class CBoxBehaviorDesc : virtual public OpenViBE::Plugins::IBoxBehaviorDesc
	{
	public:
		virtual OpenViBE::CString getName(void)                { return OpenViBE::CString("sample behavior"); }
		virtual OpenViBE::CString getAuthorName(void)          { return OpenViBE::CString("Yann Renard"); }
		virtual OpenViBE::CString getAuthorCompanyName(void)   { return OpenViBE::CString("INRIA/IRISA"); }
		virtual OpenViBE::CString getShortDescription(void)    { return OpenViBE::CString("sample behavior"); }
		virtual OpenViBE::CString getDetailedDescription(void) { return OpenViBE::CString("A sample openvibe behavior in order to see how it should work"); }
		virtual OpenViBE::CString getCategory(void)            { return OpenViBE::CString("Samples"); }
		virtual OpenViBE::CString getVersion(void)             { return OpenViBE::CString("1.0"); }

		virtual void release(void)
		{
		}
		virtual OpenViBE::CIdentifier getCreatedClass(void)
		{
			return ClassIdentifier_SampleBoxBehavior;
		}
		virtual OpenViBE::Plugins::IPluginObject* create(void)
		{
			return new OpenViBESamples::CBoxBehavior();
		}
		virtual OpenViBE::uint32 getClockFrequency(void)
		{
			return 0;
		}
		_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxBehaviorDesc, ClassIdentifier_SampleBoxBehaviorDesc)
	};

};

#endif // __SamplePlugin_CBoxBehavior_H__
