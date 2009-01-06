#ifndef __OpenViBEPlugins_Algorithm_ClassifierProcessorBliffLDA_H__
#define __OpenViBEPlugins_Algorithm_ClassifierProcessorBliffLDA_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#if defined TARGET_HAS_ThirdPartyBLiFF

#define OVP_ClassId_Algorithm_ClassifierProcessorBliffLDA          OpenViBE::CIdentifier(0x12D9E1FA, 0x077932AE)
#define OVP_ClassId_Algorithm_ClassifierProcessorBliffLDADesc      OpenViBE::CIdentifier(0xFF13F5EA, 0xBBF9C01F)

namespace OpenViBEPlugins
{
	namespace Local
	{
		class CAlgorithmClassifierProcessorBliffLDA : public OpenViBEToolkit::CAlgorithmClassifierProcessor
		{
		public:

			virtual OpenViBE::boolean classify(const OpenViBEToolkit::IFeatureVector& rFeatureVector, OpenViBE::float64& rf64Class, OpenViBEToolkit::IVector& rClassificationValues);
			virtual OpenViBE::boolean loadConfiguration(const OpenViBE::IMemoryBuffer& rMemoryBuffer);

			_IsDerivedFromClass_Final_(CAlgorithmClassifierProcessor, OVP_ClassId_Algorithm_ClassifierProcessorBliffLDA);

			OpenViBE::CMemoryBuffer m_oConfiguration;
		};

		class CAlgorithmClassifierProcessorBliffLDADesc : public OpenViBEToolkit::CAlgorithmClassifierProcessorDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("BLiFF - LDA classifier processor - GPL"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard / Fabien Lotte"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA / INSA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Algorithm_ClassifierProcessorBliffLDA; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Local::CAlgorithmClassifierProcessorBliffLDA; }

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				CAlgorithmClassifierProcessorDesc::getAlgorithmPrototype(rAlgorithmPrototype);
				return true;
			}

			_IsDerivedFromClass_Final_(CAlgorithmClassifierProcessorDesc, OVP_ClassId_Algorithm_ClassifierProcessorBliffLDADesc);
		};
	};
};

#endif // TARGET_HAS_ThirdPartyBLiFF

#endif // __OpenViBEPlugins_Algorithm_ClassifierProcessorBliffLDA_H__
