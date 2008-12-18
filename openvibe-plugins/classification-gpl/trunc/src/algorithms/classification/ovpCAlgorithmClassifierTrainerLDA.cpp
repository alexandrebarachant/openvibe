#include "ovpCAlgorithmClassifierTrainerLDA.h"

#if defined TARGET_HAS_ThirdPartyBLiFF

#include <FeatureVector.h>
#include <FeatVecSet.h>
#include <LDAClassif.h>

#include <fstream>
#include <map>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Local;

using namespace OpenViBEToolkit;

using namespace std;

#define _ParameterFile_ "./CAlgorithmClassifierTrainerLDA-config.tmp"

boolean CAlgorithmClassifierTrainerLDA::train(const IFeatureVectorSet& rFeatureVectorSet)
{
	float64 l_f64LDASign=1;
	float64 l_f64LDALabel1=rFeatureVectorSet[0].getLabel();
	float64 l_f64LDALabel2=l_f64LDALabel1;
	for(uint32 i=0; i<rFeatureVectorSet.getFeatureVectorCount(); i++)
	{
		const IFeatureVector& l_rFeatureVector=rFeatureVectorSet.getFeatureVector(i);
		if(l_rFeatureVector.getLabel()!=l_f64LDALabel1)
		{
			if(l_f64LDALabel2==l_f64LDALabel1)
			{
				l_f64LDALabel2=l_rFeatureVector.getLabel();
			}
			else if(l_f64LDALabel2!=l_rFeatureVector.getLabel())
			{
				this->getLogManager() << LogLevel_ImportantWarning << "This algorithm can not handle more than 2 classes\n";
				this->getLogManager() << LogLevel_ImportantWarning << "Found at least 3 classes : " << l_f64LDALabel1 << " " << l_f64LDALabel2 << " " << l_rFeatureVector.getLabel() << "\n";
				return false;
			}
		}
	}

	if(l_f64LDALabel1==l_f64LDALabel2)
	{
		this->getLogManager() << LogLevel_ImportantWarning << "This algorithm can not handle less than 2 classes\n";
		return false;
	}

	map<float64, float64> l_vLabel;
	if(l_f64LDALabel2<l_f64LDALabel1)
	{
		l_vLabel[l_f64LDALabel1]=l_f64LDALabel2;
		l_vLabel[l_f64LDALabel2]=l_f64LDALabel1;
		l_f64LDASign=-1;
		this->getLogManager() << LogLevel_Info << "The sign of the LDA coefficients will be changed for better coherency (first class in the negative side of the hyperplane)\n";
	}
	else
	{
		l_vLabel[l_f64LDALabel2]=l_f64LDALabel1;
		l_vLabel[l_f64LDALabel1]=l_f64LDALabel2;
		l_f64LDASign=1;
	}

	bliff::FeatVecSet l_oFeatureVectorSet;
	for(uint32 i=0; i<rFeatureVectorSet.getFeatureVectorCount(); i++)
	{
		const IFeatureVector& l_rFeatureVector=rFeatureVectorSet.getFeatureVector(i);
		bliff::FeatureVector l_oFeatureVector(l_rFeatureVector.getSize(), l_vLabel[l_rFeatureVector.getLabel()]);
		for(uint32 j=0; j<l_rFeatureVector.getSize(); j++)
		{
			l_oFeatureVector[j]=l_rFeatureVector[j];
		}
		l_oFeatureVectorSet.addFeatVec(l_oFeatureVector);
	}

	bliff::CLDAClassifier l_oLDAClassifier;

	l_oLDAClassifier.kFoldTest(l_oFeatureVectorSet, 10, 10);
	this->getLogManager() << LogLevel_Info << "KFoldTest returned : " << (float64)l_oLDAClassifier.getAccuracy() << "%\n";

	l_oLDAClassifier.train(l_oFeatureVectorSet);
	vector<double> l_vCoefficients=l_oLDAClassifier.getCoefficients();
	for(size_t j=0; j<l_vCoefficients.size(); j++)
	{
		l_oLDAClassifier.setCoefficient(j, l_f64LDASign*l_vCoefficients[j]);
	}
	l_oLDAClassifier.saveParams(_ParameterFile_);

	ifstream l_oFile(_ParameterFile_, ios::binary);
	if(l_oFile.is_open())
	{
		size_t l_iFileLen;
		l_oFile.seekg(0, ios::end);
		l_iFileLen=l_oFile.tellg();
		l_oFile.seekg(0, ios::beg);
		m_oConfiguration.setSize(l_iFileLen, true);
		l_oFile.read((char*)m_oConfiguration.getDirectPointer(), l_iFileLen);
		l_oFile.close();
	}

	l_oLDAClassifier.test(l_oFeatureVectorSet);
	this->getLogManager() << LogLevel_Info << "Test returned : " << (float64)l_oLDAClassifier.getAccuracy() << "%\n";

	return true;
}

boolean CAlgorithmClassifierTrainerLDA::saveConfiguration(IMemoryBuffer& rMemoryBuffer)
{
	rMemoryBuffer.setSize(0, true);
	rMemoryBuffer.append(m_oConfiguration);
	return true;
}

#endif // TARGET_HAS_ThirdPartyBLiFF
