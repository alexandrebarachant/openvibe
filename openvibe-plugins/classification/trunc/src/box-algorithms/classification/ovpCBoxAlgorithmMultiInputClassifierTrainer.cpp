#include "ovpCBoxAlgorithmMultiInputClassifierTrainer.h"

#include <system/Memory.h>

#include <fstream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Classification;
using namespace std;

boolean CBoxAlgorithmMultiInputClassifierTrainer::initialize(void)
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

	for(uint32 i=1; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		m_vFeatureVectorsDecoder[i-1]=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_FeatureVectorStreamDecoder));
		m_vFeatureVectorsDecoder[i-1]->initialize();
	}

	m_pStimulationsDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pClassifierTrainer=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(l_oClassifierTrainerAlgorithmClassIdentifier));

	m_pStimulationsDecoder->initialize();
	m_pClassifierTrainer->initialize();

	return true;
}

boolean CBoxAlgorithmMultiInputClassifierTrainer::uninitialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	// IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	m_pClassifierTrainer->uninitialize();
	m_pStimulationsDecoder->uninitialize();

	this->getAlgorithmManager().releaseAlgorithm(*m_pClassifierTrainer);
	this->getAlgorithmManager().releaseAlgorithm(*m_pStimulationsDecoder);

	for(uint32 i=1; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		m_vFeatureVectorsDecoder[i-1]->uninitialize();
		this->getAlgorithmManager().releaseAlgorithm(*m_vFeatureVectorsDecoder[i-1]);
	}
	m_vFeatureVectorsDecoder.clear();

	return true;
}

boolean CBoxAlgorithmMultiInputClassifierTrainer::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmMultiInputClassifierTrainer::process(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	uint32 i, j;
	boolean l_bTrainStimulationReceived=false;

	// Parses stimulations
	for(i=0; i<l_rDynamicBoxContext.getInputChunkCount(0); i++)
	{
		TParameterHandler < const IMemoryBuffer* > ip_pMemoryBuffer(m_pStimulationsDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
		TParameterHandler < const IStimulationSet* > op_pStimulationSet(m_pStimulationsDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
		ip_pMemoryBuffer=l_rDynamicBoxContext.getInputChunk(0, i);
		m_pStimulationsDecoder->process();
		if(m_pStimulationsDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
		}
		if(m_pStimulationsDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			for(uint64 j=0; j<op_pStimulationSet->getStimulationCount(); j++)
			{
				if(op_pStimulationSet->getStimulationIdentifier(j)==m_ui64TrainStimulation)
				{
					l_bTrainStimulationReceived=true;
				}
			}
		}
		if(m_pStimulationsDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
		}
		l_rDynamicBoxContext.markInputAsDeprecated(0, i);
	}

	// Parses feature vectors
	for(i=1; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		for(j=0; j<l_rDynamicBoxContext.getInputChunkCount(i); j++)
		{
			TParameterHandler < const IMemoryBuffer* > ip_pFeatureVectorMemoryBuffer(m_vFeatureVectorsDecoder[i-1]->getInputParameter(OVP_GD_Algorithm_FeatureVectorStreamDecoder_InputParameterId_MemoryBufferToDecode));
			TParameterHandler < const IMatrix* > op_pFeatureVectorMatrix(m_vFeatureVectorsDecoder[i-1]->getOutputParameter(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputParameterId_Matrix));
			ip_pFeatureVectorMemoryBuffer=l_rDynamicBoxContext.getInputChunk(i, j);
			m_vFeatureVectorsDecoder[i-1]->process();
			if(m_vFeatureVectorsDecoder[i-1]->isOutputTriggerActive(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputTriggerId_ReceivedHeader))
			{
			}
			if(m_vFeatureVectorsDecoder[i-1]->isOutputTriggerActive(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputTriggerId_ReceivedBuffer))
			{
				CBoxAlgorithmMultiInputClassifierTrainer::SFeatureVector l_oFeatureVector;
				l_oFeatureVector.m_pFeatureVectorMatrix=new CMatrix();
				l_oFeatureVector.m_ui64StartTime=l_rDynamicBoxContext.getInputChunkStartTime(i, j);
				l_oFeatureVector.m_ui64EndTime=l_rDynamicBoxContext.getInputChunkEndTime(i, j);
				l_oFeatureVector.m_ui32InputIndex=i;
				OpenViBEToolkit::Tools::Matrix::copy(*l_oFeatureVector.m_pFeatureVectorMatrix, *op_pFeatureVectorMatrix);
				m_vFeatureVector.push_back(l_oFeatureVector);
			}
			if(m_vFeatureVectorsDecoder[i-1]->isOutputTriggerActive(OVP_GD_Algorithm_FeatureVectorStreamDecoder_OutputTriggerId_ReceivedEnd))
			{
			}
			l_rDynamicBoxContext.markInputAsDeprecated(i, j);
		}
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
				float64 l_f64Class=(float64)m_vFeatureVector[j].m_ui32InputIndex;

				System::Memory::copy(
					l_pFeatureVectorSetBuffer,
					m_vFeatureVector[j].m_pFeatureVectorMatrix->getBuffer(),
					l_ui32FeatureVectorSize*sizeof(float64));

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
