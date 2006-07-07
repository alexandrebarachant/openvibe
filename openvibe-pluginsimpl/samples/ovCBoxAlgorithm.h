#ifndef __SamplePlugin_CBoxAlgorithm_H__
#define __SamplePlugin_CBoxAlgorithm_H__

#include "plugins/ovIBoxAlgorithmDesc.h"
#include "plugins/ovIBoxAlgorithm.h"

#define ClassIdentifier_SampleBoxAlgorithmDesc OpenViBE::CIdentifier(0x191F4A6F, 0x07EB1BF2)
#define ClassIdentifier_SampleBoxAlgorithm     OpenViBE::CIdentifier(0x271A6EEC, 0x7B1D847E)

namespace OpenViBESamples
{
	class CBoxAlgorithm : virtual public OpenViBE::Plugins::IBoxAlgorithm
	{
	public:
		virtual void release(void)
		{
			delete this;
		}
		virtual OpenViBE::boolean process(OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext)
		{
			return false;
		}

		_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, ClassIdentifier_SampleBoxAlgorithm)
	};

	class CBoxAlgorithmDesc : virtual public OpenViBE::Plugins::IBoxAlgorithmDesc
	{
	public:
		virtual std::string getName(void)                { return std::string("sample algorithm"); }
		virtual std::string getAuthorName(void)          { return std::string("Yann Renard"); }
		virtual std::string getAuthorCompanyName(void)   { return std::string("INRIA/IRISA"); }
		virtual std::string getShortDescription(void)    { return std::string("sample algorithm"); }
		virtual std::string getDetailedDescription(void) { return std::string("A sample openvibe algorithm in order to see how it should work"); }
		virtual std::string getCategory(void)            { return std::string("Samples"); }
		virtual std::string getVersion(void)             { return std::string("1.0"); }
		virtual void release(void)
		{
		}
		virtual OpenViBE::CIdentifier getCreatedClass(void)
		{
			return ClassIdentifier_SampleBoxAlgorithm;
		}
		virtual OpenViBE::Plugins::IPluginObject* create(void)
		{
			return new OpenViBESamples::CBoxAlgorithm();
		}
		virtual OpenViBE::boolean getBoxPrototype(
			OpenViBE::Plugins::IBoxProto& rPrototype)
		{
			return false;
		}

		_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, ClassIdentifier_SampleBoxAlgorithmDesc)
	};
};

#endif // __SamplePlugin_CBoxAlgorithm_H__
