#ifndef __OpenViBEPlugins_Algorithm_ClassifierTrainerLDA_H__
#define __OpenViBEPlugins_Algorithm_ClassifierTrainerLDA_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <xml/IWriter.h>

#if defined TARGET_HAS_ThirdPartyITPP

#include <itpp/itbase.h>

#define OVP_ClassId_Algorithm_ClassifierTrainerLDA                                        OpenViBE::CIdentifier(0xD7183FC7, 0xBD74F298)
#define OVP_ClassId_Algorithm_ClassifierTrainerLDADesc                                    OpenViBE::CIdentifier(0xD42D544A, 0x7A28DDB1)

namespace OpenViBEPlugins
{
	namespace Local
	{
		class CAlgorithmClassifierTrainerLDA : public OpenViBEToolkit::CAlgorithmClassifierTrainer, public XML::IWriterCallback
		{
		public:

			virtual OpenViBE::boolean train(const OpenViBEToolkit::IFeatureVectorSet& rFeatureVectorSet);
			virtual OpenViBE::boolean saveConfiguration(OpenViBE::IMemoryBuffer& rMemoryBuffer);

			_IsDerivedFromClass_Final_(CAlgorithmClassifierTrainer, OVP_ClassId_Algorithm_ClassifierTrainerLDA);

		protected:

			virtual void write(const char* sString); // XML IWriterCallback

			OpenViBE::CMemoryBuffer m_oConfiguration;
			itpp::vec m_oCoefficients;
		};

		class CAlgorithmClassifierTrainerLDADesc : public OpenViBEToolkit::CAlgorithmClassifierTrainerDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("LDA classifier trainer - GPL"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard / Fabien Lotte"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA / INSA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Algorithm_ClassifierTrainerLDA; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Local::CAlgorithmClassifierTrainerLDA; }

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				CAlgorithmClassifierTrainerDesc::getAlgorithmPrototype(rAlgorithmPrototype);
				return true;
			}

			_IsDerivedFromClass_Final_(CAlgorithmClassifierTrainerDesc, OVP_ClassId_Algorithm_ClassifierTrainerLDADesc);
		};
	};
};

#endif // TARGET_HAS_ThirdPartyITPP

#endif // __OpenViBEPlugins_Algorithm_ClassifierTrainerLDA_H__
