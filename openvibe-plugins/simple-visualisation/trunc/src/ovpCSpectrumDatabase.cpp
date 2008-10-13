#include <system/Memory.h>
#include <algorithm>
#include <math.h>

#include "ovpCSpectrumDatabase.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBE::Kernel;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

CSpectrumDatabase::CSpectrumDatabase(OpenViBEToolkit::TBoxAlgorithm<Plugins::IBoxAlgorithm>& oPlugin) :
	m_oParentPlugin(oPlugin),
	m_pProxy(NULL),
	m_pDrawable(NULL),
	m_bRedrawOnNewData(true),
	m_bFirstBufferReceived(false),
	m_ui32SamplingFrequency(0),
	m_ui64MaxDisplayedBufferCount(1),
	m_f64TimeScale(10/*10000*/) // 10 seconds
	//m_bError(false)
{
	m_pProxy = &m_oParentPlugin.getAlgorithmManager().getAlgorithm(
		m_oParentPlugin.getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_SpectrumStreamDecoder));

	m_pProxy->initialize();
}

CSpectrumDatabase::~CSpectrumDatabase()
{
	m_pProxy->uninitialize();
	m_oParentPlugin.getAlgorithmManager().releaseAlgorithm(*m_pProxy);

	while(m_oStreamedMatrices.size() >0)
	{
		delete m_oStreamedMatrices.front();
		m_oStreamedMatrices.pop_front();
	}
}

void CSpectrumDatabase::setDrawable(IStreamDisplayDrawable* pDrawable)
{
	m_pDrawable = pDrawable;
}

void CSpectrumDatabase::setRedrawOnNewData(boolean bRedrawOnNewData)
{
	m_bRedrawOnNewData = bRedrawOnNewData;
}

boolean CSpectrumDatabase::isFirstBufferReceived()
{
	return m_bFirstBufferReceived;
}

boolean CSpectrumDatabase::setTimeScale(float64 f64TimeScale)
{
	if(f64TimeScale <= 0)
	{
		return false;
	}

	//update time scale
	m_f64TimeScale = f64TimeScale;

	uint32 l_ui32SampleCountPerBuffer = getSampleCountPerBuffer();

	//return if first buffer has not been received yet
	if(l_ui32SampleCountPerBuffer == 0)
	{
		return false;
	}

	boolean l_bMaxDisplayedBufferCountChanged = false;
	uint64 l_ui64MaxDisplayedBufferCount =  static_cast<uint64>(ceil( (m_f64TimeScale*m_ui32SamplingFrequency) / getSampleCountPerBuffer()));

	//display at least one buffer
	if(l_ui64MaxDisplayedBufferCount == 0)
	{
		l_ui64MaxDisplayedBufferCount = 1;
	}

	if(l_ui64MaxDisplayedBufferCount != m_ui64MaxDisplayedBufferCount)
	{
		m_ui64MaxDisplayedBufferCount = l_ui64MaxDisplayedBufferCount;
		l_bMaxDisplayedBufferCountChanged = true;

		//if new number of buffers is smaller than before, destroy extra buffers
		while(m_oStreamedMatrices.size() > m_ui64MaxDisplayedBufferCount)
		{
			delete m_oStreamedMatrices.front();
			m_oStreamedMatrices.pop_front();
			m_oStartTime.pop_front();
			m_oEndTime.pop_front();
			for(uint32 c=0 ; c<getChannelCount(); c++)
			{
				m_oChannelMinMaxValues[c].pop_front();
			}
		}
	}

	return l_bMaxDisplayedBufferCountChanged;
}

boolean CSpectrumDatabase::decodeMemoryBuffer(const IMemoryBuffer* pMemoryBuffer, uint64 ui64StartTime, uint64 ui64EndTime)
{
	//if an error has occured, do nothing
	/*if(m_bError)
	{
		return false;
	}*/

	//feed memory buffer to algorithm
	m_pProxy->getInputParameter(OVP_GD_Algorithm_SpectrumStreamDecoder_InputParameterId_MemoryBufferToDecode)->setReferenceTarget(&pMemoryBuffer);

	//process buffer
	m_pProxy->process();

	//has flow header been received?
	if(m_pProxy->isOutputTriggerActive(OVP_GD_Algorithm_SpectrumStreamDecoder_OutputTriggerId_ReceivedHeader) == true)
	{
		//copy streamed matrix header
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oStreamedMatrix;
		l_oStreamedMatrix.initialize(m_pProxy->getOutputParameter(OVP_GD_Algorithm_SpectrumStreamDecoder_OutputParameterId_Matrix));
		Tools::Matrix::copyDescription(m_oStreamedMatrixHeader, *l_oStreamedMatrix);

		//retrieve spectrum header
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oFrequencyBandsMatrix;
		l_oFrequencyBandsMatrix.initialize(m_pProxy->getOutputParameter(OVP_GD_Algorithm_SpectrumStreamDecoder_OutputParameterId_MinMaxFrequencyBands));

		//store frequency bands
		for(uint32 i=0; i<l_oFrequencyBandsMatrix->getBufferElementCount(); i+=2)
		{
			m_oFrequencyBands.push_back(std::pair<float64, float64>(*(l_oFrequencyBandsMatrix->getBuffer()+i),*(l_oFrequencyBandsMatrix->getBuffer()+i+1)));
		}

		m_oChannelMinMaxValues.resize(getChannelCount());

		//create widgets
		m_pDrawable->init();
	}

	//has a buffer been received?
	if(m_pProxy->isOutputTriggerActive(OVP_GD_Algorithm_SpectrumStreamDecoder_OutputTriggerId_ReceivedBuffer) == true)
	{
		//first buffer received
		if(m_bFirstBufferReceived == false)
		{
			uint64 l_ui64BufferDuration = ui64EndTime - ui64StartTime;

			if(l_ui64BufferDuration == 0)
			{
				m_oParentPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning << "Error : buffer start time and end time are equal : " << ui64StartTime << "\n";
				//m_bError = true;
				return false;
			}

			//compute sampling frequency
			m_ui32SamplingFrequency = (uint32)((float64)(((uint64)1<<32)/(l_ui64BufferDuration)) * getSampleCountPerBuffer());

			//update number of buffers accordingly now that sampling frequency is known
			setTimeScale(m_f64TimeScale);

			m_bFirstBufferReceived = true;
		}

		//allocate memory
		CMatrix* l_pCurrentMatrix = NULL;
		if(m_oStreamedMatrices.size() < m_ui64MaxDisplayedBufferCount)
		{
			l_pCurrentMatrix = new CMatrix();
			Tools::Matrix::copyDescription(*l_pCurrentMatrix, m_oStreamedMatrixHeader);
			m_oStreamedMatrices.push_back(l_pCurrentMatrix);
		}
		else //reuse memory for new buffer
		{
			//move front matrix to back of list
			l_pCurrentMatrix = m_oStreamedMatrices.front();
			m_oStreamedMatrices.push_back(l_pCurrentMatrix);

			//remove first matrix data
			m_oStreamedMatrices.pop_front();
			m_oStartTime.pop_front();
			m_oEndTime.pop_front();
			for(uint32 c=0 ; c<getChannelCount(); c++)
			{
				m_oChannelMinMaxValues[c].pop_front();
			}
		}

		//store samples
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oStreamedMatrix;
		l_oStreamedMatrix.initialize(m_pProxy->getOutputParameter(OVP_GD_Algorithm_SpectrumStreamDecoder_OutputParameterId_Matrix));
		Tools::Matrix::copyContent(*l_pCurrentMatrix, *l_oStreamedMatrix);

		//store time stamps
		m_oStartTime.push_back(ui64StartTime);
		m_oEndTime.push_back(ui64EndTime);

		//store min/max values
		float64* l_pBuffer = l_pCurrentMatrix->getBuffer();

		for(uint32 c=0; c<getChannelCount(); c++)
		{
			float64 l_f64ChannelMin = DBL_MAX;
			float64 l_f64ChannelMax = -DBL_MAX;

			for(uint64 i=0; i<getSampleCountPerBuffer(); i++, l_pBuffer++)
			{
				if(*l_pBuffer < l_f64ChannelMin)
				{
					l_f64ChannelMin = *l_pBuffer;
				}
				if(*l_pBuffer > l_f64ChannelMax)
				{
					l_f64ChannelMax = *l_pBuffer;
				}
			}

			m_oChannelMinMaxValues[c].push_back(pair<float64, float64>(l_f64ChannelMin, l_f64ChannelMax));
		}

		if(m_bRedrawOnNewData)
		{
			m_pDrawable->redraw();
		}
	}

	return true;
}

boolean CSpectrumDatabase::getChannelMinMaxValues(uint32 ui32Channel, float64& f64Min, float64& f64Max)
{
	f64Min = +DBL_MAX;
	f64Max = -DBL_MAX;

	if(m_bFirstBufferReceived == false)
	{
		return false;
	}

	if(ui32Channel >= getChannelCount())
	{
		return false;
	}

	for(uint64 i=0; i<m_oChannelMinMaxValues[(size_t)ui32Channel].size(); i++)
	{
		if(f64Min > m_oChannelMinMaxValues[(size_t)ui32Channel][(size_t)i].first)
		{
			f64Min = m_oChannelMinMaxValues[(size_t)ui32Channel][(size_t)i].first;
		}
		if(f64Max < m_oChannelMinMaxValues[(size_t)ui32Channel][(size_t)i].second)
		{
			f64Max = m_oChannelMinMaxValues[(size_t)ui32Channel][(size_t)i].second;
		}
	}

	return true;
}

boolean CSpectrumDatabase::getGlobalMinMaxValues(float64& f64Min, float64& f64Max)
{
	f64Min = +DBL_MAX;
	f64Max = -DBL_MAX;

	if(m_bFirstBufferReceived == false)
	{
		return false;
	}

	for(uint32 c=0; c<getChannelCount(); c++)
	{
		for(uint64 i=0; i<m_oChannelMinMaxValues[(size_t)c].size(); i++)
		{
			if(f64Min > m_oChannelMinMaxValues[(size_t)c][(size_t)i].first)
			{
				f64Min = m_oChannelMinMaxValues[(size_t)c][(size_t)i].first;
			}
			if(f64Max < m_oChannelMinMaxValues[(size_t)c][(size_t)i].second)
			{
				f64Max = m_oChannelMinMaxValues[(size_t)c][(size_t)i].second;
			}
		}
	}

	return true;
}

boolean CSpectrumDatabase::getLastBufferChannelMinMaxValues(uint32 ui32Channel, float64& f64Min, float64& f64Max)
{
	f64Min = +DBL_MAX;
	f64Max = -DBL_MAX;

	if(m_bFirstBufferReceived == false)
	{
		return false;
	}

	if(ui32Channel >= getChannelCount())
	{
		return false;
	}

	f64Min = m_oChannelMinMaxValues[ui32Channel].back().first;
	f64Max = m_oChannelMinMaxValues[ui32Channel].back().second;
	return true;
}

boolean CSpectrumDatabase::getLastBufferGlobalMinMaxValues(float64& f64Min, float64& f64Max)
{
	f64Min = +DBL_MAX;
	f64Max = -DBL_MAX;

	if(m_bFirstBufferReceived == false)
	{
		return false;
	}

	for(uint32 c=0; c<m_oChannelMinMaxValues.size(); c++)
	{
		if(f64Min > m_oChannelMinMaxValues[c].back().first)
		{
			f64Min = m_oChannelMinMaxValues[c].back().first;
		}
		if(f64Max < m_oChannelMinMaxValues[c].back().second)
		{
			f64Max = m_oChannelMinMaxValues[c].back().second;
		}
	}

	return true;
}

uint64 CSpectrumDatabase::getMaxDisplayedBufferCount()
{
	return m_ui64MaxDisplayedBufferCount;
}

uint64 CSpectrumDatabase::getBufferCount()
{
	return (uint64)m_oStreamedMatrices.size();
}

const float64* CSpectrumDatabase::getBuffer(uint32 ui32Index)
{
	if(ui32Index >= m_oStreamedMatrices.size())
	{
		return NULL;
	}
	else
	{
		return m_oStreamedMatrices[ui32Index]->getBuffer();
	}
}

uint64 CSpectrumDatabase::getStartTime(uint32 ui32BufferIndex)
{
	if(ui32BufferIndex >= m_oStartTime.size())
	{
		return 0;
	}
	else
	{
		return m_oStartTime[0];
	}
}

uint64 CSpectrumDatabase::getEndTime(uint32 ui32BufferIndex)
{
	if(ui32BufferIndex >= m_oEndTime.size())
	{
		return 0;
	}
	else
	{
		return m_oEndTime[0];
	}
}

uint64 CSpectrumDatabase::getBufferDuration()
{
	if(m_oStartTime.size() == 0 || m_oEndTime.size() == 0)
	{
		return 0;
	}
	else
	{
		return m_oEndTime[0] - m_oStartTime[0];
	}
}

uint32 CSpectrumDatabase::getSampleCountPerBuffer()
{
	if(m_oStreamedMatrixHeader.getDimensionCount() == 0)
	{
		return 0;
	}
	else
	{
		return m_oStreamedMatrixHeader.getDimensionSize(1);
	}
}

uint32 CSpectrumDatabase::getChannelCount()
{
	if(m_oStreamedMatrixHeader.getDimensionCount() == 0)
	{
		return 0;
	}
	else
	{
		return m_oStreamedMatrixHeader.getDimensionSize(0);
	}
}

boolean CSpectrumDatabase::getChannelLabel(const uint32 ui32ChannelIndex, CString& rElectrodeLabel)
{
	if(m_oStreamedMatrixHeader.getDimensionCount() == 0 || m_oStreamedMatrixHeader.getDimensionSize(0) <= ui32ChannelIndex)
	{
		rElectrodeLabel = "";
		return false;
	}
	else
	{
		rElectrodeLabel = m_oStreamedMatrixHeader.getDimensionLabel(0, ui32ChannelIndex);
		return true;
	}
}

uint32 CSpectrumDatabase::getFrequencyBandCount()
{
	return m_oFrequencyBands.size();
}

float64 CSpectrumDatabase::getFrequencyBandWidth()
{
	if(m_oFrequencyBands.size() == 0)
	{
		return 0;
	}
	else
	{
		return m_oFrequencyBands[0].second - m_oFrequencyBands[0].first;
	}
}

float64 CSpectrumDatabase::getFrequencyBandStart(uint32 ui32FrequencyBandIndex)
{
	if(m_oFrequencyBands.size() == 0)
	{
		return 0;
	}
	else
	{
		return m_oFrequencyBands[ui32FrequencyBandIndex].first;
	}
}

float64 CSpectrumDatabase::getFrequencyBandStop(uint32 ui32FrequencyBandIndex)
{
	if(ui32FrequencyBandIndex >= m_oFrequencyBands.size())
	{
		return 0;
	}
	else
	{
		return m_oFrequencyBands[ui32FrequencyBandIndex].second;
	}
}
