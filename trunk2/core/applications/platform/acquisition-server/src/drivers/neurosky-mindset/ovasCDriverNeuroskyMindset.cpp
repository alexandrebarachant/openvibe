#if defined TARGET_HAS_ThirdPartyThinkGearAPI

#include "ovasCDriverNeuroskyMindset.h"
#include "ovasCConfigurationNeuroskyMindset.h"

#include <sstream>
#include <system/Time.h>

#include <thinkgear.h>

#include <openvibe/ovITimeArithmetics.h>

using namespace OpenViBEAcquisitionServer;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

//___________________________________________________________________//
//                                                                   //

CDriverNeuroskyMindset::CDriverNeuroskyMindset(IDriverContext& rDriverContext)
	:IDriver(rDriverContext)
	,m_pCallback(NULL)
	,m_ui32SampleCountPerSentBlock(0)
	,m_ui32TotalSampleCount(0)
	,m_pSample(NULL)
	,m_ui32WarningCount(-1)
{
	m_oHeader.setSamplingFrequency(512); // raw signal sampling frequency, from the official documentation.
	// CHANNEL COUNT
	m_oHeader.setChannelCount(1); // one channel on the forhead
	m_oHeader.setChannelName(0,"Electrode");

	m_i32ConnectionID = -1;

	m_ui32ComPort = OVAS_MINDSET_INVALID_COM_PORT;

	m_bESenseChannels       = m_rDriverContext.getConfigurationManager().expandAsBoolean("${AcquisitionServer_NeuroskyMindset_ESenseValues}", false);
	m_bBandPowerChannels    = m_rDriverContext.getConfigurationManager().expandAsBoolean("${AcquisitionServer_NeuroskyMindset_PowerBands}", false);
	m_bBlinkStimulations    = m_rDriverContext.getConfigurationManager().expandAsBoolean("${AcquisitionServer_NeuroskyMindset_Blink}", false);
	m_bBlinkStrenghtChannel = m_rDriverContext.getConfigurationManager().expandAsBoolean("${AcquisitionServer_NeuroskyMindset_BlinkStrength}", false);
}

CDriverNeuroskyMindset::~CDriverNeuroskyMindset(void)
{
}

const char* CDriverNeuroskyMindset::getName(void)
{
	return "NeuroSky MindSet (MindSet Dev. Kit 2.1+)";
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverNeuroskyMindset::initialize(
	const uint32 ui32SampleCountPerSentBlock,
	IDriverCallback& rCallback)
{
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Mindset Driver: INIT called.\n";
	if(m_rDriverContext.isConnected())
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] Mindset Driver: Driver already initialized.\n";
		return false;
	}

	/*
	11 channels
	-----
	eSense values:
	TG_DATA_ATTENTION
	TG_DATA_MEDITATION
	
	Raw EEG data:
	TG_DATA_RAW     (512 Hz sampling frequency)
	
	Power in defined frequency bands:
	TG_DATA_DELTA   (0.5 - 2.75 Hz)
	TG_DATA_THETA   (3.5 - 6.75 Hz)
	TG_DATA_ALPHA1  (7.5 - 9.25 Hz)
	TG_DATA_ALPHA2  (10 - 11.75 Hz)
	TG_DATA_BETA1   (13 - 16.75 Hz)
	TG_DATA_BETA2   (18 - 29.75 Hz)
	TG_DATA_GAMMA1  (31 - 39.75 Hz)
	TG_DATA_GAMMA2  (41 - 49.75 Hz)

	Since MindSet Development Kit 2.1:
	TG_DATA_BLINK_STRENGTH (1-255)
	*/

	// the tokens allow user to use all data coming from headset. 
	// Raw EEG is sampled at 512Hz, other data are sampled around 1Hz 
	// so the driver sends these data at 512 Hz, changing value every seconds (we obtain a square signal)
	// The Blink Strength can be viewed as spike signal, sampled at 512 Hz.
	// The Blinks can be viewed as an OpenViBE stimulation OVTK_GDF_Eye_Blink

	m_oHeader.setChannelCount(1); // one channel on the forhead
	m_oHeader.setChannelName(0,"Electrode");

	if(m_bESenseChannels)
	{
		m_oHeader.setChannelCount(m_oHeader.getChannelCount()+2);
	}
	if(m_bBandPowerChannels)
	{
		m_oHeader.setChannelCount(m_oHeader.getChannelCount()+8);
	}
	if(m_bBlinkStrenghtChannel)
	{
		m_oHeader.setChannelCount(m_oHeader.getChannelCount()+1);
	}

	// NAMES
	uint32 l_ui32ChannelIndex = 1;
	
	if(m_bESenseChannels)
	{
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"Attention");
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"Meditation");
	}
	if(m_bBandPowerChannels)
	{
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"Delta");
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"Theta");
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"Low Alpha");
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"High Alpha");
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"Low Beta");
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"High Beta");
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"Low Gamma");
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"Mid Gamma");
	}
	// spikes for blink strength
	if(m_bBlinkStrenghtChannel)
	{
		m_oHeader.setChannelName(l_ui32ChannelIndex++,"Blink Strength");
	}
	
	if(!m_oHeader.isChannelCountSet()
	 ||!m_oHeader.isSamplingFrequencySet())
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] Mindset Driver: Channel count or frequency not set.\n";
		return false;
	}

	// Builds up a buffer to store acquired samples. This buffer will be sent to the acquisition server later.
	m_pSample=new float32[m_oHeader.getChannelCount()*ui32SampleCountPerSentBlock];
	if(!m_pSample)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] Mindset Driver: Samples allocation failed.\n";
		return false;
	}


	int32 l_iDllVersion = 0;
	int32 l_iConnectionId = 0;
	int32 l_iErrCode = 0;

	/* Print driver version number */
	l_iDllVersion = TG_GetDriverVersion();
	m_rDriverContext.getLogManager() << LogLevel_Info << "ThinkGear DLL version: "<< l_iDllVersion <<"\n";
	if(l_iDllVersion > 7)
	{
		m_rDriverContext.getLogManager() << LogLevel_Info << "Eye blink detection is possible. \n";
	}
	else
	{
		m_rDriverContext.getLogManager() << LogLevel_Info << "Eye blink detection NOT possible. Please use MindSet Dev. Tools v2.1 or sup. \n";
	}

	// if no com port was selected with the property dialog window
	if(m_ui32ComPort == OVAS_MINDSET_INVALID_COM_PORT) 
	{
		// try the com ports. @NOTE almost duplicate code in CConfigurationMindSet
		m_rDriverContext.getLogManager() << LogLevel_Info << "Scanning COM ports 1 to 16...\n";
		for(uint32 i=1 ; i<16 && m_ui32ComPort == OVAS_MINDSET_INVALID_COM_PORT; i++)
		{
			/* Get a new connection ID handle to ThinkGear API */
			int l_iConnectionId = TG_GetNewConnectionId();
			if( l_iConnectionId < 0 ) {
				m_rDriverContext.getLogManager() << LogLevel_Error << "The driver was unable to connect to the ThinkGear Communication Driver.\n";	
				return false;
			}
			m_rDriverContext.getLogManager() << LogLevel_Trace << "ThinkGear Connection ID is: "<< l_iConnectionId <<".\n";

			/* Attempt to connect the connection ID handle to serial port */
			stringstream l_ssComPortName;
			l_ssComPortName << "\\\\.\\COM" << i;
			m_rDriverContext.getLogManager() << LogLevel_Trace << "Trying port [" << l_ssComPortName.str().c_str() << "]\n";
			int l_iErrCode = TG_Connect(l_iConnectionId,l_ssComPortName.str().c_str(),TG_BAUD_9600,TG_STREAM_PACKETS );
			if( l_iErrCode >= 0 ) 
			{
				m_rDriverContext.getLogManager() << LogLevel_Info << "Connection available on port " << l_ssComPortName.str().c_str();
		
				const uint32 l_ui32StartTime = System::Time::getTime();
				const uint32 l_ui32TimeToTry = 3000; // ms

				// With e.g. MindWave Mobile, errors do not mean that the operation couldn't succeed in the future, so we ask for a packet optimistically for a while.
				bool l_bComPortFound = false;
				while(!l_bComPortFound && (System::Time::getTime()-l_ui32StartTime) < l_ui32TimeToTry) 
				{	
					//we try to read one packet to check the connection.
					l_iErrCode = TG_ReadPackets( l_iConnectionId, 1 );
					if(l_iErrCode >= 0)
					{	
						m_rDriverContext.getLogManager() << " - Status: OK\n";
						m_ui32ComPort = i;
						l_bComPortFound = true;
					} else {
						System::Time::sleep(1);
					}
				}
				if(!l_bComPortFound) {
					m_rDriverContext.getLogManager() << " - Tried for " << l_ui32TimeToTry/1000 << " seconds, gave up.\n";
					if(l_iErrCode == -1) 
						m_rDriverContext.getLogManager() << LogLevel_Info << "  Last TG_ReadPackets error: -1, Invalid connection ID\n";
					else if(l_iErrCode == -2)
						m_rDriverContext.getLogManager() << LogLevel_Info << "  Last TG_ReadPackets error: -2, 0 bytes on the stream\n";
					else if(l_iErrCode == -3) 
						m_rDriverContext.getLogManager() << LogLevel_Info << "  Last TG_ReadPackets error: -3, I/O error occured\n";
					else 
						m_rDriverContext.getLogManager() << LogLevel_Info << "  Last TG_ReadPackets error: " << l_iErrCode << ", Unknown\n";
				}
			} else {
				m_rDriverContext.getLogManager() << LogLevel_Trace << "TG_Connect() returned error " << l_iErrCode << "\n";
			}
			// free the connection to ThinkGear API
			// We use FreeConnection() only as doing a TG_Disconnect()+TG_FreeConnection() pair can cause first-chance exceptions on visual studio & MindWave Mobile for some reason.
			TG_FreeConnection(l_iConnectionId);
		}
	}

	if(m_ui32ComPort == OVAS_MINDSET_INVALID_COM_PORT)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "The driver was unable to find any valid device on serial port COM1 to COM16.\n";
		return false;
	}

	//__________________________________
	// Saves parameters
	m_pCallback=&rCallback;
	m_ui32SampleCountPerSentBlock=ui32SampleCountPerSentBlock;

	return true;
}

boolean CDriverNeuroskyMindset::start(void)
{
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Mindset Driver: START called.\n";
	if(!m_rDriverContext.isConnected())
	{
		return false;
	}

	if(m_rDriverContext.isStarted())
	{
		return false;
	}

	if(m_ui32ComPort == OVAS_MINDSET_INVALID_COM_PORT)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "No valid Serial COM port detected.\n";
		return false;
	}

	int   l_iErrCode = 0;

	/* Get a connection ID handle to ThinkGear */
	m_i32ConnectionID = TG_GetNewConnectionId();
	if( m_i32ConnectionID < 0 ) 
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "Can't connect to ThinkGear Communication Driver (error code "<< m_i32ConnectionID <<").\n";
		return false;
	}

	m_rDriverContext.getLogManager() << LogLevel_Info << "ThinkGear Communication ID is: "<< m_i32ConnectionID <<".\n";

	/* Attempt to connect the connection ID handle to serial port */
	stringstream l_ssComPortName;
	l_ssComPortName << "\\\\.\\COM" << m_ui32ComPort;
	m_rDriverContext.getLogManager() << LogLevel_Info << "Trying to connect ThinkGear driver to Serial Port [" << l_ssComPortName.str().c_str() << "]\n";
	l_iErrCode = TG_Connect(m_i32ConnectionID,l_ssComPortName.str().c_str(),TG_BAUD_9600,TG_STREAM_PACKETS );
	if( l_iErrCode < 0 ) 
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "The ThinkGear driver was unable to connect to serial port [" << l_ssComPortName.str().c_str() << "] (error code "<<l_iErrCode<<").\n";
		return false;
	}

	if(m_bBlinkStimulations || m_bBlinkStrenghtChannel)
	{
		TG_EnableBlinkDetection(m_i32ConnectionID, 1);
	}

	return true;
}

boolean CDriverNeuroskyMindset::loop(void)
{
	if(!m_rDriverContext.isConnected())
	{
		return false;
	}

	if(m_rDriverContext.isStarted())
	{
		
		uint32 l_ui32ReceivedSamples=0;
		uint32 l_ui32ErrorCount=0;

		CStimulationSet l_oStimulationSet;

		while(l_ui32ReceivedSamples < m_ui32SampleCountPerSentBlock)
		{
			/* Attempt to read 1 Packet of data from the connection (numPackets = -1 means all packets) */
			int32 l_i32ErrorCode = TG_ReadPackets( m_i32ConnectionID, 1 );
			
			if(l_i32ErrorCode == 1)// we asked for 1 packet and received 1
			{
				// Got a packet, reset the error count
				l_ui32ErrorCount = 0;

				/* If raw value has been updated by TG_ReadPackets()... */
				if( TG_GetValueStatus(m_i32ConnectionID, TG_DATA_RAW ) != 0 )
				{
					float32 raw_value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_RAW);
					m_pSample[l_ui32ReceivedSamples] = raw_value;
					l_ui32ReceivedSamples++;
				}

				//checking the signal quality
				//if it has been updated...
				if( TG_GetValueStatus(m_i32ConnectionID, TG_DATA_POOR_SIGNAL ) != 0 )
				{
					float32 signal_quality = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_POOR_SIGNAL);

					// Special warning for value 200 (no contact with electrode)
					// Noise warning after 25% contamination.
					if(signal_quality == 200)
					{
						m_rDriverContext.getLogManager() 
							<< LogLevel_Warning 
							<< "("<<++m_ui32WarningCount
							<<") Poor Signal detected (electrode not in contact with the forehead)\n";
					}
					else if(signal_quality > 25)
					{
						m_rDriverContext.getLogManager() 
							<< LogLevel_Warning 
							<< "("<<++m_ui32WarningCount
							<<") Poor Signal detected (noise at "<< (1-(signal_quality/200.0f))*100 << "%)\n";
					}
					else 
					{
						if(m_ui32WarningCount != 0)
							m_rDriverContext.getLogManager()
								<< LogLevel_Info
								<< "Signal Quality acceptable - noise < 12.5%\n";
						m_ui32WarningCount = 0;
					}
				}

				float32 l_f32Value;
				uint32 l_ui32ChannelIndex = 1;
				if(m_bESenseChannels)
				{
					// we don't check if the value has changed, we construct a square signal (1Hz --> 512Hz)

					l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_ATTENTION);
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
					l_ui32ChannelIndex++;

					l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_MEDITATION);
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
					l_ui32ChannelIndex++;
				}
				if(m_bBandPowerChannels)
				{
					l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_DELTA);
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
					l_ui32ChannelIndex++;

					l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_THETA);
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
					l_ui32ChannelIndex++;

					l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_ALPHA1);
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
					l_ui32ChannelIndex++;

					l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_ALPHA2);
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
					l_ui32ChannelIndex++;

					l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_BETA1);
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
					l_ui32ChannelIndex++;

					l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_BETA2);
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
					l_ui32ChannelIndex++;

					l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_GAMMA1);
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
					l_ui32ChannelIndex++;

					l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_GAMMA2);
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
					l_ui32ChannelIndex++;
				}

				boolean l_bBlinkDetected = false;
				// We construct a "blink" spike signal if requested.
				if(m_bBlinkStrenghtChannel)
				{
					if(TG_GetValueStatus(m_i32ConnectionID,TG_DATA_BLINK_STRENGTH) != 0)
					{
						l_f32Value = (float32) TG_GetValue(m_i32ConnectionID, TG_DATA_BLINK_STRENGTH);
						l_bBlinkDetected = true;
					}
					else
					{
						l_f32Value = 0;
					}
					m_pSample[l_ui32ChannelIndex * m_ui32SampleCountPerSentBlock + l_ui32ReceivedSamples-1] = l_f32Value;
				}
				// We send a "blink" stimulation if requested.
				if(m_bBlinkStimulations)
				{
					if(TG_GetValueStatus(m_i32ConnectionID,TG_DATA_BLINK_STRENGTH) != 0 || l_bBlinkDetected)
					{
						uint64 l_ui64StimulationTime = ITimeArithmetics::sampleCountToTime(m_oHeader.getSamplingFrequency(), l_ui32ReceivedSamples);

						l_oStimulationSet.appendStimulation(OVTK_GDF_Eye_Blink, l_ui64StimulationTime,0);
					}
				}
			}
			else
			{
				// Received something else than 1 packet. This is not necessarily an unrecoverable error. Try for a while.
				// @note this is using counting to avoid polling the clock constantly
				l_ui32ErrorCount++;
				
				const uint32 l_ui32ErrorSleep = 2; // In ms
				const uint32 l_ui32ErrorTolerance = 1000; // As each error sleeps 2 ms, 1000*2 = 2000ms
				if(l_ui32ErrorCount > l_ui32ErrorTolerance) 
				{
					m_rDriverContext.getLogManager() << LogLevel_Warning << "No valid packet from ThinkGear for a while, returning prematurely.\n";
					return true;	// Let the acquisition server decide what to do, don't return a failure. It'll timeout after a bit.
				}

				System::Time::sleep(l_ui32ErrorSleep);
			}
		}

		m_pCallback->setSamples(m_pSample);
		m_rDriverContext.correctDriftSampleCount(m_rDriverContext.getSuggestedDriftCorrectionSampleCount());
		m_pCallback->setStimulationSet(l_oStimulationSet);

	}

	return true;
}

boolean CDriverNeuroskyMindset::stop(void)
{
	m_rDriverContext.getLogManager() << LogLevel_Trace << "Mindset Driver: STOP called.\n";
	if(!m_rDriverContext.isConnected())
	{
		return false;
	}

	if(!m_rDriverContext.isStarted())
	{
		return false;
	}

	if(m_i32ConnectionID>=0) 
	{
		// We use FreeConnection() only as doing a TG_Disconnect()+TG_FreeConnection() pair can cause first-chance exceptions on visual studio & MindWave Mobile for some reason.
		TG_FreeConnection(m_i32ConnectionID);
		m_i32ConnectionID = -1;
	}

	return true;
}

boolean CDriverNeuroskyMindset::uninitialize(void)
{
	if(!m_rDriverContext.isConnected())
	{
		return false;
	}

	if(m_rDriverContext.isStarted())
	{
		return false;
	}


	if(m_pSample)
	{
		delete [] m_pSample;
		m_pSample=NULL;
	}
	m_pCallback=NULL;

	return true;
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverNeuroskyMindset::isConfigurable(void)
{
	return true;
}

boolean CDriverNeuroskyMindset::configure(void)
{
	CConfigurationNeuroskyMindset m_oConfiguration(m_rDriverContext, OpenViBE::Directories::getDataDir() + "/openvibe-applications/acquisition-server/interface-Neurosky-Mindset.ui"
		,m_ui32ComPort
		,m_bESenseChannels
		,m_bBandPowerChannels
		,m_bBlinkStimulations
		,m_bBlinkStrenghtChannel);

	if(!m_oConfiguration.configure(m_oHeader)) // the basic configure will use the basic header
	{
		return false;
	}

	return true;
}

#endif // TARGET_HAS_ThirdPartyThinkGearAPI
