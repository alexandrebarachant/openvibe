#ifndef __OpenViBEPlugins_Algorithm_ClassifierTrainerBliffLDA_H__
#define __OpenViBEPlugins_Algorithm_ClassifierTrainerBliffLDA_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#if defined TARGET_HAS_ThirdPartyBLiFF

#define OVP_ClassId_Algorithm_ClassifierTrainerBliffLDA                                        OpenViBE::CIdentifier(0xC200B3CC, 0x19E64CBD)
#define OVP_ClassId_Algorithm_ClassifierTrainerBliffLDADesc                                    OpenViBE::CIdentifier(0xBA291D66, 0x2CF290F7)

namespace OpenViBEPlugins
{
	namespace Local
	{
		class CAlgorithmClassifierTrainerBliffLDA : public OpenViBEToolkit::CAlgorithmClassifierTrainer
		{
		public:

			virtual OpenViBE::boolean train(const OpenViBEToolkit::IFeatureVectorSet& rFeatureVectorSet);
			virtual OpenViBE::boolean saveConfiguration(OpenViBE::IMemoryBuffer& rMemoryBuffer);

			_IsDerivedFromClass_Final_(CAlgorithmClassifierTrainer, OVP_ClassId_Algorithm_ClassifierTrainerBliffLDA);

			OpenViBE::CMemoryBuffer m_oConfiguration;
		};

		class CAlgorithmClassifierTrainerBliffLDADesc : public OpenViBEToolkit::CAlgorithmClassifierTrainerDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("BLiFF LDA classifier trainer - GPL"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard / Fabien Lotte"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA / INSA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Algorithm_ClassifierTrainerBliffLDA; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Local::CAlgorithmClassifierTrainerBliffLDA; }

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				CAlgorithmClassifierTrainerDesc::getAlgorithmPrototype(rAlgorithmPrototype);
				return true;
			}

			_IsDerivedFromClass_Final_(CAlgorithmClassifierTrainerDesc, OVP_ClassId_Algorithm_ClassifierTrainerBliffLDADesc);
		};
	};
};

#endif // TARGET_HAS_ThirdPartyBLiFF

#endif // __OpenViBEPlugins_Algorithm_ClassifierTrainerBliffLDA_H__
