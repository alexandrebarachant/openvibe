#include "ovpCLDABoxAlgorithm.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessingGpl;
using namespace std;

boolean CLDABoxAlgorithm::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	m_ui64NbEntries = getStaticBoxContext().getInputCount();

	m_vStreamDecoder.resize(getStaticBoxContext().getInputCount());
	m_vStreamDecoderEndTime.resize(getStaticBoxContext().getInputCount());
	for(uint32 i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		m_vStreamDecoder[i]=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamDecoder));
		m_vStreamDecoder[i]->initialize();
		m_vStreamDecoderEndTime[i] = 0;
	}
	m_pStreamEncoder=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamEncoder));
	m_pStreamEncoder->initialize();

	// Extract features
	m_pFeatureExtractionLda=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_FeatureExtractionLda));
	m_pFeatureExtractionLda->initialize();
	// Train Fisher Lda
	m_pComputeFisherLdaFunction=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_ComputeFisherLdaFunction));
	m_pComputeFisherLdaFunction->initialize();
	// Compute Fisher Lda
	m_pApplyFisherLdaFunction=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_ApplyFisherLdaFunction));
	m_pApplyFisherLdaFunction->initialize();

	// compute lda settings
	CString l_oNbTrainingSamples;
	CString l_oParameters;

	getStaticBoxContext().getSettingValue(0, l_oNbTrainingSamples);
	getStaticBoxContext().getSettingValue(1, l_oParameters);

	m_uint64NbTrainingSamples=atoi(l_oNbTrainingSamples);

	istringstream l_oStreamString((const char*)l_oParameters);
	uint64 l_uint64CurrentValue;
	while(l_oStreamString>>l_uint64CurrentValue)
	{
		m_oCoefficients.push_back(l_uint64CurrentValue);
	}
	IMatrix* l_pClass1SamplesNumbersInputMatrix = &m_oClass1SamplesNumbersInputMatrix;
	l_pClass1SamplesNumbersInputMatrix->setDimensionCount(2);
	l_pClass1SamplesNumbersInputMatrix->setDimensionSize(0,1);
	l_pClass1SamplesNumbersInputMatrix->setDimensionSize(1,m_uint64NbTrainingSamples);
	float64* l_pClass1SamplesNumbersInput = l_pClass1SamplesNumbersInputMatrix->getBuffer();
	for (uint32 i = 0; i<m_oCoefficients.size(); i++)
	{
		l_pClass1SamplesNumbersInput[i] = m_oCoefficients[i];
	}

	m_pFeatureExtractionLda->getInputParameter(OVP_Algorithm_FeatureExtractionLda_InputParameterId_Class1SamplesNumber)->setValue(&l_pClass1SamplesNumbersInputMatrix);

	m_pComputeFisherLdaFunction->getInputParameter(OVP_Algorithm_ComputeFisherLdaFunction_InputParameterId_MatrixFirstClass)->setReferenceTarget(m_pFeatureExtractionLda->getOutputParameter(OVP_Algorithm_FeatureExtractionLda_OutputParameterId_MatrixFirstClass));
	m_pComputeFisherLdaFunction->getInputParameter(OVP_Algorithm_ComputeFisherLdaFunction_InputParameterId_MatrixSecondClass)->setReferenceTarget(m_pFeatureExtractionLda->getOutputParameter(OVP_Algorithm_FeatureExtractionLda_OutputParameterId_MatrixSecondClass));

	m_pApplyFisherLdaFunction->getInputParameter(OVP_Algorithm_ApplyFisherLdaFunction_InputParameterId_MatrixGlobalMean)->setReferenceTarget(m_pComputeFisherLdaFunction->getOutputParameter(OVP_Algorithm_ComputeFisherLdaFunction_OutputParameterId_MatrixGlobalMean));
	m_pApplyFisherLdaFunction->getInputParameter(OVP_Algorithm_ApplyFisherLdaFunction_InputParameterId_MatrixGlobalCovariance)->setReferenceTarget(m_pComputeFisherLdaFunction->getOutputParameter(OVP_Algorithm_ComputeFisherLdaFunction_OutputParameterId_MatrixGlobalCovariance));
	m_pApplyFisherLdaFunction->getInputParameter(OVP_Algorithm_ApplyFisherLdaFunction_InputParameterId_MatrixGlobalProbability)->setReferenceTarget(m_pComputeFisherLdaFunction->getOutputParameter(OVP_Algorithm_ComputeFisherLdaFunction_OutputParameterId_MatrixGlobalProbability));

	m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputParameterId_Matrix)->setReferenceTarget(m_pApplyFisherLdaFunction->getOutputParameter(OVP_Algorithm_ApplyFisherLdaFunction_OutputParameterId_FisherLdaDecision));

	m_ui64LastStartTime=0;
	m_ui64LastEndTime=0;

	m_ui64TrainingIndex = 0;
	m_ui64NbEntriesDecoded = 0;
	m_bHasSentHeader=false;
	m_bFirstTime = true;
	m_bFirstTimeComputeLDA = true;

	return true;
}

boolean CLDABoxAlgorithm::uninitialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	m_pStreamEncoder->uninitialize();
	getAlgorithmManager().releaseAlgorithm(*m_pStreamEncoder);
	for(uint32 i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		m_vStreamDecoder[i]->uninitialize();
		getAlgorithmManager().releaseAlgorithm(*m_vStreamDecoder[i]);
	}
	m_vStreamDecoder.clear();

	m_pFeatureExtractionLda->uninitialize();
	m_pComputeFisherLdaFunction->uninitialize();
	m_pApplyFisherLdaFunction->uninitialize();

	getAlgorithmManager().releaseAlgorithm(*m_pFeatureExtractionLda);
	getAlgorithmManager().releaseAlgorithm(*m_pComputeFisherLdaFunction);
	getAlgorithmManager().releaseAlgorithm(*m_pApplyFisherLdaFunction);

	return true;
}

boolean CLDABoxAlgorithm::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CLDABoxAlgorithm::process(void)
{
	IBoxIO& l_rDynamicBoxContext=getDynamicBoxContext();
	IBox& l_rStaticBoxContext=getStaticBoxContext();

	TParameterHandler < IMemoryBuffer* > l_oOutputMemoryBufferHandle(m_pStreamEncoder->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_OutputParameterId_EncodedMemoryBuffer));

	uint64 l_ui64ReadChunkMinEndTime=0xffffffffffffffffll;
	for(uint32 i=0; i<l_rStaticBoxContext.getInputCount(); i++)
	{
		for(uint32 j=0; j<l_rDynamicBoxContext.getInputChunkCount(i); j++)
		{
			TParameterHandler < const IMemoryBuffer* > l_ipMemoryBuffer(m_vStreamDecoder[i]->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_InputParameterId_MemoryBufferToDecode));
			l_ipMemoryBuffer=l_rDynamicBoxContext.getInputChunk(i, j);
			m_vStreamDecoder[i]->process();

			if(m_vStreamDecoder[i]->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedHeader))
			{
				m_pFeatureExtractionLda->process(OVP_Algorithm_FeatureExtractionLda_InputTriggerId_Initialize);
			}

			if(m_vStreamDecoder[i]->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedBuffer))
			{
				m_oSignalIntputMatrixHandle = m_vStreamDecoder[i]->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputParameterId_Matrix);
				IMatrix* l_pSignalInputMatrix = m_oSignalIntputMatrixHandle;
				float64* l_pSignalInput = l_pSignalInputMatrix->getBuffer();
				// dimensions of the input buffer of signal
				uint32 l_ui32SignalInputColumnDimensionSize=m_oSignalIntputMatrixHandle->getDimensionSize(1);

				if(m_bFirstTime)
				{
					m_pEpochTable = new CMatrix[m_ui64NbEntries*l_ui32SignalInputColumnDimensionSize];
					m_pEpochTable->setDimensionCount(2);
					m_pEpochTable->setDimensionSize(0,m_ui64NbEntries);
					m_pEpochTable->setDimensionSize(1,l_ui32SignalInputColumnDimensionSize);
					m_pf64EpochTable = m_pEpochTable->getBuffer();
					m_bFirstTime = false;
				}
				for (uint32 k=0; k< l_ui32SignalInputColumnDimensionSize; k++)
				{
					m_pf64EpochTable[k+i*l_ui32SignalInputColumnDimensionSize] = l_pSignalInput[k];
				}

				m_ui64NbEntriesDecoded++;

			}

			if(m_vStreamDecoder[i]->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedEnd))
			{
			}

			m_vStreamDecoderEndTime[i]=l_rDynamicBoxContext.getInputChunkEndTime(i, j);
			l_rDynamicBoxContext.markInputAsDeprecated(i, j);
		}

		if(l_ui64ReadChunkMinEndTime > m_vStreamDecoderEndTime[i])
		{
			l_ui64ReadChunkMinEndTime=m_vStreamDecoderEndTime[i];
		}

	}

	if (m_ui64NbEntriesDecoded == m_ui64NbEntries)
	{

		if (m_ui64TrainingIndex < m_uint64NbTrainingSamples)
		{
			this->getLogManager() << LogLevel_Debug << "m_ui64TrainingIndex = " <<m_ui64TrainingIndex+1 << " / " << m_uint64NbTrainingSamples << "\n";
			m_pFeatureExtractionLda->getInputParameter(OVP_Algorithm_FeatureExtractionLda_InputParameterId_EpochTable)->setValue(&m_pEpochTable);
			m_pFeatureExtractionLda->process(OVP_Algorithm_FeatureExtractionLda_InputTriggerId_ExtractFeature);

			m_ui64TrainingIndex++;
		}
		else
		{
			if (m_bFirstTimeComputeLDA)
			{
				m_pComputeFisherLdaFunction->process(OVP_Algorithm_ComputeFisherLdaFunction_InputTriggerId_ComputeFunction);
				m_bFirstTimeComputeLDA = false;
			}

			m_pApplyFisherLdaFunction->getInputParameter(OVP_Algorithm_ApplyFisherLdaFunction_InputParameterId_MatrixSignal)->setValue(&m_pEpochTable);
			m_pApplyFisherLdaFunction->process(OVP_Algorithm_ApplyFisherLdaFunction_InputTriggerId_ApplyFunction);

			if(!m_bHasSentHeader)
			{
				l_oOutputMemoryBufferHandle=l_rDynamicBoxContext.getOutputChunk(0);
				m_pStreamEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeHeader);
				l_rDynamicBoxContext.markOutputAsReadyToSend(0, m_ui64LastEndTime, m_ui64LastEndTime);
				m_bHasSentHeader=true;
			}

			l_oOutputMemoryBufferHandle=l_rDynamicBoxContext.getOutputChunk(0);
			m_pStreamEncoder->process(OVP_GD_Algorithm_StreamedMatrixStreamEncoder_InputTriggerId_EncodeBuffer);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, m_ui64LastStartTime, l_ui64ReadChunkMinEndTime);
			m_ui64LastStartTime=m_ui64LastEndTime;
			m_ui64LastEndTime=l_ui64ReadChunkMinEndTime;
		}
		m_ui64NbEntriesDecoded=0;
	}

	return true;
}
