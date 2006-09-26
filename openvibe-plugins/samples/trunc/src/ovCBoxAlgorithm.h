#ifndef __SamplePlugin_CBoxAlgorithm_H__
#define __SamplePlugin_CBoxAlgorithm_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>

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
		virtual OpenViBE::CString getName(void)                { return OpenViBE::CString("sample algorithm"); }
		virtual OpenViBE::CString getAuthorName(void)          { return OpenViBE::CString("Yann Renard"); }
		virtual OpenViBE::CString getAuthorCompanyName(void)   { return OpenViBE::CString("INRIA/IRISA"); }
		virtual OpenViBE::CString getShortDescription(void)    { return OpenViBE::CString("sample algorithm"); }
		virtual OpenViBE::CString getDetailedDescription(void) { return OpenViBE::CString("A sample openvibe algorithm in order to see how it should work"); }
		virtual OpenViBE::CString getCategory(void)            { return OpenViBE::CString("Samples"); }
		virtual OpenViBE::CString getVersion(void)             { return OpenViBE::CString("1.0"); }

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
