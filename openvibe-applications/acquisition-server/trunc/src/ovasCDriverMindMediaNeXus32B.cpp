#include "ovasCDriverMindMediaNeXus32B.h"
#include "ovasIHeader.h"
#include "ovasIHeaderConfigurator.h"
#include "ovasCHeaderConfiguratorGlade.h"

#include <openvibe-toolkit/ovtk_all.h>

#include <system/Time.h>

#include <math.h>

#if defined OVAS_OS_Windows
 #include <windows.h>
 #define boolean OpenViBEAcquisitionServer::boolean
#endif

using namespace OpenViBEAcquisitionServer;

#define OVAS_ElectrodeNames_File "../share/openvibe-applications/acquisition-server/electrode-names.txt"
#define OVAS_ConfigureGUI_File   "../share/openvibe-applications/acquisition-server/interface-MindMedia-NeXus32B.glade"

//___________________________________________________________________//
//                                                                   //

CDriverMindMediaNeXus32B::CDriverMindMediaNeXus32B(void)
	:m_pCallback(NULL)
	,m_pHeader(NULL)
	,m_bInitialized(false)
	,m_bStarted(false)
	,m_ui32SampleCountPerSentBlock(0)
	,m_pSample(NULL)
	,m_ui32SampleIndex(0)
{
	m_pHeader=createHeader();
	m_pHeader->setSamplingFrequency(512);
	m_pHeader->setChannelCount(4);
}

CDriverMindMediaNeXus32B::~CDriverMindMediaNeXus32B(void)
{
	m_pHeader->release();
	m_pHeader=NULL;
}

void CDriverMindMediaNeXus32B::release(void)
{
	delete this;
}

const char* CDriverMindMediaNeXus32B::getName(void)
{
	return "MindMedia NeXus32B";
}

//___________________________________________________________________//
//                                                                   //

#if defined OVAS_OS_Windows

#define _MindMedia_NeXus32B_DLLFileName_ "NeXusDLL.dll"

typedef void (*NeXusDLL_ProcessData)(int iSampleCount, int iChannel, float* pSample);
typedef ::DWORD (*NeXusDLL_Init)(::NeXusDLL_ProcessData fpProcessData);
typedef ::DWORD (*NeXusDLL_Start)(::DWORD* dwSamplingRate);
typedef ::DWORD (*NeXusDLL_Stop)(void);

static HINSTANCE g_hNeXusDLLInstance=NULL;
static NeXusDLL_Init g_fpNeXusDLLInit=NULL;
static NeXusDLL_Start g_fpNeXusDLLStart=NULL;
static NeXusDLL_Stop g_fpNeXusDLLStop=NULL;

static OpenViBEAcquisitionServer::CDriverMindMediaNeXus32B* g_pDriver=NULL;

//___________________________________________________________________//
//                                                                   //

static void processData(int iSampleCount, int iChannel, float* pSample)
{
	if(g_pDriver)
	{
		g_pDriver->processData((uint32)iSampleCount, (uint32) iChannel, (float32*) pSample);
	}
}

#endif

//___________________________________________________________________//
//                                                                   //

boolean CDriverMindMediaNeXus32B::initialize(
	const uint32 ui32SampleCountPerSentBlock,
	IDriverCallback& rCallback)
{
#if defined OVAS_OS_Windows

	if(m_bInitialized)
	{
		return false;
	}

	g_hNeXusDLLInstance=::LoadLibrary(_MindMedia_NeXus32B_DLLFileName_);
	if(!g_hNeXusDLLInstance)
	{
		return false;
	}

	g_fpNeXusDLLInit=(NeXusDLL_Init)GetProcAddress(g_hNeXusDLLInstance, "InitNeXusDevice");
	g_fpNeXusDLLStart=(NeXusDLL_Start)GetProcAddress(g_hNeXusDLLInstance,"StartNeXusDevice");
	g_fpNeXusDLLStop=(NeXusDLL_Stop)GetProcAddress(g_hNeXusDLLInstance, "StopNeXusDevice");
	m_pSample=new float32[m_pHeader->getChannelCount()*ui32SampleCountPerSentBlock];

	if(!g_fpNeXusDLLInit || !g_fpNeXusDLLStart || !g_fpNeXusDLLStop || !m_pSample)
	{
		::FreeLibrary(g_hNeXusDLLInstance);
		delete [] m_pSample;
		g_hNeXusDLLInstance=NULL;
		g_fpNeXusDLLInit=NULL;
		g_fpNeXusDLLStart=NULL;
		g_fpNeXusDLLStop=NULL;
		m_pSample=NULL;
		return false;
	}

	::DWORD l_dwError=g_fpNeXusDLLInit(::processData);
	if(l_dwError)
	{
		::FreeLibrary(g_hNeXusDLLInstance);
		delete [] m_pSample;
		g_hNeXusDLLInstance=NULL;
		g_fpNeXusDLLInit=NULL;
		g_fpNeXusDLLStart=NULL;
		g_fpNeXusDLLStop=NULL;
		m_pSample=NULL;
		return false;
	}

	m_pCallback=&rCallback;
	m_bInitialized=true;
	m_ui32SampleCountPerSentBlock=ui32SampleCountPerSentBlock;
	m_ui32SampleIndex=0;
	g_pDriver=this;

	return true;

#else

	return false;

#endif
}

boolean CDriverMindMediaNeXus32B::start(void)
{
#if defined OVAS_OS_Windows

	if(!m_bInitialized)
	{
		return false;
	}

	if(m_bStarted)
	{
		return false;
	}

	::DWORD l_dwSamplingFrequency=::DWORD(m_pHeader->getSamplingFrequency());
	::DWORD l_dwError=g_fpNeXusDLLStart(&l_dwSamplingFrequency);
	m_bStarted=(l_dwError?false:true);
	return m_bStarted;

#else

	return false;

#endif
}

boolean CDriverMindMediaNeXus32B::loop(void)
{
#if defined OVAS_OS_Windows

	if(!m_bInitialized)
	{
		return false;
	}

	if(!m_bStarted)
	{
		return false;
	}

	return true;

#else

	return false;

#endif
}

boolean CDriverMindMediaNeXus32B::stop(void)
{
#if defined OVAS_OS_Windows

	if(!m_bInitialized)
	{
		return false;
	}

	if(!m_bStarted)
	{
		return false;
	}

	::DWORD l_dwError=g_fpNeXusDLLStop();
	m_bStarted=(l_dwError?true:false);
	return !m_bStarted;

#else

	return false;

#endif
}

boolean CDriverMindMediaNeXus32B::uninitialize(void)
{
#if defined OVAS_OS_Windows

	if(!m_bInitialized)
	{
		return false;
	}

	if(m_bStarted)
	{
		return false;
	}

	m_bInitialized=false;

	::FreeLibrary(g_hNeXusDLLInstance);
	delete [] m_pSample;
	m_pSample=NULL;
	m_pCallback=NULL;
	g_hNeXusDLLInstance=NULL;
	g_fpNeXusDLLInit=NULL;
	g_fpNeXusDLLStart=NULL;
	g_fpNeXusDLLStop=NULL;
	g_pDriver=NULL;

	return true;

#else

	return false;

#endif
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverMindMediaNeXus32B::isConfigurable(void)
{
#if defined OVAS_OS_Windows

	return true;

#else

	return false;

#endif
}

boolean CDriverMindMediaNeXus32B::configure(void)
{
#if defined OVAS_OS_Windows

	boolean l_bResult=false;
	IHeaderConfigurator* l_pHeaderConfigurator=NULL;

	l_pHeaderConfigurator=createHeaderConfiguratorGlade(OVAS_ConfigureGUI_File, OVAS_ElectrodeNames_File);
	if(l_pHeaderConfigurator)
	{
		l_bResult=l_pHeaderConfigurator->configure(*m_pHeader);
	}
	l_pHeaderConfigurator->release();

	return l_bResult;

#else

	return false;

#endif
}

void CDriverMindMediaNeXus32B::processData(
	uint32 ui32SampleCount,
	uint32 ui32Channel,
	float32* pSample)
{
#if defined OVAS_OS_Windows

	for(uint32 i=0; i<m_pHeader->getChannelCount(); i++)
	{
		m_pSample[m_ui32SampleIndex+i*m_ui32SampleCountPerSentBlock]=pSample[i];
	}
	m_ui32SampleIndex++;
	m_ui32SampleIndex%=m_ui32SampleCountPerSentBlock;
	if(!m_ui32SampleIndex)
	{
		m_pCallback->setSamples(m_pSample);
	}

#endif
}
