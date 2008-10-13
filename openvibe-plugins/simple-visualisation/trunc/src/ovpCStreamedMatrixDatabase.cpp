#include <system/Memory.h>
#include <algorithm>
#include <math.h>

#include "ovpCStreamedMatrixDatabase.h"

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBE::Kernel;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

CStreamedMatrixDatabase::CStreamedMatrixDatabase(OpenViBEToolkit::TBoxAlgorithm<Plugins::IBoxAlgorithm>& oPlugin) :
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
		m_oParentPlugin.getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StreamedMatrixStreamDecoder));

	m_pProxy->initialize();
}

CStreamedMatrixDatabase::~CStreamedMatrixDatabase()
{
	m_pProxy->uninitialize();
	m_oParentPlugin.getAlgorithmManager().releaseAlgorithm(*m_pProxy);

	while(m_oStreamedMatrices.size() >0)
	{
		delete m_oStreamedMatrices.front();
		m_oStreamedMatrices.pop_front();
	}
}

void CStreamedMatrixDatabase::setDrawable(IStreamDisplayDrawable* pDrawable)
{
	m_pDrawable = pDrawable;
}

void CStreamedMatrixDatabase::setRedrawOnNewData(boolean bRedrawOnNewData)
{
	m_bRedrawOnNewData = bRedrawOnNewData;
}

boolean CStreamedMatrixDatabase::isFirstBufferReceived()
{
	return m_bFirstBufferReceived;
}

boolean CStreamedMatrixDatabase::setTimeScale(float64 f64TimeScale)
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
			/*for(uint32 c=0 ; c<getChannelCount(); c++)
			{
				m_oChannelMinMaxValues[c].pop_front();
			}*/
		}
	}

	return l_bMaxDisplayedBufferCountChanged;
}

boolean CStreamedMatrixDatabase::decodeMemoryBuffer(const IMemoryBuffer* pMemoryBuffer, uint64 ui64StartTime, uint64 ui64EndTime)
{
	//if an error has occured, do nothing
	/*if(m_bError)
	{
		return false;
	}*/

	//feed memory buffer to algorithm
	m_pProxy->getInputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_InputParameterId_MemoryBufferToDecode)->setReferenceTarget(&pMemoryBuffer);

	//process buffer
	m_pProxy->process();

	//has flow header been received?
	if(m_pProxy->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedHeader) == true)
	{
		//copy streamed matrix header
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oStreamedMatrix;
		l_oStreamedMatrix.initialize(m_pProxy->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputParameterId_Matrix));
		Tools::Matrix::copyDescription(m_oStreamedMatrixHeader, *l_oStreamedMatrix);

		//m_oChannelMinMaxValues.resize(getChannelCount());

		//create widgets
		m_pDrawable->init();
	}

	//has a buffer been received?
	if(m_pProxy->isOutputTriggerActive(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputTriggerId_ReceivedBuffer) == true)
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
			/*for(uint32 c=0 ; c<getChannelCount(); c++)
			{
				m_oChannelMinMaxValues[c].pop_front();
			}*/
		}

		//store samples
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oStreamedMatrix;
		l_oStreamedMatrix.initialize(m_pProxy->getOutputParameter(OVP_GD_Algorithm_StreamedMatrixStreamDecoder_OutputParameterId_Matrix));
		Tools::Matrix::copyContent(*l_pCurrentMatrix, *l_oStreamedMatrix);

		//store time stamps
		m_oStartTime.push_back(ui64StartTime);
		m_oEndTime.push_back(ui64EndTime);

		//store min/max values
		/*
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
		}*/

		if(m_bRedrawOnNewData)
		{
			m_pDrawable->redraw();
		}
	}

	return true;
}

uint64 CStreamedMatrixDatabase::getMaxDisplayedBufferCount()
{
	return m_ui64MaxDisplayedBufferCount;
}

uint64 CStreamedMatrixDatabase::getBufferCount()
{
	return (uint64)m_oStreamedMatrices.size();
}

const float64* CStreamedMatrixDatabase::getBuffer(uint32 ui32Index)
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

uint64 CStreamedMatrixDatabase::getStartTime(uint32 ui32BufferIndex)
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

uint64 CStreamedMatrixDatabase::getEndTime(uint32 ui32BufferIndex)
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

uint64 CStreamedMatrixDatabase::getBufferDuration()
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

uint32 CStreamedMatrixDatabase::getSampleCountPerBuffer()
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

uint32 CStreamedMatrixDatabase::getChannelCount()
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

boolean CStreamedMatrixDatabase::getChannelLabel(const uint32 ui32ChannelIndex, CString& rElectrodeLabel)
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
