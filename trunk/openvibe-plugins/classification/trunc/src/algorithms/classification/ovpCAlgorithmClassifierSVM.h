#ifndef __OpenViBEPlugins_Algorithm_ClassifierSVM_H__
#define __OpenViBEPlugins_Algorithm_ClassifierSVM_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <xml/IWriter.h>
#include <xml/IReader.h>

#include <stack>
#include "libSVM/svm.h"

#define OVP_ClassId_Algorithm_ClassifierSVM                                        OpenViBE::CIdentifier(0x50486EC2, 0x6F2417FC)
#define OVP_ClassId_Algorithm_ClassifierSVMDesc                                    OpenViBE::CIdentifier(0x272B056E, 0x0C6502AC)

namespace OpenViBEPlugins
{
	namespace Classification
	{
		class CAlgorithmClassifierSVM : public OpenViBEToolkit::CAlgorithmClassifier, public XML::IWriterCallback, public XML::IReaderCallback
		{
		public:

			CAlgorithmClassifierSVM(void);

			virtual OpenViBE::boolean train(const OpenViBEToolkit::IFeatureVectorSet& rFeatureVectorSet);
			virtual OpenViBE::boolean classify(const OpenViBEToolkit::IFeatureVector& rFeatureVector, OpenViBE::float64& rf64Class, OpenViBEToolkit::IVector& rClassificationValues);

			virtual OpenViBE::boolean saveConfiguration(OpenViBE::IMemoryBuffer& rMemoryBuffer);
			virtual OpenViBE::boolean loadConfiguration(const OpenViBE::IMemoryBuffer& rMemoryBuffer);
			virtual OpenViBE::CString paramToString(svm_parameter *pParam);
			virtual OpenViBE::CString modelToString();
			virtual OpenViBE::CString problemToString(svm_problem *pProb);
			_IsDerivedFromClass_Final_(CAlgorithmClassifier, OVP_ClassId_Algorithm_ClassifierSVM);

		protected:

			virtual void write(const char* sString); // XML IWriterCallback

			virtual void openChild(const char* sName, const char** sAttributeName, const char** sAttributeValue, XML::uint64 ui64AttributeCount); // XML IReaderCallback
			virtual void processChildData(const char* sData); // XML IReaderCallback
			virtual void closeChild(void); // XML ReaderCallback
			std::stack<OpenViBE::CString> m_vNode;

			std::vector <OpenViBE::float64> m_vClass;

			//struct svm_parameter *m_oParam; // set by parse_command_line
			//struct svm_problem m_oProb;     // set by read_problem
			struct svm_model *m_pModel;
			OpenViBE::int32 m_i32IndexSV;
			OpenViBE::uint32 m_ui32NumberOfFeatures;
			OpenViBE::CMemoryBuffer m_oConfiguration;
			//todo à modifier en fonction de svn_save_model
			//itpp::vec m_oCoefficients;
		};

		class CAlgorithmClassifierSVMDesc : public OpenViBEToolkit::CAlgorithmClassifierDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("SVM classifier"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Laurent Bougrain / Baptiste Payan"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("UHP_Nancy1/LORIA INRIA/LORIA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Algorithm_ClassifierSVM; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Classification::CAlgorithmClassifierSVM; }

			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const
			{
				CAlgorithmClassifierDesc::getAlgorithmPrototype(rAlgorithmPrototype);
				return true;
			}

			_IsDerivedFromClass_Final_(CAlgorithmClassifierDesc, OVP_ClassId_Algorithm_ClassifierSVMDesc);
		};
	};
};

#endif // __OpenViBEPlugins_Algorithm_ClassifierSVM_H__
