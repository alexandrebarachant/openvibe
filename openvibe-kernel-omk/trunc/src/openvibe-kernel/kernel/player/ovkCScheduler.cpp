#include "ovkCScheduler.h"
#include "ovkCSimulatedBox.h"

#include <system/Time.h>

#include <xml/IReader.h>

#include <string>
#include <iostream>
#include <fstream>

//___________________________________________________________________//
//                                                                   //

using namespace std;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

//___________________________________________________________________//
//                                                                   //

#define OVD_AttributeId_SettingOverrideFilename             OpenViBE::CIdentifier(0x8D21FF41, 0xDF6AFE7E)

class CBoxSettingModifierVisitor : public IObjectVisitor, public XML::IReaderCallback
{
public:

	virtual void openChild(const char* sName, const char** sAttributeName, const char** sAttributeValue, XML::uint64 ui64AttributeCount)
	{
		if(!m_bIsParsingSettingOverride)
		{
			if(string(sName)==string("OpenViBE-SettingsOverride"))
			{
				m_bIsParsingSettingOverride=true;
			}
		}
		else if(string(sName)==string("SettingValue"))
		{
			m_bIsParsingSettingValue=true;
		}
		else
		{
			m_bIsParsingSettingValue=false;
		}
	}

	virtual void processChildData(const char* sData)
	{
		if(m_bIsParsingSettingValue)
		{
			m_pObjectVisitorContext->getLogManager() << LogLevel_Debug << "Using [" << CString(sData) << "] as setting " << m_ui32SettingIndex << "...\n";
			m_pBox->setSettingValue(m_ui32SettingIndex, sData);
			m_ui32SettingIndex++;
		}
	}

	virtual void closeChild(void)
	{
		m_bIsParsingSettingValue=false;
	}

	virtual boolean processBegin(IObjectVisitorContext& rObjectVisitorContext, IBox& rBox)
	{
		m_pObjectVisitorContext=&rObjectVisitorContext;

		// checks if this box should override
		// settings from external file
		if(rBox.hasAttribute(OVD_AttributeId_SettingOverrideFilename))
		{
			CString l_sSettingOverrideFilename=rBox.getAttributeValue(OVD_AttributeId_SettingOverrideFilename);;

			// message
			rObjectVisitorContext.getLogManager() << LogLevel_Trace << "Trying to override [" << rBox.getName() << "] box settings with file [" << l_sSettingOverrideFilename << "] !\n";

			// creates XML reader
			XML::IReader* l_pReader=XML::createReader(*this);

			// adds new box settings
			m_pBox=&rBox;
			m_ui32SettingIndex=0;
			m_bIsParsingSettingValue=false;
			m_bIsParsingSettingOverride=false;

			// 1. Open settings file (binary because read would conflict with tellg for text files)
			// 2. Loop until end of file, reading it
			//    and sending what is read to the XML parser
			// 3. Close the settings file
			ifstream l_oFile(l_sSettingOverrideFilename.toASCIIString(), ios::binary);
			if(l_oFile.is_open())
			{
				char l_sBuffer[1024];
				size_t l_iBufferLen=0;
				size_t l_iFileLen;
				bool l_bStatusOk=true;
				l_oFile.seekg(0, ios::end);
				l_iFileLen=l_oFile.tellg();
				l_oFile.seekg(0, ios::beg);
				while(l_iFileLen && l_bStatusOk)
				{
					l_iBufferLen=(l_iFileLen>sizeof(l_sBuffer)?sizeof(l_sBuffer):l_iFileLen);
					l_oFile.read(l_sBuffer, l_iBufferLen);
					l_iFileLen-=l_iBufferLen;
					l_bStatusOk=l_pReader->processData(l_sBuffer, l_iBufferLen);
				}
				l_oFile.close();
			}

			// message
			rObjectVisitorContext.getLogManager() << LogLevel_Trace << "Overrided " << m_ui32SettingIndex << " settings with this configuration file...\n";

			// cleans up internal state
			m_pBox=NULL;
			m_ui32SettingIndex=0;
			m_bIsParsingSettingValue=false;
			m_bIsParsingSettingOverride=false;

			// releases XML reader
			l_pReader->release();
			l_pReader=NULL;
		}

		return true;
	}

	virtual boolean processEnd(IObjectVisitorContext& rObjectVisitorContext, IBox& rBox)
	{
		m_pObjectVisitorContext=&rObjectVisitorContext;
		return true;
	}

	IObjectVisitorContext* m_pObjectVisitorContext;
	IBox* m_pBox;
	uint32 m_ui32SettingIndex;
	boolean m_bIsParsingSettingValue;
	boolean m_bIsParsingSettingOverride;

#undef boolean
	_IsDerivedFromClass_Final_(IObjectVisitor, OV_UndefinedIdentifier);
#define boolean OpenViBE::boolean
};

//___________________________________________________________________//
//                                                                   //

CScheduler::CScheduler(const IKernelContext& rKernelContext)
	:TKernelObject<IKernelObject>(rKernelContext)
	,m_oScenarioIdentifier(OV_UndefinedIdentifier)
	,m_pScenario(NULL)
	,m_ui64Steps(0)
	,m_ui64Frequency(0)
	,m_ui64CurrentTime(0)
	,m_bIsInitialized(false)
{
}

CScheduler::~CScheduler(void)
{
	if(m_bIsInitialized)
	{
		this->uninitialize();
	}
}

//___________________________________________________________________//
//                                                                   //

boolean CScheduler::setScenario(
	const CIdentifier& rScenarioIdentifier)
{
	log() << LogLevel_Trace << "Scheduler setScenario\n";

	if(m_bIsInitialized)
	{
		log() << LogLevel_Warning << "Trying to configure an intialized scheduler !\n";
		return false;
	}

	m_oScenarioIdentifier=rScenarioIdentifier;
	m_pScenario=NULL;
	return true;
}

boolean CScheduler::setFrequency(
	const uint64 ui64Frequency)
{
	log() << LogLevel_Trace << "Scheduler setFrequency\n";

	if(m_bIsInitialized)
	{
		log() << LogLevel_Warning << "Trying to configure an intialized scheduler !\n";
		return false;
	}

	m_ui64Frequency=ui64Frequency;
	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CScheduler::initialize(void)
{
	log() << LogLevel_Trace << "Scheduler initialize\n";

	if(m_bIsInitialized)
	{
		log() << LogLevel_Warning << "Trying to initialize an intialized scheduler !\n";
		return false;
	}

	CIdentifier l_oNewScenarioIdentifier=m_oScenarioIdentifier;
/*
	getScenarioManager().getScenario(m_oScenarioIdentifier).save("tmp.scenario.xml", CIdentifier(0x77075b3b, 0x3d632492)); // $$$
	getScenarioManager().createScenario(l_oNewScenarioIdentifier);
	getScenarioManager().getScenario(l_oNewScenarioIdentifier).load("tmp.scenario.xml", CIdentifier(0x440BF3AC, 0x2D960300)); // $$$
*/
	m_pScenario=&getScenarioManager().getScenario(l_oNewScenarioIdentifier);
/*
	CBoxSettingModifierVisitor l_oBoxSettingModifierVisitor;
	l_rScenario.acceptVisitor(l_oBoxSettingModifierVisitor);
*/

	CIdentifier l_oBoxIdentifier;
	while((l_oBoxIdentifier=m_pScenario->getNextBoxIdentifier(l_oBoxIdentifier))!=OV_UndefinedIdentifier)
	{
		CSimulatedBox* l_pSimulatedBox=new CSimulatedBox(getKernelContext(), *this);
		l_pSimulatedBox->setScenarioIdentifier(m_oScenarioIdentifier);
		l_pSimulatedBox->setBoxIdentifier(l_oBoxIdentifier);
		m_vSimulatedBox[l_oBoxIdentifier]=l_pSimulatedBox;
	}

	for(map < CIdentifier, CSimulatedBox* >::iterator itSimulatedBox=m_vSimulatedBox.begin(); itSimulatedBox!=m_vSimulatedBox.end(); itSimulatedBox++)
	{
		if(itSimulatedBox->second)
		{
			itSimulatedBox->second->initialize();
		}
	}

	m_ui64Steps=0;
	m_ui64CurrentTime=0;
	m_bIsInitialized=true;

	m_oBenchmarkChrono.reset(m_ui64Frequency);

	return true;

}

boolean CScheduler::uninitialize(void)
{
	log() << LogLevel_Trace << "Scheduler uninitialize\n";

	if(!m_bIsInitialized)
	{
		log() << LogLevel_Warning << "Trying to uninitialize an uninitialized player !\n";
		return false;
	}

	for(map < CIdentifier, CSimulatedBox* >::iterator itSimulatedBox=m_vSimulatedBox.begin(); itSimulatedBox!=m_vSimulatedBox.end(); itSimulatedBox++)
	{
		if(itSimulatedBox->second)
		{
			itSimulatedBox->second->uninitialize();
		}
	}

	for(map < CIdentifier, CSimulatedBox* >::iterator itSimulatedBox=m_vSimulatedBox.begin(); itSimulatedBox!=m_vSimulatedBox.end(); itSimulatedBox++)
	{
		delete itSimulatedBox->second;
	}
	m_vSimulatedBox.clear();

	m_bIsInitialized=false;
	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CScheduler::loop(void)
{
	if(!m_bIsInitialized)
	{
		return false;
	}

	m_oBenchmarkChrono.stepIn();
	for(map < CIdentifier, CSimulatedBox* >::iterator itSimulatedBox=m_vSimulatedBox.begin(); itSimulatedBox!=m_vSimulatedBox.end(); itSimulatedBox++)
	{
		CSimulatedBox* l_pSimulatedBox=itSimulatedBox->second;
		if(l_pSimulatedBox)
		{
			l_pSimulatedBox->processClock();

			if(l_pSimulatedBox->isReadyToProcess())
			{
				l_pSimulatedBox->process();
			}

			map < uint32, list < CChunk > >& l_rSimulatedBoxInput=m_vSimulatedBoxInput[itSimulatedBox->first];
			map < uint32, list < CChunk > >::iterator itSimulatedBoxInput;
			for(itSimulatedBoxInput=l_rSimulatedBoxInput.begin(); itSimulatedBoxInput!=l_rSimulatedBoxInput.end(); itSimulatedBoxInput++)
			{
				list < CChunk >& l_rSimulatedBoxInputChunkList=itSimulatedBoxInput->second;
				list < CChunk >::iterator itSimulatedBoxInputChunkList;
				for(itSimulatedBoxInputChunkList=l_rSimulatedBoxInputChunkList.begin(); itSimulatedBoxInputChunkList!=l_rSimulatedBoxInputChunkList.end(); itSimulatedBoxInputChunkList++)
				{
					l_pSimulatedBox->processInput(itSimulatedBoxInput->first, *itSimulatedBoxInputChunkList);

					if(l_pSimulatedBox->isReadyToProcess())
					{
						l_pSimulatedBox->process();
					}
				}
				l_rSimulatedBoxInputChunkList.clear();
			}
		}
	}
	m_oBenchmarkChrono.stepOut();

	if((m_ui64Steps%m_ui64Frequency)==0)
	{
		log() << LogLevel_Debug
			<< "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Scheduler" << LogColor_PopStateBit
			<< "::" << LogColor_PushStateBit << LogColor_ForegroundBlue << "elapsed time" << LogColor_PopStateBit << "> "
			<< m_ui64Steps/m_ui64Frequency << "s\n";
	}

	if(m_oBenchmarkChrono.hasNewEstimation())
	{
		log() << LogLevel_Benchmark
			<< "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Scheduler" << LogColor_PopStateBit
			<< "::" << LogColor_PushStateBit << LogColor_ForegroundBlue << "processor use" << LogColor_PopStateBit << "> "
			<< m_oBenchmarkChrono.getStepInPercentage() << "%\n";
	}

	m_ui64Steps++;
	m_ui64CurrentTime=m_ui64Steps*((1LL<<32)/m_ui64Frequency);

	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CScheduler::sendInput(
	const CChunk& rChunk,
	const CIdentifier& rBoxIdentifier,
	const uint32 ui32InputIndex)
{
	IBox* l_pBox=m_pScenario->getBoxDetails(rBoxIdentifier);
	if(!l_pBox)
	{
		getLogManager() << LogLevel_Warning << "Tried to send data chunk with invalid box identifier " << rBoxIdentifier << "\n";
		return false;
	}

	if(ui32InputIndex >= l_pBox->getInputCount())
	{
		getLogManager() << LogLevel_Warning << "Tried to send data chunk with invalid input index " << ui32InputIndex << " for box identifier" << rBoxIdentifier << "\n";
		return false;
	}

	map < CIdentifier, CSimulatedBox* >::iterator itSimulatedBox=m_vSimulatedBox.find(rBoxIdentifier);
	if(itSimulatedBox==m_vSimulatedBox.end())
	{
		getLogManager() << LogLevel_ImportantWarning << "Tried to send data chunk with valid box identifier but invalid simulated box identifier " << rBoxIdentifier << "\n";
		return false;
	}

	CSimulatedBox* l_pSimulatedBox=itSimulatedBox->second;
	if(!l_pSimulatedBox)
	{
		getLogManager() << LogLevel_ImportantWarning << "Tried to send data chunk with valid box identifier, valid simulated box identifier " << rBoxIdentifier << " but the box has never been created\n";
		return false;
	}

	// TODO: check if ui32InputIndex does not overflow

	m_vSimulatedBoxInput[rBoxIdentifier][ui32InputIndex].push_back(rChunk);

	return true;
}

uint64 CScheduler::getCurrentTime(void) const
{
	return m_ui64CurrentTime;
}
