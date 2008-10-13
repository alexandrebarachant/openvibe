#include "ovpCDownsamplingBoxAlgorithm.h"

#include <stdlib.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SignalProcessingGpl;
using namespace std;

boolean CDownsamplingBoxAlgorithm::initialize(void)
{
	CIdentifier l_oInputTypeIdentifier;
	getStaticBoxContext().getInputType(0, l_oInputTypeIdentifier);
	if(l_oInputTypeIdentifier==OV_TypeId_Signal)
	{
		m_pStreamDecoder=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_SignalDecoder));
		m_pStreamEncoder=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_SignalEncoder));
	}
	else
	{
		return false;
	}
	m_pStreamDecoder->initialize();
	m_pStreamEncoder->initialize();

	// Compute filter coeff algorithm
	m_pComputeTemporalFilterCoefficients=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_ComputeTemporalFilterCoefficients));
	m_pComputeTemporalFilterCoefficients->initialize();

	// Apply filter to signal input buffer
	m_pApplyTemporalFilter=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_ApplyTemporalFilter));
	m_pApplyTemporalFilter->initialize();

	// Compute Downsampling of signal input buffer
	m_pDownsampling=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_Downsampling));
	m_pDownsampling->initialize();

	// compute filter coefs settings
	CString l_oNewSamplingRate;
	CString l_oNameFilter;
	CString l_oFilterOrder;
	CString l_oPassBandRipple;
	CString l_oRatio;

	getStaticBoxContext().getSettingValue(0, l_oNewSamplingRate);
	getStaticBoxContext().getSettingValue(1, l_oRatio);
	getStaticBoxContext().getSettingValue(2, l_oNameFilter);
	getStaticBoxContext().getSettingValue(3, l_oFilterOrder);
	getStaticBoxContext().getSettingValue(4, l_oPassBandRipple);

	m_ui64NewSamplingRate = atoi(l_oNewSamplingRate);

	float64 l_f64Ratio = 1.0/4;
	if(this->getTypeManager().getEnumerationEntryValueFromName(OVP_TypeId_FrequencyCutOffRatio, l_oRatio)==OVP_TypeId_FrequencyCutOffRatio_14)
	{
		l_f64Ratio=1.0/4;
	}
	if(this->getTypeManager().getEnumerationEntryValueFromName(OVP_TypeId_FrequencyCutOffRatio, l_oRatio)==OVP_TypeId_FrequencyCutOffRatio_13)
	{
		l_f64Ratio=1.0/3;
	}
	if(this->getTypeManager().getEnumerationEntryValueFromName(OVP_TypeId_FrequencyCutOffRatio, l_oRatio)==OVP_TypeId_FrequencyCutOffRatio_12)
	{
		l_f64Ratio=1.0/2;
	}

	uint64 l_iu64NameFilter=this->getTypeManager().getEnumerationEntryValueFromName(OVP_TypeId_FilterMethod, l_oRatio);

	uint64 l_iu64KindFilter=1; //Low Pass
	uint64 l_ui64FilterOrder=atoi(l_oFilterOrder);
	float64 l_f64LowCutFrequency = 0;
	float64 l_f64HighCutFrequency = (float64)m_ui64NewSamplingRate*l_f64Ratio;
	float64 l_f64PassBandRipple = atof(l_oPassBandRipple);

	// compute filter settings
	m_pComputeTemporalFilterCoefficients->getInputParameter(OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_SamplingFrequency)->setReferenceTarget(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));
	m_pComputeTemporalFilterCoefficients->getInputParameter(OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_FilterMethod)->setValue(&l_iu64NameFilter);
	m_pComputeTemporalFilterCoefficients->getInputParameter(OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_FilterType)->setValue(&l_iu64KindFilter);
	m_pComputeTemporalFilterCoefficients->getInputParameter(OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_FilterOrder)->setValue(&l_ui64FilterOrder);
	m_pComputeTemporalFilterCoefficients->getInputParameter(OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_LowCutFrequency)->setValue(&l_f64LowCutFrequency);
	m_pComputeTemporalFilterCoefficients->getInputParameter(OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_HighCutFrequency)->setValue(&l_f64HighCutFrequency);
	m_pComputeTemporalFilterCoefficients->getInputParameter(OVP_Algorithm_ComputeTemporalFilterCoefficients_InputParameterId_BandPassRipple)->setValue(&l_f64PassBandRipple);

	// apply filter settings
	m_pApplyTemporalFilter->getInputParameter(OVP_Algorithm_ApplyTemporalFilter_InputParameterId_SignalMatrix)->setReferenceTarget(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	m_pApplyTemporalFilter->getInputParameter(OVP_Algorithm_ApplyTemporalFilter_InputParameterId_FilterCoefficientsMatrix)->setReferenceTarget(m_pComputeTemporalFilterCoefficients->getOutputParameter(OVP_Algorithm_ComputeTemporalFilterCoefficients_OutputParameterId_Matrix));

	// Downsampling settings
	m_pDownsampling->getInputParameter(OVP_Algorithm_Downsampling_InputParameterId_SignalMatrix)->setReferenceTarget(m_pApplyTemporalFilter->getOutputParameter(OVP_Algorithm_ApplyTemporalFilter_OutputParameterId_FilteredSignalMatrix));
	m_pDownsampling->getInputParameter(OVP_Algorithm_Downsampling_InputParameterId_SamplingFrequency)->setReferenceTarget(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));
	m_pDownsampling->getInputParameter(OVP_Algorithm_Downsampling_InputParameterId_NewSamplingFrequency)->setValue(&m_ui64NewSamplingRate);

	// encoder settings
	m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_SamplingRate)->setValue(&m_ui64NewSamplingRate);

	m_pInputSignal.initialize(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_Matrix));
	m_pOutputSignal.initialize(m_pStreamEncoder->getInputParameter(OVP_GD_Algorithm_SignalStreamEncoder_InputParameterId_Matrix));
	m_ui64SamplingRate.initialize(m_pStreamDecoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamDecoder_OutputParameterId_SamplingRate));

	m_pOutputSignalDescription=new CMatrix();

	m_ui64LastStartTime=0;
	m_ui64LastEndTime=0;
	m_bFlagFirstTime = true;

	return true;
}

boolean CDownsamplingBoxAlgorithm::uninitialize(void)
{
	delete m_pOutputSignalDescription;
	m_pOutputSignalDescription=NULL;

	m_pApplyTemporalFilter->uninitialize();
	m_pComputeTemporalFilterCoefficients->uninitialize();
	m_pStreamEncoder->uninitialize();
	m_pStreamDecoder->uninitialize();
	m_pDownsampling->uninitialize();

	getAlgorithmManager().releaseAlgorithm(*m_pApplyTemporalFilter);
	getAlgorithmManager().releaseAlgorithm(*m_pComputeTemporalFilterCoefficients);
	getAlgorithmManager().releaseAlgorithm(*m_pStreamEncoder);
	getAlgorithmManager().releaseAlgorithm(*m_pStreamDecoder);
	getAlgorithmManager().releaseAlgorithm(*m_pDownsampling);

	return true;
}

boolean CDownsamplingBoxAlgorithm::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CDownsamplingBoxAlgorithm::process(void)
{
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();

	for(uint32 j=0; j<l_rDynamicBoxContext.getInputChunkCount(0); j++)
	{
		TParameterHandler < const IMemoryBuffer* > l_oInputMemoryBufferHandle(m_pStreamDecoder->getInputParameter(OVP_GD_Algorithm_SignalStreamDecoder_InputParameterId_MemoryBufferToDecode));
		TParameterHandler < IMemoryBuffer* > l_oOutputMemoryBufferHandle(m_pStreamEncoder->getOutputParameter(OVP_GD_Algorithm_SignalStreamEncoder_OutputParameterId_EncodedMemoryBuffer));
		l_oInputMemoryBufferHandle=l_rDynamicBoxContext.getInputChunk(0, j);
		l_oOutputMemoryBufferHandle=l_rDynamicBoxContext.getOutputChunk(0);
		uint64 l_ui64EndTime=m_ui64LastStartTime+l_rDynamicBoxContext.getInputChunkEndTime(0, j)-l_rDynamicBoxContext.getInputChunkStartTime(0, j);

		m_pStreamDecoder->process();
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedHeader))
		{
		}
		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedBuffer))
		{
			if (m_bFlagFirstTime)
			{
				m_pComputeTemporalFilterCoefficients->process(OVP_Algorithm_ComputeTemporalFilterCoefficients_InputTriggerId_Initialize);
				m_pComputeTemporalFilterCoefficients->process(OVP_Algorithm_ComputeTemporalFilterCoefficients_InputTriggerId_ComputeCoefficients);
				m_pApplyTemporalFilter->process(OVP_Algorithm_ApplyTemporalFilter_InputTriggerId_Initialize);
			}

			if (m_ui64LastEndTime==l_rDynamicBoxContext.getInputChunkStartTime(0, j))
			{
				m_pApplyTemporalFilter->process(OVP_Algorithm_ApplyTemporalFilter_InputTriggerId_ApplyFilterWithHistoric);
			}
			else
			{
				m_pApplyTemporalFilter->process(OVP_Algorithm_ApplyTemporalFilter_InputTriggerId_ApplyFilter);
			}

			m_pDownsampling->process(OVP_Algorithm_Downsampling_InputTriggerId_Resample);

			if (m_bFlagFirstTime)
			{
				m_pOutputSignalDescription->setDimensionCount(2);
				m_pOutputSignalDescription->setDimensionSize(0, m_pInputSignal->getDimensionSize(0));
				m_pOutputSignalDescription->setDimensionSize(1, (uint64)((float64)m_pInputSignal->getDimensionSize(1)/m_ui64SamplingRate*m_ui64NewSamplingRate));
				for(uint32 k=0; k< m_pInputSignal->getDimensionSize(0); k++)
				{
					m_pOutputSignalDescription->setDimensionLabel(0, k, m_pInputSignal->getDimensionLabel(0, k));
				}
				m_pOutputSignal.setReferenceTarget(m_pOutputSignalDescription);
				m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeHeader);
				l_rDynamicBoxContext.markOutputAsReadyToSend(0, m_ui64LastStartTime, m_ui64LastStartTime);
				m_bFlagFirstTime = false;
			}
			m_pOutputSignal.setReferenceTarget(m_pDownsampling->getOutputParameter(OVP_Algorithm_Downsampling_OutputParameterId_SignalMatrix));
			m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeBuffer);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, m_ui64LastStartTime, l_ui64EndTime);
		}

		if(m_pStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_SignalStreamDecoder_OutputTriggerId_ReceivedEnd))
		{
			m_pStreamEncoder->process(OVP_GD_Algorithm_SignalStreamEncoder_InputTriggerId_EncodeEnd);
			l_rDynamicBoxContext.markOutputAsReadyToSend(0, m_ui64LastStartTime, l_ui64EndTime);
		}

		m_ui64LastStartTime+=l_rDynamicBoxContext.getInputChunkEndTime(0, j)-l_rDynamicBoxContext.getInputChunkStartTime(0, j);
		m_ui64LastEndTime+=l_rDynamicBoxContext.getInputChunkEndTime(0, j)-l_rDynamicBoxContext.getInputChunkStartTime(0, j);
		l_rDynamicBoxContext.markInputAsDeprecated(0, j);
	}

	return true;
}
