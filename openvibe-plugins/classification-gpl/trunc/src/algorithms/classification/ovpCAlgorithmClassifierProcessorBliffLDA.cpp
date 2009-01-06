#include "ovpCAlgorithmClassifierProcessorBliffLDA.h"

#if defined TARGET_HAS_ThirdPartyBLiFF

#include <FeatureVector.h>
#include <FeatVecSet.h>
#include <LDAClassif.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Local;

using namespace OpenViBEToolkit;

#define _ParameterFile_ "./CAlgorithmClassifierProcessorBliffLDA-config.tmp"

boolean CAlgorithmClassifierProcessorBliffLDA::classify(const IFeatureVector& rFeatureVector, float64& rf64Class, IVector& rClassificationValues)
{
	bliff::FeatureVector l_oFeatureVector(rFeatureVector.getSize(), 0);
	for(uint32 j=0; j<rFeatureVector.getSize(); j++)
	{
		l_oFeatureVector[j]=rFeatureVector[j];
	}

	FILE* l_pFile=fopen(_ParameterFile_, "wb");
	fwrite(m_oConfiguration.getDirectPointer(), m_oConfiguration.getSize(), 1, l_pFile);
	fclose(l_pFile);

	itpp::Vec<double> l_vResult;
	double l_dResult;
	bliff::CLDAClassifier l_oBliffLDAClassifier;
	l_oBliffLDAClassifier.readParams(_ParameterFile_);
	l_vResult=l_oBliffLDAClassifier.classify(l_oFeatureVector);
	l_dResult=l_oBliffLDAClassifier.assign(l_oFeatureVector);

	rf64Class=l_dResult;
	rClassificationValues.setSize(l_vResult.size());
	for(size_t i=0; i<rClassificationValues.getSize(); i++)
	{
		rClassificationValues[i]=l_vResult[i];
	}

	return true;
}

boolean CAlgorithmClassifierProcessorBliffLDA::loadConfiguration(const IMemoryBuffer& rMemoryBuffer)
{
	m_oConfiguration.setSize(0, true);
	m_oConfiguration.append(rMemoryBuffer);
	return true;
}

#endif // TARGET_HAS_ThirdPartyBLiFF
