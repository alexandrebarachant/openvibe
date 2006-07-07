#ifndef __SamplePlugin_CBoxBehavior_H__
#define __SamplePlugin_CBoxBehavior_H__

#include "plugins/ovIBoxBehaviorDesc.h"
#include "plugins/ovIBoxBehavior.h"

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
			OpenViBE::Kernel::IBoxBehaviorContext& rBoxBehaviorContext,
			OpenViBE::CMessageEvent& rMessageEvent)
		{
			return false;
		}
		virtual OpenViBE::boolean processSignal(
			OpenViBE::Kernel::IBoxBehaviorContext& rBoxBehaviorContext,
			OpenViBE::CMessageSignal& rMessageSignal)
		{
			return false;
		}
		virtual OpenViBE::boolean processClock(
			OpenViBE::Kernel::IBoxBehaviorContext& rBoxBehaviorContext,
			OpenViBE::CMessageClock& rMessageClock)
		{
			return false;
		}
		virtual OpenViBE::boolean processInput(
			OpenViBE::Kernel::IBoxBehaviorContext& rBoxBehaviorContext,
			OpenViBE::uint32 ui32InputIndex)
		{
			return false;
		}

		_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxBehavior, ClassIdentifier_SampleBoxBehavior)
	};

	class CBoxBehaviorDesc : virtual public OpenViBE::Plugins::IBoxBehaviorDesc
	{
	public:
		virtual std::string getName(void)                { return std::string("sample behavior"); }
		virtual std::string getAuthorName(void)          { return std::string("Yann Renard"); }
		virtual std::string getAuthorCompanyName(void)   { return std::string("INRIA/IRISA"); }
		virtual std::string getShortDescription(void)    { return std::string("sample behavior"); }
		virtual std::string getDetailedDescription(void) { return std::string("A sample openvibe behavior in order to see how it should work"); }
		virtual std::string getCategory(void)            { return std::string("Samples"); }
		virtual std::string getVersion(void)             { return std::string("1.0"); }

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
