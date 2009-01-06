#ifndef __OpenViBEPlugins_Algorithm_ClassifierProcessorLDA_H__
#define __OpenViBEPlugins_Algorithm_ClassifierProcessorLDA_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <xml/IReader.h>

#include <stack>

#if defined TARGET_HAS_ThirdPartyITPP

#include <itpp/itbase.h>

#define OVP_ClassId_Algorithm_ClassifierProcessorLDA          OpenViBE::CIdentifier(0x11784AB1, 0xEA05F77C)
#define OVP_ClassId_Algorithm_ClassifierProcessorLDADesc      OpenViBE::CIdentifier(0xB2FA0286, 0x87A54AC3)

namespace OpenViBEPlugins
{
	namespace Local
	{
		class CAlgorithmClassifierProcessorLDA : public OpenViBEToolkit::CAlgorithmClassifierProcessor, public XML::IReaderCallback
		{
		public:

			virtual OpenViBE::boolean classify(const OpenViBEToolkit::IFeatureVector& rFeatureVector, OpenViBE::float64& rf64Class, OpenViBEToolkit::IVector& rClassificationValues);
			virtual OpenViBE::boolean loadConfiguration(const OpenViBE::IMemoryBuffer& rMemoryBuffer);

			_IsDerivedFromClass_Final_(CAlgorithmClassifierProcessor, OVP_ClassId_Algorithm_ClassifierProcessorLDA);

		protected:

			virtual void openChild(const char* sName, const char** sAttributeName, const char** sAttributeValue, XML::uint64 ui64AttributeCount); // XML IReaderCallback
			virtual void processChildData(const char* sData); // XML IReaderCallback
			virtual void closeChild(void); // XML ReaderCallback

			std::stack<OpenViBE::CString> m_vNode;

			OpenViBE::float64 m_f64Class1;
			OpenViBE::float64 m_f64Class2;
			itpp::vec m_oCoefficients;
		};

		class CAlgorithmClassifierProcessorLDADesc : public OpenViBEToolkit::CAlgorithmClassifierProcessorDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("LDA classifier processor - GPL"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard / Fabien Lotte"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA / INSA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Algorithm_ClassifierProcessorLDA; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Local::CAlgorithmClassifierProcessorLDA; }

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				CAlgorithmClassifierProcessorDesc::getAlgorithmPrototype(rAlgorithmPrototype);
				return true;
			}

			_IsDerivedFromClass_Final_(CAlgorithmClassifierProcessorDesc, OVP_ClassId_Algorithm_ClassifierProcessorLDADesc);
		};
	};
};

#endif // TARGET_HAS_ThirdPartyITPP

#endif // __OpenViBEPlugins_Algorithm_ClassifierProcessorLDA_H__
