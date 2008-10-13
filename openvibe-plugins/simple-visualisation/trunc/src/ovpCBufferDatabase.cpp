#include "ovpCBufferDatabase.h"

#include <system/Memory.h>

#include <algorithm>

#include <math.h>

using namespace OpenViBE;
using namespace OpenViBE::Plugins;
using namespace OpenViBE::Kernel;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{	
#ifdef ELAN_VALIDATION
		static const unsigned int s_nbChannels = 143;

		static string s_channelLabels[s_nbChannels] = {
		"LCa",
		"LCb",
		"LCc",
		"LCd",
		"LCe",
		"LCf",
		"LCk",
		"LCl",
		"LCm",
		"LCn",
		"LCo",
		"LCp",
		"LCq",
		"LCr",
		"LCu",
		"LCv",
		"LCw",
		"LCx",
		"LCy",
		"LCz",
		"LCA",
		"LCE",
		"LCF",
		"LCG",
		"LFa",
		"LFb",
		"LFc",
		"LFd",
		"LFe",
		"LFf",
		"LFk",
		"LFl",
		"LFm",
		"LFn",
		"LFo",
		"LFp",
		"LFu",
		"LFv",
		"LFw",
		"LFx",
		"LFy",
		"LFE",
		"LFF",
		"LFG",
		"LFH",
		"LFI",
		"LPa",
		"LPb",
		"LPc",
		"LPd",
		"LPe",
		"LPk",
		"LPl",
		"LPm",
		"LPn",
		"LPo",
		"LPp",
		"LPq",
		"LPu",
		"LPv",
		"LPw",
		"LPx",
		"LPE",
		"LPF",
		"LPG",
		"LPH",
		"LPI",
		"MC1",
		"MC2",
		"MC3",
		"MC4",
		"MC5",
		"MF1",
		"MF2",
		"MP1",
		"MP2",
		"RCa",
		"RCb",
		"RCc",
		"RCd",
		"RCe",
		"RCf",
		"RCk",
		"RCl",
		"RCm",
		"RCn",
		"RCo",
		"RCp",
		"RCq",
		"RCr",
		"RCu",
		"RCv",
		"RCw",
		"RCx",
		"RCy",
		"RCz",
		"RCA",
		"RCE",
		"RCF",
		"RCG",
		"RFa",
		"RFb",
		"RFc",
		"RFd",
		"RFe",
		"RFf",
		"RFk",
		"RFl",
		"RFm",
		"RFn",
		"RFo",
		"RFp",
		"RFu",
		"RFv",
		"RFw",
		"RFx",
		"RFy",
		"RFE",
		"RFF",
		"RFG",
		"RFH",
		"RFI",
		"RPa",
		"RPb",
		"RPc",
		"RPd",
		"RPe",
		"RPk",
		"RPl",
		"RPm",
		"RPn",
		"RPo",
		"RPp",
		"RPu",
		"RPv",
		"RPw",
		"RPx",
		"RPy",
		"RPE",
		"RPF",
		"RPG",
		"RPH",
		"RPI"
		};

		static float64 s_channelCoords[2*s_nbChannels] = {
		61.099998,38.599998,
		55.799999,23.600000,
		62.099998,7.700000,
		54.500000,352.000000,
		50.000000,333.600006,
		49.400002,314.100006,
		58.099998,72.300003,
		53.799999,54.900002,
		45.099998,36.900002,
		45.700001,11.100000,
		39.700001,352.100006,
		35.599998,330.700012,
		37.099998,308.299988,
		46.400002,291.799988,
		45.200001,76.699997,
		38.400002,59.799999,
		30.700001,39.299999,
		31.600000,13.500000,
		24.200001,347.299988,
		23.400000,310.700012,
		32.599998,286.299988,
		20.200001,61.500000,
		16.400000,20.700001,
		11.100000,320.500000,
		104.699997,76.199997,
		105.099998,61.299999,
		108.599998,46.299999,
		115.900002,31.200001,
		123.900002,15.900000,
		118.099998,358.399994,
		92.900002,82.000000,
		92.599998,67.000000,
		93.400002,51.700001,
		96.199997,38.200001,
		103.699997,25.100000,
		110.900002,11.100000,
		81.199997,74.300003,
		81.300003,58.099998,
		80.099998,39.799999,
		86.699997,24.500000,
		94.599998,9.100000,
		69.500000,81.199997,
		69.099998,64.900002,
		69.500000,50.200001,
		71.000000,24.900000,
		77.800003,8.900000,
		111.800003,343.399994,
		105.400002,327.600006,
		101.099998,312.200012,
		97.400002,297.899994,
		95.599998,283.500000,
		103.699997,355.100006,
		96.900002,340.600006,
		92.099998,324.100006,
		87.300003,308.899994,
		84.199997,293.799988,
		82.500000,278.799988,
		87.699997,354.100006,
		81.000000,338.000000,
		76.900002,321.600006,
		72.800003,305.100006,
		70.599998,287.899994,
		70.500000,352.299988,
		64.599998,336.000000,
		62.500000,318.799988,
		59.099998,299.799988,
		57.299999,280.799988,
		57.000000,90.000000,
		31.299999,89.800003,
		5.400000,87.500000,
		19.400000,270.799988,
		44.200001,270.500000,
		104.599998,90.099998,
		81.199997,90.099998,
		94.900002,270.200012,
		69.699997,270.299988,
		60.799999,141.699997,
		55.500000,156.800003,
		61.700001,172.899994,
		54.299999,188.500000,
		49.700001,206.899994,
		49.099998,226.500000,
		57.900002,107.699997,
		53.500000,125.099998,
		44.599998,143.199997,
		45.299999,169.399994,
		39.400002,188.399994,
		35.400002,210.000000,
		36.700001,232.500000,
		46.200001,249.100006,
		45.000000,103.099998,
		38.099998,120.000000,
		30.299999,140.699997,
		31.200001,166.800003,
		23.799999,193.300003,
		23.100000,230.800003,
		32.500000,254.699997,
		19.900000,117.699997,
		16.000000,158.899994,
		10.800000,220.800003,
		104.699997,104.000000,
		105.000000,118.900002,
		108.500000,134.100006,
		115.800003,149.199997,
		123.900002,164.800003,
		117.699997,182.100006,
		92.800003,98.199997,
		92.500000,113.099998,
		93.400002,128.600006,
		96.000000,142.100006,
		103.500000,155.199997,
		110.000000,169.600006,
		81.099998,106.000000,
		81.000000,122.099998,
		79.699997,140.699997,
		86.199997,155.899994,
		94.300003,171.399994,
		69.300003,99.000000,
		68.800003,115.199997,
		69.199997,130.000000,
		70.699997,155.500000,
		77.400002,171.600006,
		111.500000,197.300003,
		105.199997,213.000000,
		100.900002,228.300003,
		97.400002,242.600006,
		95.500000,256.799988,
		103.300003,185.399994,
		96.500000,199.899994,
		91.599998,216.399994,
		87.000000,231.600006,
		84.000000,246.600006,
		82.500000,261.600006,
		87.300003,186.500000,
		80.500000,202.699997,
		76.500000,218.899994,
		72.599998,235.500000,
		70.400002,252.699997,
		70.099998,188.199997,
		64.300003,204.399994,
		62.099998,221.699997,
		58.900002,240.899994,
		57.200001,259.899994
		};

		static float64 s_channelValues[s_nbChannels] = {
		0.836,
		1.255,
		1.686,
		1.985,
		1.960,
		1.857,
		0.319,
		0.613,
		1.073,
		1.537,
		1.671,
		1.639,
		1.468,
		1.232,
		0.306,
		0.687,
		1.071,
		1.360,
		1.295,
		1.024,
		0.612,
		0.493,
		0.870,
		0.463,
		0.158,
		0.230,
		0.268,
		0.304,
		0.291,
		0.382,
		0.111,
		0.225,
		0.325,
		0.495,
		0.671,
		0.580,
		0.189,
		0.312,
		0.612,
		0.992,
		1.029,
		0.146,
		0.319,
		0.494,
		1.110,
		1.628,
		0.500,
		0.563,
		0.549,
		0.400,
		0.260,
		0.828,
		1.007,
		1.314,
		1.152,
		0.827,
		0.429,
		1.405,
		1.686,
		2.042,
		1.666,
		0.995,
		2.255,
		2.317,
		2.136,
		1.631,
		0.698,
		0.008,
		-0.104,
		-0.228,
		-0.278,
		-0.160,
		0.072,
		0.034,
		0.082,
		0.068,
		-0.929,
		-1.447,
		-1.992,
		-2.336,
		-2.295,
		-2.237,
		-0.307,
		-0.658,
		-1.246,
		-1.864,
		-2.012,
		-2.001,
		-1.882,
		-1.570,
		-0.359,
		-0.852,
		-1.398,
		-1.714,
		-1.658,
		-1.435,
		-1.103,
		-0.842,
		-1.274,
		-0.981,
		-0.035,
		-0.123,
		-0.154,
		-0.155,
		-0.136,
		-0.218,
		-0.024,
		-0.159,
		-0.280,
		-0.429,
		-0.543,
		-0.428,
		-0.146,
		-0.291,
		-0.638,
		-1.024,
		-0.958,
		-0.112,
		-0.292,
		-0.498,
		-1.228,
		-1.765,
		-0.307,
		-0.342,
		-0.316,
		-0.212,
		-0.097,
		-0.648,
		-0.842,
		-1.103,
		-0.945,
		-0.633,
		-0.214,
		-1.397,
		-1.801,
		-2.080,
		-1.665,
		-0.927,
		-2.535,
		-2.638,
		-2.417,
		-1.871,
		-0.791
		};

#else //Normal mode : set of 72 'standard' electrodes

		//TODO : retrieve this info from electrode localisation stream
		static const unsigned int s_nbChannels = 72;// + 23;

		static string s_channelLabels[s_nbChannels] = {
			//from INSERM's elec.dat
			"O1",
			"O2",
			"T5",
			"P3",
			"Pz",
			"P4",
			"T6",
			"T3",
			"C3",
			"Cz",
			"C4",
			"T4",
			"F7",
			"F3",
 			"Fz",
 			"F4",
 			"F8",
			"Fp1",
 			"Fp2",
			"P5",
			"P1",
			"P2",
			"P6",
			"C5",
			"C1",
			"C2",
			"C6",
			"F5",
			"F1",
			"F2",
			"F6",
			"M1",
			"M2",
			"Oz",
			"POz",
			"CPz",
			"FCz",
			"FFz",
			"Fpz",
			"FC1",
			"FC2",
			"CP1",
			"CP2",
			"FC3",
			"FC4",
			"CP3",
			"CP4",
			"FT3",
			"FT4",
			"TP3",
			"TP4",
			"P'1",
			"P'2",
			"Nz",
			"I1",
			"Iz",
			"I2",
			"FF3",
			"TP5",
			"OT5",
			"IT1",
			"IT2",
			"IP1",
			"IPz",
			"IP2",
			"M'1",
			"M'2",
			"TP7",
			"TP8",
			"Nez",
			"F3z",
			"F4z",
#if 0
			//23 channels from http://www.biosemi.com/download/Cap_coords_all.xls
			"AF3",
			"AF4",
			"AF7",
			"AF8",
			"AFz",
			"CP5",
			"CP6",
			"F9",
			"F10",
			"FC5",
			"FC6",
			"FT7",
			"FT8",
			/*"FT9", x*/
			/*"FT10",x*/
			/*"O9",  x*/
			/*"O10"  x*/
			"P7",
			"P8",
			"P9",
			"P10",
			"PO3",
			"PO4",
			"PO7",
			"PO8",
			/*"PO9", x*/
			/*"PO10",x*/
			"T7",
			"T8"
			/*"TP9", x*/
			/*"TP10",x*/
#endif
		};

		static float64 s_channelCoords[s_nbChannels*2] = {
			//72 channels from INSERM's elec.dat
/*"O1"*/			90.0, 252.0,
/*"O2"*/ 			90.0, 288.0,
/*"T5"*/ 			90.0, 216.0,
/*"P3"*/ 			63.1, 223.1,
/*"Pz"*/ 			45.0, 270.0,
/*"P4"*/ 			63.1, 316.9,
/*"T6"*/ 			90.0, 324.0,
/*"T3"*/ 			90.0, 180.0,
/*"C3"*/ 			45.0, 180.0,
/*"Cz"*/ 			0.0,  0.0,
/*"C4"*/			45.0, 0.0,
/*"T4"*/			90.0,	0.0,
/*"F7"*/			90.0, 144.0,
/*"F3"*/			63.1, 136.9,
/*"Fz"*/			45.0, 90.0,
/*"F4"*/			63.1, 43.1,
/*"F8"*/			90.0, 36.0,
/*"Fp1"*/			90.0, 108.0,
/*"Fp2"*/			90.0, 72.0,
/*"P5"*/			76.5, 219.3,
/*"P1"*/			51.7, 243.7,
/*"P2"*/ 			51.7, 296.3,
/*"P6"*/ 			76.5, 320.7,
/*"C5"*/ 			67.5, 180.0,
/*"C1"*/ 			22.5, 180.0,
/*"C2"*/ 			22.5, 0.0,
/*"C6"*/ 			67.5, 0.0,
/*"F5"*/ 			76.5, 140.7,
/*"F1"*/ 			51.7, 116.3,
/*"F2"*/ 			51.7, 63.7,
/*"F6"*/ 			76.5, 39.3,
/*"M1"*/ 			120.0, 215.6,
/*"M2"*/ 			120.0, 324.4,
/*"Oz"*/ 			90.0, 270.0,
/*"POz"*/			67.5, 270.0,
/*"CPz"*/			22.5, 270.0,
/*"FCz"*/			22.5, 90.0,
/*"FFz"*/			67.5, 90.0,
/*"Fpz"*/			90.0, 90.0,
/*"FC1"*/			31.5, 136.9,
/*"FC2"*/			31.5, 43.1,
/*"CP1"*/			31.5, 223.1,
/*"CP2"*/			31.5, 316.9,
/*"FC3"*/			52.1, 155.8,
/*"FC4"*/			52.1, 24.2,
/*"CP3"*/			52.1, 204.2,
/*"CP4"*/			52.1, 335.8,
/*"FT3"*/			75.6, 159.7,
/*"FT4"*/			75.6, 20.3,
/*"TP3"*/			75.6, 200.3,
/*"TP4"*/			75.6, 339.7,
/*"P'1"*/			52.0, 241.3,
/*"P'2"*/			52.0, 298.7,
/*"Nz"*/ 			112.5, 90.0,
/*"I1"*/ 			120.0, 234.4,
/*"Iz"*/ 			112.5, 270.0,
/*"I2"*/			120.0, 305.6,
/*"FF3"*/			33.2, 135.6,
/*"TP5"*/			66.5, 201.2,
/*"OT5"*/			77.4, 241.8,
/*"IT1"*/			102.6, 241.8,
/*"IT2"*/			102.6, 298.2,
/*"IP1"*/			87.6, 247.0,
/*"IPz"*/			78.8, 270.0,
/*"IP2"*/			87.6, 293.0,
/*"M'1"*/			105.4, 203.0,
/*"M'2"*/			105.4, 337.0,
/*"TP7"*/			90.0, 198.0,
/*"TP8"*/			90.0, 342.0,
/*"Nez"*/			118.1, 90.0,
/*"F3z"*/			63.4, 113.0,
/*"F4z"*/			63.4, 67.0//,
#if 0
//23 channels from http://www.biosemi.com/download/Cap_coords_all.xls

/*"AF3"  -74, -65,*/ 74, 115,
/*"AF4"  74, 65, */ 74, 65,
/*"AF7" -92, -54,*/ 92, 126,
/*"AF8" 92, 54,*/ 92, 54,
/*"AFz" 69, 90,*/ 69, 90,
/*"CP5" -72, 21,*/ 72, 21,
/*"CP6" 72, -21,*/ 72, 159,
/*"F9" -82.8, -56.25,*/ 82.8, 123.75,
/*"F10"-92, -56.25*/ 92, 123.75,
/*"FC5" -72, -21,*/ 72, 159,
/*"FC6" 72, 21,*/ 72, 21,
/*"FT7" -92, -18,*/ 92, 162,
/*"FT8" 92, 18,*/ 92, 18,
/*"FT9", x*/
/*"FT10",x*/
/*"O9",  x*/
/*"O10"  x*/
/*"P7" -92, 36, */ 92, 36,
/*"P8"  92, -36,*/ 92, 144,
/*"P9" -115, 40*/ 115, 40,
/*"P10" 115, -40,*/ 115, 120,
/*"PO3" -74, 65,*/ 74, 65,
/*"PO4" 74, -65,*/ 74, 115,
/*"PO7" -92, 54,*/ 92, 54,
/*"PO8" 92, -54,*/ 92, 126,
/*"PO9", x*/
/*"PO10",x*/
/*"T7" -92, 0*/ 92, 180,
/*"T8" 92, 0*/ 92, 0
/*"TP9", x*/
/*"TP10",x*/
#endif
		};
#endif

			CBufferDatabase::CBufferDatabase(OpenViBEToolkit::TBoxAlgorithm<Plugins::IBoxAlgorithm>& oPlugin)
			:
			m_i64NbElectrodes(0)
			,m_bFirstBufferReceived(false)
			,m_ui32SamplingFrequency(0)
			,m_ui64NumberOfBufferToDisplay(2)
			,m_f64MaximumValue(-DBL_MAX)
			,m_f64MinimumValue(+DBL_MAX)
			,m_f64TotalDuration(10000) // 10 seconds
			,m_ui64BufferDuration(0)
			,m_pDrawable(NULL)
			,m_oParentPlugin(oPlugin)
			,m_bError(false)
			,m_bRedrawOnNewData(true)
		{
			m_pDimmensionSizes[0] = m_pDimmensionSizes[1] = 0;
		}

		CBufferDatabase::~CBufferDatabase()
		{			
			//delete all the remaining buffers
			while(m_oSampleBuffers.size() > 0)
			{
				delete[] m_oSampleBuffers.front();
				m_oSampleBuffers.pop_front();
			}
		}

		boolean CBufferDatabase::isFirstBufferReceived()
		{
			return m_bFirstBufferReceived;
		}

		boolean CBufferDatabase::adjustNumberOfDisplayedBuffers(float64 f64NumberOfSecondsToDisplay)
		{
			boolean l_bNumberOfBufferToDisplayChanged = false;

			if(f64NumberOfSecondsToDisplay>0)
			{
				m_f64TotalDuration=f64NumberOfSecondsToDisplay;
			}

			uint64 l_ui64NewNumberOfBufferToDisplay =  static_cast<uint64>(ceil( (m_f64TotalDuration*m_ui32SamplingFrequency) / m_pDimmensionSizes[1]));

			//displays at least one buffer
			l_ui64NewNumberOfBufferToDisplay = (l_ui64NewNumberOfBufferToDisplay == 0) ? 1 : l_ui64NewNumberOfBufferToDisplay;
			if(l_ui64NewNumberOfBufferToDisplay != m_ui64NumberOfBufferToDisplay || f64NumberOfSecondsToDisplay<=0)
			{
				m_ui64NumberOfBufferToDisplay = l_ui64NewNumberOfBufferToDisplay;
				l_bNumberOfBufferToDisplayChanged = true;

				//if the new number of buffer to display is greater than the previous one, destroy the useless buffers
				while(m_ui64NumberOfBufferToDisplay < m_oSampleBuffers.size())
				{
					delete[] m_oSampleBuffers.front();
					m_oSampleBuffers.pop_front();

					//suppress the corresponding starting time from the list
					m_oStartTime.pop_front();
					m_oEndTime.pop_front();

					//suppress the corresponding minmax values
					for(uint32 c=0 ; c<m_pDimmensionSizes[0] ; c++)
					{
						m_oLocalMinMaxValue[c].pop_front();
					}
				}
			}

			return l_bNumberOfBufferToDisplayChanged;
		}

		uint64 CBufferDatabase::getChannelCount()
		{
			return m_pDimmensionSizes[0];
		}

		float64 CBufferDatabase::getDisplayedTimeIntervalWidth()
		{
			return (m_ui64NumberOfBufferToDisplay * ((m_pDimmensionSizes[1]*1000.0) / m_ui32SamplingFrequency));
		}

		void CBufferDatabase::setMatrixDimmensionCount(const uint32 ui32DimmensionCount)
		{
			if(ui32DimmensionCount != 2)
			{

				m_oParentPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning << "Error dimmension count isn't 2!\n";
			}
		}

		void CBufferDatabase::setMatrixDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
		{
#ifdef ELAN_VALIDATION
			if(ui32DimmensionIndex == 0)
			{
				*(uint32*)&ui32DimmensionSize = s_nbChannels;				
			}
#endif
			m_pDimmensionSizes[ui32DimmensionIndex] = ui32DimmensionSize;			
			m_pDimmesionLabels[ui32DimmensionIndex].resize(m_pDimmensionSizes[ui32DimmensionIndex]);

			if(ui32DimmensionIndex == 0)
			{					
				m_i64NbElectrodes = m_pDimmensionSizes[ui32DimmensionIndex];

				//resize min/max values vector
				m_oLocalMinMaxValue.resize((unsigned int)m_i64NbElectrodes);

				//resize electrode coords matrix
				m_oElectrodesCoords.setDimensionCount(1);
				m_oElectrodesCoords.setDimensionSize(0, (uint32)(3*m_i64NbElectrodes));

				//resize electrode spherical coords matrix
				m_oElectrodesSphericalCoords.setDimensionCount(1);
				m_oElectrodesSphericalCoords.setDimensionSize(0, (uint32)(2*m_i64NbElectrodes));

				//resize electrode labels vector
				m_oElectrodesLabels.resize((uint32)m_i64NbElectrodes);
			}			
		}

		void CBufferDatabase::setMatrixDimmensionLabel(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
		{
#ifdef ELAN_VALIDATION
			//done upon first buffer reception in setMatrixBuffer	
#else
			m_pDimmesionLabels[ui32DimmensionIndex][ui32DimmensionEntryIndex] = sDimmensionLabel;
#endif
		}

		void CBufferDatabase::setMatrixBuffer(const float64* pBuffer, uint64 ui64StartTime, uint64 ui64EndTime)
		{
			//if an error has occured, do nothing
			if(m_bError)
			{
				return;
			}

			uint64 l_ui64NumberOfSamplesPerBuffer = m_pDimmensionSizes[0] * m_pDimmensionSizes[1];

			if(m_bFirstBufferReceived == false)
			{				
#ifdef ELAN_VALIDATION
				for(uint32 i=0; i<s_nbChannels; i++)
				{
					m_pDimmesionLabels[0][i] = s_channelLabels[i];
				}
#endif

				//make sure channel coordinates are filled
				//TODO : add coordinates info to streamed matrix stream header!!
				computeChannelCoords();

				m_ui64BufferDuration = ui64EndTime - ui64StartTime;

				//test if it is equal to zero : Error
				if(m_ui64BufferDuration == 0)
				{
					m_oParentPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Warning << "Error : buffer start time and end time are equal : " << ui64StartTime << "\n";

					m_bError = true;
					return;
				}

				//computes the sampling frequency
				m_ui32SamplingFrequency = (uint32)((float64)(((uint64)1<<32)/(m_ui64BufferDuration)) * m_pDimmensionSizes[1]);

				//computes the number of buffer necessary to display the interval
				adjustNumberOfDisplayedBuffers(-1);

				m_pDrawable->init();

				m_bFirstBufferReceived = true;
			}
#ifdef ELAN_VALIDATION
			return;			
#endif
			float64 * l_pBufferToWrite;

			//Adds the starting time to the list
			m_oStartTime.push_back(ui64StartTime);
			m_oEndTime.push_back(ui64EndTime);

			if(m_ui64NumberOfBufferToDisplay - m_oSampleBuffers.size() > 0)
			{
				////If the number of buffer to display has changed and is greater than before, create a new buffer
				l_pBufferToWrite = new float64[(size_t)l_ui64NumberOfSamplesPerBuffer];
			}
			else if (m_ui64NumberOfBufferToDisplay == m_oSampleBuffers.size())
			{
				//If it is the same than before, get the first buffer and removes it from the list
				l_pBufferToWrite = m_oSampleBuffers.front();
				m_oSampleBuffers.pop_front();

				//remove the corresponding starting time
				m_oStartTime.pop_front();
				m_oEndTime.pop_front();

				//removes the corresponding minmax values
				for(uint32 c=0 ; c<m_pDimmensionSizes[0] ; c++)
				{
					m_oLocalMinMaxValue[c].pop_front();
				}
			}

			uint64 l_ui64CurrentSample = 0;
			//for each channel
			for(uint32 c=0 ; c<m_pDimmensionSizes[0] ; c++)
			{
				float64 l_f64LocalMin = DBL_MAX;
				float64 l_f64LocalMax = -DBL_MAX;

				//for each sample
				for(uint64 i=0 ; i<m_pDimmensionSizes[1] ; i++, l_ui64CurrentSample++)
				{
					//get channel local min/max
					if(pBuffer[l_ui64CurrentSample] < l_f64LocalMin)
					{
						l_f64LocalMin = pBuffer[l_ui64CurrentSample];
					}
					if(pBuffer[l_ui64CurrentSample] > l_f64LocalMax)
					{
						l_f64LocalMax = pBuffer[l_ui64CurrentSample];
					}
				}

				//adds the minmax pair to the corresponding channel's list
				m_oLocalMinMaxValue[c].push_back(pair<float64, float64>(l_f64LocalMin, l_f64LocalMax));

				if(l_f64LocalMax > m_f64MaximumValue)
				{
					m_f64MaximumValue = l_f64LocalMax;
				}
				if(l_f64LocalMin < m_f64MinimumValue)
				{
					m_f64MinimumValue = l_f64LocalMin;
				}

			}

			//copy the content of pBuffer into the selected buffer
			System::Memory::copy(l_pBufferToWrite,
					pBuffer,
					l_ui64NumberOfSamplesPerBuffer*sizeof(float64));

			//add the buffer at the end of the list
			m_oSampleBuffers.push_back(l_pBufferToWrite);

			//tells the drawable to redraw himself since the signal information has been updated
			if(m_bRedrawOnNewData)
			{
				m_pDrawable->redraw();
			}
		}

		void CBufferDatabase::getDisplayedChannelLocalMinMaxValue(uint32 ui32Channel, float64& f64Min, float64& f64Max)
		{
			f64Min = +DBL_MAX;
			f64Max = -DBL_MAX;

			for(uint64 i=0 ; i<m_oLocalMinMaxValue[(size_t)ui32Channel].size() ; i++)
			{
				if(f64Min > m_oLocalMinMaxValue[(size_t)ui32Channel][(size_t)i].first)
				{
					f64Min = m_oLocalMinMaxValue[(size_t)ui32Channel][(size_t)i].first;
				}
				if(f64Max < m_oLocalMinMaxValue[(size_t)ui32Channel][(size_t)i].second)
				{
					f64Max = m_oLocalMinMaxValue[(size_t)ui32Channel][(size_t)i].second;
				}
			}
		}

		void CBufferDatabase::getDisplayedGlobalMinMaxValue(float64& f64Min, float64& f64Max)
		{
			f64Min = +DBL_MAX;
			f64Max = -DBL_MAX;

			for(uint32 c=0 ; c<m_oLocalMinMaxValue.size() ; c++)
			{
				for(uint64 i=0 ; i<m_oLocalMinMaxValue[(size_t)c].size() ; i++)
				{
					if(f64Min > m_oLocalMinMaxValue[(size_t)c][(size_t)i].first)
					{
						f64Min = m_oLocalMinMaxValue[(size_t)c][(size_t)i].first;
					}
					if(f64Max < m_oLocalMinMaxValue[(size_t)c][(size_t)i].second)
					{
						f64Max = m_oLocalMinMaxValue[(size_t)c][(size_t)i].second;
					}
				}
			}
		}

		uint64 CBufferDatabase::getElectrodeCount()
		{
			return s_nbChannels;
		}

		boolean CBufferDatabase::getElectrodePosition(const uint32 ui32ElectrodeIndex, float64* pElectrodePosition)
		{
			if(ui32ElectrodeIndex < s_nbChannels)
			{
				double teta = s_channelCoords[ui32ElectrodeIndex*2] * 3.1415926535 / 180.0;
				double phi  = s_channelCoords[ui32ElectrodeIndex*2+1] * 3.1415926535 / 180.0;
				*pElectrodePosition = sin(teta) * cos(phi);
				*(pElectrodePosition+1) = sin(teta) * sin(phi);
				*(pElectrodePosition+2) = cos(teta);
				return true;
			}
			return false;
		}

		boolean CBufferDatabase::getElectrodePosition(const CString& rElectrodeLabel, float64* pElectrodePosition)
		{
			for(unsigned int i=0; i<s_nbChannels; i++)
			{
				if(strcmp(rElectrodeLabel.toASCIIString(), s_channelLabels[i].c_str()) == 0)
				{
					double teta = s_channelCoords[i*2] * 3.1415926535 / 180.0;
					double phi  = s_channelCoords[i*2+1] * 3.1415926535 / 180.0;
					*pElectrodePosition = sin(teta) * cos(phi);
					*(pElectrodePosition+1) = sin(teta) * sin(phi);
					*(pElectrodePosition+2) = cos(teta);
					return true;
				}
			}

			return false;
		}

		boolean CBufferDatabase::getElectrodeLabel(const uint32 ui32ElectrodeIndex, CString& rElectrodeLabel)
		{
			if(ui32ElectrodeIndex >= s_nbChannels)
			{
				return false;
			}
			rElectrodeLabel = s_channelLabels[ui32ElectrodeIndex].c_str();
			return true;
		}

		boolean CBufferDatabase::getChannelPosition(const uint32 ui32ChannelIndex, float64*& pElectrodePosition)
		{
			if(ui32ChannelIndex >= 0 && ui32ChannelIndex < m_oElectrodesCoords.getDimensionSize(0))
			{
				pElectrodePosition = m_oElectrodesCoords.getBuffer() + 3*ui32ChannelIndex;
				return true;
			}

			return false;
		}

		boolean CBufferDatabase::getChannelSphericalCoordinates(const uint32 ui32ChannelIndex, float64& rTheta, float64& rPhi)
		{
			if(ui32ChannelIndex >= 0 && ui32ChannelIndex < m_oElectrodesSphericalCoords.getDimensionSize(0))
			{
				rTheta = *(m_oElectrodesSphericalCoords.getBuffer() + 2*ui32ChannelIndex);
				rPhi = *(m_oElectrodesSphericalCoords.getBuffer() + 2*ui32ChannelIndex+1);
				return true;
			}

			return false;
		}

		boolean CBufferDatabase::getChannelLabel(const uint32 ui32ChannelIndex, CString& rElectrodeLabel)
		{
			if(ui32ChannelIndex >= 0 && ui32ChannelIndex < m_oElectrodesLabels.size())
			{
				rElectrodeLabel = m_oElectrodesLabels[ui32ChannelIndex];
				return true;
			}
			else
			{
				rElectrodeLabel = "";
				return false;
			}
		}

		void CBufferDatabase::setStimulationCount(const uint32 ui32StimulationCount)
		{
		}

		void CBufferDatabase::setStimulation(const uint32 ui32StimulationIndex, const uint64 ui64StimulationIdentifier, const uint64 ui64StimulationDate)
		{
			// m_oParentPlugin.getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Trace << "Received new stimulation id:" << ui64StimulationIdentifier << " date:" << ui64StimulationDate << "\n";

			m_oStimulations.push_back(std::pair<uint64, uint64>(ui64StimulationDate, ui64StimulationIdentifier));

			if(m_oStartTime.size()!=0)
			{
				std::deque<std::pair<uint64, uint64> >::iterator i;
				for(i=m_oStimulations.begin(); i!=m_oStimulations.end() && (*i).first<m_oStartTime.front(); )
				{
					i++;
					m_oStimulations.pop_front();
				}
			}
		}

		void CBufferDatabase::setRedrawOnNewData(boolean bSet)
		{
			m_bRedrawOnNewData = bSet;
		}

		boolean CBufferDatabase::computeChannelCoords()
		{
			boolean res = true;

			for(uint32 i=0; i<m_pDimmensionSizes[0]; i++)
			{
				//trim string
				uint32 firstNonWhitespaceChar = 0;
				for(; firstNonWhitespaceChar < m_pDimmesionLabels[0][i].size(); firstNonWhitespaceChar++)
				{
					if(!isspace(m_pDimmesionLabels[0][i][firstNonWhitespaceChar]))
					{
						break;
					}
				}

				uint32 lastNonWhitespaceChar = 0;
				if(m_pDimmesionLabels[0][i].size() > 0)
				{
					for(lastNonWhitespaceChar = m_pDimmesionLabels[0][i].size()-1; lastNonWhitespaceChar >= 0; lastNonWhitespaceChar--)
					{
						if(!isspace(m_pDimmesionLabels[0][i][lastNonWhitespaceChar]))
						{
							break;
						}
					}
				}

				boolean l_bLabelRecognized = false;

				if(firstNonWhitespaceChar < lastNonWhitespaceChar)
				{
					std::string l_oChannelLabel(m_pDimmesionLabels[0][i].substr(firstNonWhitespaceChar, lastNonWhitespaceChar-firstNonWhitespaceChar+1));

					for(unsigned int j=0; j<s_nbChannels; j++)
					{
						//look for a similar string in database
						if(strcmp(l_oChannelLabel.c_str(), s_channelLabels[j].c_str()) == 0)
						{
							l_bLabelRecognized = true;
							double teta = s_channelCoords[j*2] * 3.1415926535 / 180.0;
							double phi  = s_channelCoords[j*2+1] * 3.1415926535 / 180.0;
							*(m_oElectrodesCoords.getBuffer()+3*i) = sin(teta) * cos(phi);
							*(m_oElectrodesCoords.getBuffer()+3*i+1) = sin(teta) * sin(phi);
							*(m_oElectrodesCoords.getBuffer()+3*i+2) = cos(teta);

							*(m_oElectrodesSphericalCoords.getBuffer()+2*i) = s_channelCoords[j*2];
							*(m_oElectrodesSphericalCoords.getBuffer()+2*i+1) = s_channelCoords[j*2+1];

							//copy label
							m_oElectrodesLabels[i] = s_channelLabels[j].c_str();

							break;
						}
					}
				}

				//unrecognized electrode!
				if(l_bLabelRecognized == false)
				{
					m_oParentPlugin.getLogManager() << LogLevel_Trace
						<< "Unrecognized electrode name (index=" << (uint32)i
						<< ", name=" << m_pDimmesionLabels[0][i].c_str()
						<< ")!\n";
					res = false;
				}
			}

			m_oParentPlugin.getLogManager() << LogLevel_Trace << "Electrodes list : " ;

			for(uint32 i=0; i<m_pDimmensionSizes[0]; i++)
			{
				m_oParentPlugin.getLogManager() << (uint32)i << ":" << m_oElectrodesLabels[i];
				if(i<m_pDimmensionSizes[0]-1)
				{
					m_oParentPlugin.getLogManager() << ", ";
				}
				else
				{
					m_oParentPlugin.getLogManager() << "\n";
				}
			}

			return res;
		}
	};
};
