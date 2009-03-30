#include "ovpCBoxAlgorithmClassifierTrainer.h"

#include <system/Memory.h>

#include <fstream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Classification;
using namespace std;

boolean CBoxAlgorithmClassifierTrainer::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	CIdentifier l_oClassifierTrainerAlgorithmClassIdentifier;
	CString l_sClassifierTrainerAlgorithmClassIdentifier;
	l_rStaticBoxContext.getSettingValue(0, l_sClassifierTrainerAlgorithmClassIdentifier);
	l_oClassifierTrainerAlgorithmClassIdentifier=this->getTypeManager().getEnumerationEntryValueFromName(OVTK_TypeId_ClassificationTrainerAlgorithm, l_sClassifierTrainerAlgorithmClassIdentifier);

	if(l_oClassifierTrainerAlgorithmClassIdentifier==OV_UndefinedIdentifier)
	{
		this->getLogManager() << LogLevel_ImportantWarning << "Unknown classifier trainer algorithm [" << l_sClassifierTrainerAlgorithmClassIdentifier << "]\n";
		return false;
	}

	CString l_sConfigurationFilename;
	l_rStaticBoxContext.getSettingValue(1, l_sConfigurationFilename);

	CString l_sStimulationName;
	l_rStaticBoxContext.getSettingValue(2, l_sStimulationName);
	m_ui64TrainStimulation=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, l_sStimulationName);

	for(uint32 i=3; i<l_rStaticBoxContext.getSettingCount(); i++)
	{
		CString l_sStimulationName;
		l_rStaticBoxContext.getSettingValue(i, l_sStimulationName);
		m_vClass[this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, l_sStimulationName)]=i-2;
	}

	m_pFeaturesDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_FeatureVectorStreamDecoder));
	m_pLabelsDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pClassifierTrainer=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(l_oClassifierTrainerAlgorithmClassIdentifier));

	m_pFeaturesDecoder->initialize();
	m_pLabelsDecoder->initialize();
	m_pClassifierTrainer->initialize();

	m_ui64StimulationEndTime=0;

	return true;
}

boolean CBoxAlgorithmClassifierTrainer::uninitialize(void)
{
	m_pClassifierTrainer->uninitialize();
	m_pLabelsDecoder->uninitialize();
	m_pFeaturesDecoder->uninitialize();

	this->getAlgorithmManager().releaseAlgorithm(*m_pClassifierTrainer);
	this->getAlgorithmManager().releaseAlgorithm(*m_pLabelsDecoder);
	this->getAlgorithmManager().releaseAlgorithm(*m_pFeaturesDecoder);

	m_oStimulationSet.setStimulationCount(0);

	return true;
}

boolean CBoxAlgorithmClassifierTrainer::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmClassifierTrainer::process(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	uint32 i;
	boolean l_bTrainStimulationReceived=false;

	// Parses stimulations
	for(i=0; i<l_rDynamicBoxContext.getInputChunkCount(1); i++)
	{
		TParameterHandler < const IMemoryBuffer* > ip_pLabelsMemoryBuffer(m_pLabelsDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
		TParameterHandler < const IStimulationSet* > op_pStimulationSet(m_pLabelsDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
		ip_pLabelsMemoryBuffer=l_rDynamicBoxContext.getInputChunk(1, i);
		m_pLabelsDecoder->process();
		if(m_pLabelsDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
			m_oStimulationSet.setStimulationCount(0);
		}
		if(m_pLabelsDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			for(uint64 j=0; j<op_pStimulationSet->getStimulationCount(); j++)
			{
				m_oStimulationSet.appendStimulation(
					op_pStimulationSet->getStimulationIdentifier(j),
					op_pStimulationSet->getStimulationDate(j),
					op_pStimulationSet->getStimulationDuration(j));

				if(op_pStimulationSet->getStimulationIdentifier(j)==m_ui64TrainStimulation)
				{
					l_bTrainStimulationReceived=true;
				}
			}
		}
		if(m_pLabelsDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
		}
		l_rDynamicBoxContext.markInputAsDeprecated(1, i);
	}

	// Parses feature vectors
	for(i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		TParameterHandler < const IMemoryBuffer* > ip_pFeatureVectorMemoryBuffer(m_pFeaturesDecoder->getInputParameter(OVP_GD_Algorithm_FeatureVectorStreamDecoder_InputParameterId_MemoryBufferToDecode));
		TParameterHandler < const IMatrix* > op_pFeatureVectorMatrix(m_pFeaturesDecoder->getOutputParameter(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputParameterId_Matrix));
		ip_pFeatureVectorMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
		m_pFeaturesDecoder->process();
		if(m_pFeaturesDecoder->isOutputTriggerActive(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
			m_vFeatureVector.clear();
		}
		if(m_pFeaturesDecoder->isOutputTriggerActive(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			CBoxAlgorithmClassifierTrainer::SFeatureVector l_oFeatureVector;
			l_oFeatureVector.m_pFeatureVectorMatrix=new CMatrix();
			l_oFeatureVector.m_ui64StartTime=l_rDynamicBoxContext.getInputChunkStartTime(0, i);
			l_oFeatureVector.m_ui64EndTime=l_rDynamicBoxContext.getInputChunkEndTime(0, i);
			OpenViBEToolkit::Tools::Matrix::copy(*l_oFeatureVector.m_pFeatureVectorMatrix, *op_pFeatureVectorMatrix);
			m_vFeatureVector.push_back(l_oFeatureVector);
		}
		if(m_pFeaturesDecoder->isOutputTriggerActive(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
		}
		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	// On train stimulation reception, build up the labelled feature vector set matrix and go on training
	if(l_bTrainStimulationReceived)
	{
		if(m_vFeatureVector.size()==0)
		{
			this->getLogManager() << LogLevel_Warning << "Received train stimulation but no feature vector\n";
		}
		else
		{
			this->getLogManager() << LogLevel_Info << "Received train stimulation\n";

			uint32 l_ui32FeatureVectorCount=m_vFeatureVector.size();
			uint32 l_ui32FeatureVectorSize=m_vFeatureVector[0].m_pFeatureVectorMatrix->getBufferElementCount();

			TParameterHandler < IMatrix* > ip_pFeatureVectorSet(m_pClassifierTrainer->getInputParameter(OVTK_Algorithm_ClassifierTrainer_InputParameterId_FeatureVectorSet));
			ip_pFeatureVectorSet->setDimensionCount(2);
			ip_pFeatureVectorSet->setDimensionSize(0, l_ui32FeatureVectorCount);
			ip_pFeatureVectorSet->setDimensionSize(1, l_ui32FeatureVectorSize+1);

			float64* l_pFeatureVectorSetBuffer=ip_pFeatureVectorSet->getBuffer();
			for(size_t j=0; j<m_vFeatureVector.size(); j++)
			{
				System::Memory::copy(
					l_pFeatureVectorSetBuffer,
					m_vFeatureVector[j].m_pFeatureVectorMatrix->getBuffer(),
					l_ui32FeatureVectorSize*sizeof(float64));

				float64 l_f64Class=0;
				for(uint64 k=0; k<m_oStimulationSet.getStimulationCount(); k++)
				{
					if(m_oStimulationSet.getStimulationDate(k)<=m_vFeatureVector[j].m_ui64StartTime)
					{
						map < uint64, float64 >::iterator itClass=m_vClass.find(m_oStimulationSet.getStimulationIdentifier(k));
						if(itClass!=m_vClass.end())
						{
							l_f64Class=itClass->second;
						}
					}
				}

				l_pFeatureVectorSetBuffer[l_ui32FeatureVectorSize]=l_f64Class;

				this->getLogManager() << LogLevel_Trace << "Feature vector with class " << l_f64Class << " at time (" << m_vFeatureVector[j].m_ui64StartTime << ":" << m_vFeatureVector[j].m_ui64EndTime << ")\n";

				l_pFeatureVectorSetBuffer+=(l_ui32FeatureVectorSize+1);
			}

			this->getLogManager() << LogLevel_Trace << "Labelled feature vector built\n";

			m_pClassifierTrainer->process(OVTK_Algorithm_ClassifierTrainer_InputTriggerId_Train);

			this->getLogManager() << LogLevel_Info << "Training finished\n";

			m_pClassifierTrainer->process(OVTK_Algorithm_ClassifierTrainer_InputTriggerId_SaveConfiguration);

			this->getLogManager() << LogLevel_Trace << "Parameters saving finished\n";

			CString l_sConfigurationFilename;
			l_rStaticBoxContext.getSettingValue(1, l_sConfigurationFilename);

			TParameterHandler < const IMemoryBuffer* > op_pClassificationTrainerConfiguration(m_pClassifierTrainer->getOutputParameter(OVTK_Algorithm_ClassifierTrainer_OutputParameterId_Configuration));
			const IMemoryBuffer* l_pConfiguration=op_pClassificationTrainerConfiguration;
			ofstream l_oFile(l_sConfigurationFilename.toASCIIString(), ios::binary);
			if(l_oFile.is_open())
			{
				l_oFile.write((char*)l_pConfiguration->getDirectPointer(), (std::streamsize)l_pConfiguration->getSize());
				l_oFile.close();
			}
			else
			{
				this->getLogManager() << LogLevel_Warning << "Could not save configuration to file [" << l_sConfigurationFilename << "]\n";
			}
		}
	}

	return true;
}
