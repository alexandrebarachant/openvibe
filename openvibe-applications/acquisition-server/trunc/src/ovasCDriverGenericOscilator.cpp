#include "ovasCDriverGenericOscilator.h"
#include "ovasIHeader.h"
#include "ovasIHeaderConfigurator.h"
#include "ovasCHeaderConfiguratorGlade.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <system/Time.h>

#include <math.h>

#include <iostream>

using namespace OpenViBEAcquisitionServer;
using namespace std;

#define OVAS_ElectrodeNames_File           "../share/openvibe-applications/acquisition-server/electrode-names.txt"
#define OVAS_ConfigureGUI_File             "../share/openvibe-applications/acquisition-server/interface-Generic-Oscillator.glade"
#define OVAS_ConfigureGUIElectrodes_File   "../share/openvibe-applications/acquisition-server/interface-channel-names.glade"

//___________________________________________________________________//
//                                                                   //

CDriverGenericOscillator::CDriverGenericOscillator(void)
	:m_pCallback(NULL)
	,m_pHeader(NULL)
	,m_bInitialized(false)
	,m_bStarted(false)
	,m_ui32SampleCountPerSentBlock(0)
	,m_pSample(NULL)
	,m_ui32TotalSampleCount(0)
	,m_ui32StartTime(0)
{
	m_pHeader=createHeader();
	m_pHeader->setSamplingFrequency(512);
	m_pHeader->setChannelCount(4);
}

CDriverGenericOscillator::~CDriverGenericOscillator(void)
{
	m_pHeader->release();
	m_pHeader=NULL;
}

void CDriverGenericOscillator::release(void)
{
	delete this;
}

const char* CDriverGenericOscillator::getName(void)
{
	return "Generic Oscillator";
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverGenericOscillator::initialize(
	const uint32 ui32SampleCountPerSentBlock,
	IDriverCallback& rCallback)
{
	if(m_bInitialized)
	{
		return false;
	}

	if(!m_pHeader->isChannelCountSet()
	 ||!m_pHeader->isSamplingFrequencySet())
	{
		return false;
	}

	m_pSample=new float32[m_pHeader->getChannelCount()*ui32SampleCountPerSentBlock];
	if(!m_pSample)
	{
		delete [] m_pSample;
		m_pSample=NULL;
		return false;
	}

	m_pCallback=&rCallback;
	m_bInitialized=true;
	m_ui32SampleCountPerSentBlock=ui32SampleCountPerSentBlock;
	m_ui32TotalSampleCount=0;

	m_ui32StartTime=System::Time::getTime();
	return true;
}

boolean CDriverGenericOscillator::start(void)
{
	if(!m_bInitialized)
	{
		return false;
	}

	if(m_bStarted)
	{
		return false;
	}

	m_bStarted=true;
	return m_bStarted;
}

boolean CDriverGenericOscillator::loop(void)
{
	if(!m_bInitialized)
	{
		return false;
	}

	if(!m_bStarted)
	{
		return false;
	}

	uint32 l_ui32CurrentTime=System::Time::getTime();

	if(l_ui32CurrentTime-m_ui32StartTime > (1000.0*m_ui32TotalSampleCount)/m_pHeader->getSamplingFrequency())
	{
		for(uint32 j=0; j<m_pHeader->getChannelCount(); j++)
		{
			for(uint32 i=0; i<m_ui32SampleCountPerSentBlock; i++)
			{
#if 1
				float64 l_f64Value=
					sin(((i+m_ui32TotalSampleCount)*(j+1)*12.3)/m_pHeader->getSamplingFrequency())+
					sin(((i+m_ui32TotalSampleCount)*(j+1)* 4.5)/m_pHeader->getSamplingFrequency())+
					sin(((i+m_ui32TotalSampleCount)*(j+1)*67.8)/m_pHeader->getSamplingFrequency());
				m_pSample[j*m_ui32SampleCountPerSentBlock+i]=(float32)l_f64Value;
#else
				m_pSample[j*m_ui32SampleCountPerSentBlock+i]=j;
#endif
			}
		}

		m_ui32TotalSampleCount+=m_ui32SampleCountPerSentBlock;
		m_pCallback->setSamples(m_pSample);
	}

	return true;
}

boolean CDriverGenericOscillator::stop(void)
{
	if(!m_bInitialized)
	{
		return false;
	}

	if(!m_bStarted)
	{
		return false;
	}

	m_bStarted=false;
	return !m_bStarted;
}

boolean CDriverGenericOscillator::uninitialize(void)
{
	if(!m_bInitialized)
	{
		return false;
	}

	if(m_bStarted)
	{
		return false;
	}

	m_bInitialized=false;

	delete [] m_pSample;
	m_pSample=NULL;
	m_pCallback=NULL;

	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverGenericOscillator::isConfigurable(void)
{
	return true;
}

boolean CDriverGenericOscillator::configure(void)
{
	boolean l_bResult=false;
	IHeaderConfigurator* l_pHeaderConfigurator=NULL;

	l_pHeaderConfigurator=createHeaderConfiguratorGlade(OVAS_ConfigureGUI_File, OVAS_ElectrodeNames_File, OVAS_ConfigureGUIElectrodes_File);
	if(l_pHeaderConfigurator)
	{
		l_bResult=l_pHeaderConfigurator->configure(*m_pHeader);
	}
	l_pHeaderConfigurator->release();

	return l_bResult;
}
