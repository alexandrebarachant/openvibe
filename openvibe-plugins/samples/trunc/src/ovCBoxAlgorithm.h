#ifndef __SamplePlugin_CBoxAlgorithm_H__
#define __SamplePlugin_CBoxAlgorithm_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <iostream>

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
		virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("sample algorithm"); }
		virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
		virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
		virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("sample algorithm"); }
		virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("A sample openvibe algorithm in order to see how it should work"); }
		virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
		virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

		virtual void release(void)
		{
		}
		virtual OpenViBE::CIdentifier getCreatedClass(void) const
		{
			return ClassIdentifier_SampleBoxAlgorithm;
		}
		virtual OpenViBE::Plugins::IPluginObject* create(void)
		{
			return new OpenViBESamples::CBoxAlgorithm();
		}
		virtual OpenViBE::boolean getBoxPrototype(
			OpenViBE::Plugins::IBoxProto& rPrototype) const
		{
			rPrototype.addInput("a sample input 1", OpenViBE::CIdentifier(0,1));
			rPrototype.addInput("a sample input 2", OpenViBE::CIdentifier(1,0));
			rPrototype.addInput("a sample input 3", OpenViBE::CIdentifier(1,1));

			rPrototype.addOutput("an output 1", OpenViBE::CIdentifier(0,1));
			rPrototype.addOutput("an output 2", OpenViBE::CIdentifier(1,0));
			rPrototype.addOutput("an output 3", OpenViBE::CIdentifier(1,1));

			rPrototype.addSetting("a setting 1", OpenViBE::CIdentifier(0,1), "default value 1");
			rPrototype.addSetting("a setting 2", OpenViBE::CIdentifier(1,0), "default value 2");
			rPrototype.addSetting("a setting 3", OpenViBE::CIdentifier(1,1), "default value 3");

			return true;
		}

		_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, ClassIdentifier_SampleBoxAlgorithmDesc)
	};
};

#endif // __SamplePlugin_CBoxAlgorithm_H__
