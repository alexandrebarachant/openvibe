#include "ovpCTopographicMap3DDisplay.h"
#include "algorithms/ovpCAlgorithmSphericalSplineInterpolation.h"
#include <stdlib.h>
#include <math.h>
#include <memory.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;
using namespace std;

namespace OpenViBEPlugins
{
namespace SimpleVisualisation
{
static const int s_nbChannels = 78 - 8; //8 electrodes lack spherical coords for now

static string s_channelLabels[s_nbChannels] = {
	"Iz",
	"Oz",
	"POz",
	"Pz",
	"CPz",
	"Cz",
	"FCz",
	"Fz",
	"AFz",
	"Fpz",
	"Fp1",
	"Fp2",
	"AF7",
	"AF8",
	"F7",
	"F8",
	"FT7",
	"FT8",
	"T7",
	"T8",
	"T3",
	"T4",
	"TP7",
	"TP8",
	"P7",
	"P8",
	"T5",
	"T6",
	"PO7",
	"PO8",
	"O1",
	"O2",
	"F5",
	"F6",
	"FC5",
	"FC6",
	"C5",
	"C6",
	"CP5",
	"CP6",
	"P5",
	"P6",
	"AF3",
	"AF4",
	"F3",
	"F4",
	"FC3",
	"FC4",
	"C3",
	"C4",
	"CP3",
	"CP4",
	"P3",
	"P4",
	"PO3",
	"PO4",
	"F1",
	"F2",
	"FC1",
	"FC2",
	"C1",
	"C2",
	"CP1",
	"CP2",
	"P1",
	"P2",
	"F9",
	"F10",
	/*"FT9",
	"FT10",
	"TP9",
	"TP10",*/
	"P9",
	"P10"/*,
	"PO9",
	"PO10",
	"O9",
	"O10"*/
};

static float64 s_channelCoords[3*s_nbChannels] = {
/*"Iz"*/ 					0.000000,0.324107,-1.816900,
/*"Oz"*/ 					0.000000,1.077350,-1.859270,
/*"POz"*/					0.000000,1.780830,-1.678850,
/*"Pz"*/ 					0.000000,2.283700,-1.229050,
/*"CPz"*/					0.000000,2.489760,-0.625818,
/*"Cz"*/ 					0.000000,2.521990,-0.074703,
/*"FCz"*/					0.000000,2.449310,0.458199,
/*"Fz"*/ 					0.000000,2.197910,0.948010,
/*"AFz"*/					0.000000,1.636610,1.243520,
/*"Fpz"*/					0.000000,1.087250,1.427310,
/*"Fp1"*/					0.431096,1.136220,1.383540,
/*"Fp2"*/					-0.430637,1.135010,1.382070,
/*"AF7"*/					0.868440,1.212150,1.179260,
/*"AF8"*/					-0.853668,1.191530,1.159200,
/*"F7"*/ 					1.077960,1.138210,0.695629,
/*"F8"*/ 					-1.070310,1.130140,0.690693,
/*"FT7"*/					1.193720,1.169370,0.324902,
/*"FT8"*/					-1.192900,1.168570,0.324679,
/*"T7"*/ 					1.268070,1.128820,-0.075298,
/*"T8"*/ 					-1.257960,1.119820,-0.074698,
/*"T3"*/ 					1.268070,1.128820,-0.075298,
/*"T4"*/ 					-1.257960,1.119820,-0.074698,
/*"TP7"*/					1.293350,1.093940,-0.480522,
/*"TP8"*/					-1.295430,1.095700,-0.481295,
/*"P7"*/ 					1.239860,1.040190,-0.961792,
/*"P8"*/ 					-1.239620,1.039990,-0.961601,
/*"T5"*/ 					1.239860,1.040190,-0.961792,
/*"T6"*/ 					-1.239620,1.039990,-0.961601,
/*"PO7"*/					0.991943,1.035330,-1.429190,
/*"PO8"*/					-0.984801,1.027880,-1.418900,
/*"O1"*/ 					0.541729,1.052830,-1.755660,
/*"O2"*/ 					-0.536080,1.041850,-1.737360,
/*"F5"*/ 					1.000260,1.595040,0.759633,
/*"F6"*/ 					-1.014230,1.617330,0.770249,
/*"FC5"*/					1.131910,1.606920,0.347484,
/*"FC6"*/					-1.118740,1.588230,0.343442,
/*"C5"*/ 					1.180460,1.603820,-0.073446,
/*"C6"*/ 					-1.181080,1.604670,-0.073484,
/*"CP5"*/					1.226810,1.541050,-0.559545,
/*"CP6"*/					-1.213390,1.524190,-0.553426,
/*"P5"*/ 					1.162760,1.523560,-1.077470,
/*"P6"*/ 					-1.135050,1.487260,-1.051800,
/*"AF3"*/					0.598288,1.563540,1.172600,
/*"AF4"*/					-0.599236,1.566020,1.174450,
/*"F3"*/ 					0.766714,1.887900,0.854557,
/*"F4"*/ 					-0.771871,1.900590,0.860304,
/*"FC3"*/					0.900475,1.981120,0.507370,
/*"FC4"*/					-0.900256,1.980630,0.507247,
/*"C3"*/ 					0.942403,2.071010,-0.073806,
/*"C4"*/ 					-0.950497,2.088800,-0.074440,
/*"CP3"*/					0.950821,2.040280,-0.712553,
/*"CP4"*/					-0.968544,2.078310,-0.725835,
/*"P3"*/ 					0.890590,1.988430,-1.202420,
/*"P4"*/ 					-0.873229,1.949660,-1.178990,
/*"PO3"*/					0.681842,1.608680,-1.570740,
/*"PO4"*/					-0.679282,1.602640,-1.564850,
/*"F1"*/ 					0.419552,2.086410,0.918746,
/*"F2"*/ 					-0.418200,2.079690,0.915787,
/*"FC1"*/					0.503221,2.309680,0.421867,
/*"FC2"*/					-0.503213,2.309640,0.421860,
/*"C1"*/ 					0.527505,2.453540,-0.075810,
/*"C2"*/ 					-0.516740,2.403470,-0.074263,
/*"CP1"*/					0.528469,2.384180,-0.618806,
/*"CP2"*/					-0.528633,2.384920,-0.618997,
/*"P1"*/ 					0.460929,2.191080,-1.218350,
/*"P2"*/ 					-0.461184,2.192290,-1.219030,
/*"F9"*/ 					1.131800,0.631298,0.717026,
/*"F10"*/					-1.130930,0.630810,0.716472,
/*"FT9"*/					//1.232730,0.679354,0.315221,
/*"FT10"*/				//	-1.224270,0.674692,0.313058,
/*"TP9"*/					//1.287920,0.596754,-0.609839,
/*"TP10"*/				//	-1.275420,0.590961,-0.603919,
/*"P9"*/ 					1.193760,0.416675,-0.960809,
/*"P10"*/					-1.192910,0.416379,-0.960126
/*"PO9"*/					//0.953803,0.337360,-1.411520,
/*"PO10"*/				//-0.943379,0.333673,-1.396100,
/*"O9"*/ 					//0.529359,0.330813,-1.729350,
/*"O10"*/					//-0.522668,0.326631,-1.707490
};

CTopographicMap3DDisplay::CTopographicMap3DDisplay(void) :
	m_pStreamedMatrixReader(NULL),
	m_pStreamedMatrixReaderCallBack(NULL),
	m_pProxy(NULL),
	m_pTopographicMapDatabase(NULL),
	m_pTopographicMap3DView(NULL),
	m_o3DWidgetIdentifier(OV_UndefinedIdentifier),
	m_bCameraPositioned(false),
	m_ui32NbColors(0),
	m_pColorScale(NULL),
	m_oScalpId(OV_UndefinedIdentifier),
	m_ui32NbScalpVertices(0),
	m_pScalpColors(NULL)
{
	//TODO : read color scale from some database of flow header
	m_ui32NbColors = 13;
	m_pColorScale = new float32[m_ui32NbColors*3];

	m_pColorScale[0] = 255/255.f; m_pColorScale[1] = 0/255.f; m_pColorScale[2] = 0/255.f;
	m_pColorScale[3] = 234/255.f; m_pColorScale[4] = 1/255.f; m_pColorScale[5] = 0/255.f;
	m_pColorScale[6] = 205/255.f; m_pColorScale[7] = 0/255.f; m_pColorScale[8] = 101/255.f;
	m_pColorScale[9] = 153/255.f; m_pColorScale[10] = 0/255.f; m_pColorScale[11] = 178/255.f;
	m_pColorScale[12] = 115/255.f; m_pColorScale[13] = 1/255.f; m_pColorScale[14] = 177/255.f;
	m_pColorScale[15] = 77/255.f; m_pColorScale[16] = 0/255.f; m_pColorScale[17] = 178/255.f;
	m_pColorScale[18] = 0/255.f; m_pColorScale[19] = 0/255.f; m_pColorScale[20] = 152/255.f;
	m_pColorScale[21] = 0/255.f; m_pColorScale[22] = 97/255.f; m_pColorScale[23] = 121/255.f;
	m_pColorScale[24] = 0/255.f; m_pColorScale[25] = 164/255.f; m_pColorScale[26] = 100/255.f;
	m_pColorScale[27] = 0/255.f; m_pColorScale[28] = 225/255.f; m_pColorScale[29] = 25/255.f;
	m_pColorScale[30] = 150/255.f; m_pColorScale[31] = 255/255.f; m_pColorScale[32] = 0/255.f;
	m_pColorScale[33] = 200/255.f; m_pColorScale[34] = 255/255.f; m_pColorScale[35] = 0/255.f;
	m_pColorScale[36] = 255/255.f; m_pColorScale[37] = 255/255.f; m_pColorScale[38] = 0/255.f;

	m_oSampleCoordinatesMatrix.setDimensionCount(2);
}

uint64 CTopographicMap3DDisplay::getClockFrequency(void)
{
	return ((uint64)1LL)<<37;
}

boolean CTopographicMap3DDisplay::initialize(void)
{
	//initializes the ebml input
	m_pStreamedMatrixReaderCallBack = createBoxAlgorithmStreamedMatrixInputReaderCallback(*this);
	m_pStreamedMatrixReader=EBML::createReader(*m_pStreamedMatrixReaderCallBack);

	m_pProxy=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_AlgorithmSphericalSplineInterpolation));
	m_pProxy->initialize();

	m_pTopographicMapDatabase = new CTopographicMapDatabase(*this, *m_pProxy);
	m_pTopographicMap3DView = new CTopographicMap3DView();
	m_pTopographicMapDatabase->setDrawable(this);
	m_pTopographicMapDatabase->setRedrawOnNewData(false);

	//send widget pointers to visualisation context for parenting
	::GtkWidget* l_pWidget=NULL;
	m_o3DWidgetIdentifier = getBoxAlgorithmContext()->getVisualisationContext()->create3DWidget(l_pWidget);
	if(l_pWidget != NULL)
	{
		getBoxAlgorithmContext()->getVisualisationContext()->setWidget(l_pWidget);
	}

	::GtkWidget* l_pToolbarWidget=NULL;
	m_pTopographicMap3DView->getToolbar(l_pToolbarWidget);
	if(l_pToolbarWidget != NULL)
	{
		getBoxAlgorithmContext()->getVisualisationContext()->setToolbar(l_pToolbarWidget);
	}

	return true;
}

boolean CTopographicMap3DDisplay::uninitialize(void)
{
	//release the ebml reader
	releaseBoxAlgorithmStreamedMatrixInputReaderCallback(m_pStreamedMatrixReaderCallBack);
	m_pStreamedMatrixReaderCallBack=NULL;

	m_pStreamedMatrixReader->release();
	m_pStreamedMatrixReader=NULL;

	//release algorithm
	m_pProxy->uninitialize();
	getAlgorithmManager().releaseAlgorithm(*m_pProxy);

	delete m_pTopographicMap3DView;
	m_pTopographicMap3DView = NULL;
	delete m_pTopographicMapDatabase;
	m_pTopographicMapDatabase = NULL;

	delete m_pColorScale;
	delete m_pScalpColors;

	return true;
}

boolean CTopographicMap3DDisplay::processInput(OpenViBE::uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CTopographicMap3DDisplay::processClock(IMessageClock& rMessageClock)
{
	process3D();

	return true;
}

boolean CTopographicMap3DDisplay::process(void)
{
	IDynamicBoxContext* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();
	uint32 i;

	for(i=0; i<l_pDynamicBoxContext->getInputChunkCount(0); i++)
	{
		uint64 l_ui64ChunkSize=0;
		const uint8* l_pChunkBuffer=NULL;

		if(l_pDynamicBoxContext->getInputChunk(0, i, m_ui64StartTime, m_ui64EndTime, l_ui64ChunkSize, l_pChunkBuffer))
		{
			m_pStreamedMatrixReader->processData(l_pChunkBuffer, l_ui64ChunkSize);
			l_pDynamicBoxContext->markInputAsDeprecated(0, i);
		}
	}

	return true;
}

void CTopographicMap3DDisplay::setMatrixDimmensionCount(const uint32 ui32DimmensionCount)
{
	m_pTopographicMapDatabase->setMatrixDimmensionCount(ui32DimmensionCount);
}

void CTopographicMap3DDisplay::setMatrixDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
{
	m_pTopographicMapDatabase->setMatrixDimmensionSize(ui32DimmensionIndex, ui32DimmensionSize);
}

void CTopographicMap3DDisplay::setMatrixDimmensionLabel(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
{
	m_pTopographicMapDatabase->setMatrixDimmensionLabel(ui32DimmensionIndex, ui32DimmensionEntryIndex, sDimmensionLabel);
}

void CTopographicMap3DDisplay::setMatrixBuffer(const float64* pBuffer)
{
	m_pTopographicMapDatabase->setMatrixBuffer(pBuffer, m_ui64StartTime, m_ui64EndTime);
}

//CSignalDisplayDrawable implementation
//-------------------------------------
void CTopographicMap3DDisplay::init()
{
	m_pTopographicMap3DView->init();
}

void CTopographicMap3DDisplay::redraw()
{
	//3D widgets refresh is handled by OpenMASK
}

//CTopographicMapDrawable implementation
//--------------------------------------
CMatrix* CTopographicMap3DDisplay::getSampleCoordinatesMatrix()
{
	if(m_ui32NbScalpVertices == 0)
	{
		//retrieve number of vertices in scalp mesh
		if(getVisualisationContext().getObjectVertexCount(m_oScalpId, m_ui32NbScalpVertices) == false)
		{
			//log <<
		}

		if(m_ui32NbScalpVertices > 0)
		{
			//allocate colors array
			m_pScalpColors = new float32[4*m_ui32NbScalpVertices];

			//retrieve vertices
			float32* l_pScalpVertices = new float32[3*m_ui32NbScalpVertices];

			if(getVisualisationContext().getObjectVertexPositionArray(m_oScalpId, m_ui32NbScalpVertices, l_pScalpVertices) == false)
			{
				//log <<
			}

			//allocate normalized sample coordinates matrix
			m_oSampleCoordinatesMatrix.setDimensionSize(0, m_ui32NbScalpVertices);
			m_oSampleCoordinatesMatrix.setDimensionSize(1, 3);

#if 1
			//compute scalp vertices BB
			float32 l_f32MinCoordinates[3] = { FLT_MAX, FLT_MAX, FLT_MAX };
			float32 l_f32MaxCoordinates[3] = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

			for(uint32 i=0; i<m_ui32NbScalpVertices; i++)
			{
				if(l_pScalpVertices[3*i] < l_f32MinCoordinates[0])
					l_f32MinCoordinates[0] = l_pScalpVertices[3*i];
				if(l_pScalpVertices[3*i] > l_f32MaxCoordinates[0])
					l_f32MaxCoordinates[0] = l_pScalpVertices[3*i];

				if(l_pScalpVertices[3*i+1] < l_f32MinCoordinates[1])
					l_f32MinCoordinates[1] = l_pScalpVertices[3*i+1];
				if(l_pScalpVertices[3*i+1] > l_f32MaxCoordinates[1])
					l_f32MaxCoordinates[1] = l_pScalpVertices[3*i+1];

				if(l_pScalpVertices[3*i+2] < l_f32MinCoordinates[2])
					l_f32MinCoordinates[2] = l_pScalpVertices[3*i+2];
				if(l_pScalpVertices[3*i+2] > l_f32MaxCoordinates[0])
					l_f32MaxCoordinates[2] = l_pScalpVertices[3*i+2];
			}

			//compute BB center (used as origin of normalized coordinates)
			//TODO : use barycenter or some other scheme to compute a "better" origin (see Tempo source code)
			float32 l_f32Origin[3];
			l_f32Origin[0] = (l_f32MinCoordinates[0] + l_f32MaxCoordinates[0]) / 2;
			l_f32Origin[1] = (l_f32MinCoordinates[1] + l_f32MaxCoordinates[1]) / 2;
			l_f32Origin[2] = (l_f32MinCoordinates[2] + l_f32MaxCoordinates[2]) / 2;

			//transform scalp vertices to normalized coordinates
			float32 l_f32Point[3];
			float32 l_f32InvVectorLength;
			for(uint32 i=0; i<m_ui32NbScalpVertices; i++)
			{
				/*
				  OMK     Normalized space
				  ===     ================

				  Y      Z Y
				  |      |/
				  +-- X  +-- X
				 /
				Z
				   Xn = X
				   Yn = -Z
				   Zn = Y
				*/

				//express scalp vertices in normalized frame where Z is up (and not Y as in OpenMASK)
				l_pScalpVertices[3*i] -= l_f32Origin[0];
				l_pScalpVertices[3*i+1] -= l_f32Origin[1];
				l_pScalpVertices[3*i+2] -= l_f32Origin[2];

				l_f32Point[0] = l_pScalpVertices[3*i];
				l_f32Point[1] = -l_pScalpVertices[3*i+2];
				l_f32Point[2] = l_pScalpVertices[3*i+1];

				l_f32InvVectorLength = 1 / sqrtf(l_f32Point[0]*l_f32Point[0] + l_f32Point[1]*l_f32Point[1] + l_f32Point[2]*l_f32Point[2]);

				//normalize vertices and store them in matrix to be fed to interpolation algorithm
				*(m_oSampleCoordinatesMatrix.getBuffer() + 3*i) = l_f32Point[0] * l_f32InvVectorLength;
				*(m_oSampleCoordinatesMatrix.getBuffer() + 3*i+1) = l_f32Point[1] * l_f32InvVectorLength;
				*(m_oSampleCoordinatesMatrix.getBuffer() + 3*i+2) = l_f32Point[2] * l_f32InvVectorLength;
			}
#else
			for(uint32 i=0; i<m_ui32NbScalpVertices; i++)
			{/*
				if(i>0)
					break;
				{
					CIdentifier id = getVisualisationContext().createObject(Standard3DObject_Sphere);
					getVisualisationContext().setObjectScale(id, 5.f, 5.f, 5.f);
					getVisualisationContext().setObjectPosition(id, 150, 0, 0);
				}
				{
					CIdentifier id = getVisualisationContext().createObject(Standard3DObject_Sphere);
					getVisualisationContext().setObjectScale(id, 10.f, 10.f, 10.f);
					getVisualisationContext().setObjectPosition(id, 0, 150, 0);
				}
				{
					CIdentifier id = getVisualisationContext().createObject(Standard3DObject_Sphere);
					getVisualisationContext().setObjectScale(id, 20.f, 20.f, 20.f);
					getVisualisationContext().setObjectPosition(id, 0, 0, 150);
				}*/
				/*
				CIdentifier id = getVisualisationContext().createObject(Standard3DObject_Cone);
				getVisualisationContext().setObjectScale(id, 5.f, 5.f, 5.f);
				getVisualisationContext().setObjectPosition(id, l_pScalpVertices[3*i], l_pScalpVertices[3*i+1], l_pScalpVertices[3*i+2]);
				*/

				//find distance from current vertex to each electrode
				vector<float64> l_oSquaredDistances((unsigned int)m_pTopographicMapDatabase->getElectrodeCount());

				for(uint32 j=0; j<m_pTopographicMapDatabase->getElectrodeCount(); j++)
				{
					//get electrode coordinates
					float64 l_f64ElectrodeX, l_f64ElectrodeY, l_f64ElectrodeZ;
					if(getElectrodeObjectCoordinates(j, l_f64ElectrodeX, l_f64ElectrodeY, l_f64ElectrodeZ) == false)
					{/*
						CString l_oElectrodeLabel;
						m_pTopographicMapDatabase->getElectrodeLabel(j, l_oElectrodeLabel);
						getLogManager() << LogLevel_Warning
							<< "couldn't determine coordinates of electrode #" << i
							<< "(" << l_oElectrodeLabel << ")\n";
							*/
						l_oSquaredDistances[j] = DBL_MAX;
						continue;
					}

					//compute square distance	of vertex to electrode
					l_oSquaredDistances[j] = (l_pScalpVertices[3*i] - l_f64ElectrodeX) * (l_pScalpVertices[3*i] - l_f64ElectrodeX) +
						(l_pScalpVertices[3*i+1] - l_f64ElectrodeY) * (l_pScalpVertices[3*i+1] - l_f64ElectrodeY) +
						(l_pScalpVertices[3*i+2] - l_f64ElectrodeZ) * (l_pScalpVertices[3*i+2] - l_f64ElectrodeZ);
				}

				//find 3 nearest electrodes
				uint32 l_pNearestElectrodes[3];
				float64 l_pDistanceToNearestElectrodes[3];
				float64 l_pWeights[3];
				for(uint32 j=0; j<3; j++)
				{
					uint32 l_ui32Index = 0;
					float64 l_f64SquaredDistance = l_oSquaredDistances[l_ui32Index];
					for(uint32 k=l_ui32Index+1; k<m_pTopographicMapDatabase->getElectrodeCount(); k++)
					{
						if(l_oSquaredDistances[k] < l_f64SquaredDistance)
						{
							l_ui32Index = k;
							l_f64SquaredDistance = l_oSquaredDistances[k];
						}
					}
					//save nearest electrode and distance
					l_pNearestElectrodes[j] = l_ui32Index;
					l_pDistanceToNearestElectrodes[j] = sqrt(l_f64SquaredDistance);
					//ensure it won't be selected again
					l_oSquaredDistances[l_ui32Index] = DBL_MAX;
				}

				//find weights from distances
				float64 l_f64TotalWeight = 0;
				for(uint32 j=0; j<3; j++)
				{
					if(l_pDistanceToNearestElectrodes[j] == 0)
					{
						for(uint32 k=0; k<3; k++)
						{
							l_pWeights[k] = 0;
						}
						l_pWeights[j] = 1;
						break;
					}
					l_pWeights[j] = 1 / l_pDistanceToNearestElectrodes[j];
					l_f64TotalWeight += l_pWeights[j];
				}

				//normalize weights
				if(l_f64TotalWeight != 0)
				{
					float64 l_f64InvTotalWeight = 1 / l_f64TotalWeight;
					for(uint32 k=0; k<3; k++)
					{
						l_pWeights[k] *= l_f64InvTotalWeight;
					}
				}

				//find vertex position in normalized space using 3 nearest electrode names and weight coefficients
				float64 l_f64VertexX = 0;
				float64 l_f64VertexY = 0;
				float64 l_f64VertexZ = 0;
				float64 l_pElectrodePosition[3] = { 0, 0, 0 };
				for(uint32 j=0; j<3; j++)
				{
					if(m_pTopographicMapDatabase->getElectrodePosition(l_pNearestElectrodes[j], l_pElectrodePosition) == false)
					{/*
						CString l_oElectrodeLabel;
						m_pTopographicMapDatabase->getElectrodeLabel(j, l_oElectrodeLabel);
						getLogManager() << LogLevel_Warning
							<< "couldn't determine coordinates of electrode #" << j
							<< "(" << l_oElectrodeLabel << ")\n";
							*/
						break;
					}

					l_f64VertexX += l_pWeights[j] * l_pElectrodePosition[0];
					l_f64VertexY += l_pWeights[j] * l_pElectrodePosition[1];
					l_f64VertexZ += l_pWeights[j] * l_pElectrodePosition[2];
				}

				//normalize vertex
				float64 l_f64Length = sqrt(l_f64VertexX * l_f64VertexX + l_f64VertexY * l_f64VertexY + l_f64VertexZ * l_f64VertexZ);
				if(l_f64Length > 0)
				{
					float64 l_f64InvLength = 1 / l_f64Length;
					l_f64VertexX *= l_f64InvLength;
					l_f64VertexY *= l_f64InvLength;
					l_f64VertexZ *= l_f64InvLength;
				}

				//store vertex in matrix to be fed to interpolation algorithm
				*(m_oSampleCoordinatesMatrix.getBuffer() + 3*i) = l_f64VertexX;
				*(m_oSampleCoordinatesMatrix.getBuffer() + 3*i+1) = l_f64VertexY;
				*(m_oSampleCoordinatesMatrix.getBuffer() + 3*i+2) = l_f64VertexZ;

				//DEBUG REMOVE ME
				/*
				CIdentifier id = getVisualisationContext().createObject(Standard3DObject_Sphere);
				getVisualisationContext().setObjectScale(id, 5.f, 5.f, 5.f);
				getVisualisationContext().setObjectPosition(id, -150 * l_f64VertexX, 150 * l_f64VertexY, -150 * l_f64VertexZ);
				*/
			}
#endif

			delete[] l_pScalpVertices;
		}
	}

	return &m_oSampleCoordinatesMatrix;
}

boolean CTopographicMap3DDisplay::setSampleValuesMatrix(IMatrix* pSampleValuesMatrix)
{
	//ensure matrix has the right size
	if(pSampleValuesMatrix == NULL || pSampleValuesMatrix->getDimensionSize(0) < m_ui32NbScalpVertices)
	{
		return false;
	}

	//retrieve min/max potentials
	float64 l_f64MinPotential, l_f64MaxPotential;
	m_pTopographicMapDatabase->getLastBufferInterpolatedMinMaxValue(l_f64MinPotential, l_f64MaxPotential);
	float64 l_f64InvPotentialStep = m_ui32NbColors / (l_f64MaxPotential - l_f64MinPotential);

	//determine color index of each vertex
	for(uint32 i=0; i<m_ui32NbScalpVertices; i++)
	{
		//determine color index to use
		float64 l_f64Value = *(pSampleValuesMatrix->getBuffer() + i);

		uint32 l_iColorIndex = (uint32)((l_f64Value - l_f64MinPotential) * l_f64InvPotentialStep);
		if(l_iColorIndex >= m_ui32NbColors)
			l_iColorIndex = m_ui32NbColors-1;

		m_pScalpColors[i*4] = m_pColorScale[l_iColorIndex*3];
		m_pScalpColors[i*4+1] = m_pColorScale[l_iColorIndex*3+1];
		m_pScalpColors[i*4+2] = m_pColorScale[l_iColorIndex*3+2];
		m_pScalpColors[i*4+3] = 1;
	}

	//set colors
	getVisualisationContext().setObjectVertexColorArray(m_oScalpId, m_ui32NbScalpVertices, m_pScalpColors);

	return true;
}

void CTopographicMap3DDisplay::process3D()
{
	//initialize 3D scene
	if(m_oScalpId == OV_UndefinedIdentifier)
	{
		//set background color
		getVisualisationContext().setBackgroundColor(m_o3DWidgetIdentifier, 0, 0, 0);

		//load meshes
		getVisualisationContext().setCameraToEncompassObjects(m_o3DWidgetIdentifier);
		getVisualisationContext().createObject("face");
		getVisualisationContext().setCameraToEncompassObjects(m_o3DWidgetIdentifier);
		m_oScalpId = getVisualisationContext().createObject("scalp");
		getVisualisationContext().setCameraToEncompassObjects(m_o3DWidgetIdentifier);

		//DEBUG REMOVE ME
		for(int i=0; i<s_nbChannels; i++)
		{
			CIdentifier id = getVisualisationContext().createObject(Standard3DObject_Sphere);
			getVisualisationContext().setObjectScale(id, 5.f, 5.f, 5.f);
			getVisualisationContext().setObjectPosition(id,
				*(s_channelCoords + 3*i),
				*(s_channelCoords + 3*i+1),
				*(s_channelCoords + 3*i+2));
		}
#if 0
		float64 s_extraChannelCoords[3*8] = {
			/*"FT9"*/ 44.1862,-11.2988,24.3508,
		/*"FT10"*/ -44.1862,-11.2988,24.3508,
		/*"TP9"*/ 49.6056,23.4886,22.9846,
		/*"TP10"*/ -49.6056,23.4886,22.9846,
		/*"PO9"*/ 39.114,57.8844,13.8346,
		/*"PO10"*/ -39.114,57.8844,13.8346,
		/*"O9"*/ 21.135,69.0455,13.2079,
		/*"O10"*/ -21.135,69.0455,13.2079
		};

		for(int i=0; i<8; i++)
		{
			CIdentifier id = getVisualisationContext().createObject(Standard3DObject_Sphere);
			getVisualisationContext().setObjectScale(id, 5.f, 5.f, 5.f);
			getVisualisationContext().setObjectPosition(id,
				*(s_extraChannelCoords + 3*i),
				-*(s_extraChannelCoords + 3*i+1),
				-*(s_extraChannelCoords + 3*i+2));
		}
#endif
	}
	else //meshes loaded
	{
		//camera may now be auto positioned
		if(m_bCameraPositioned == false)
		{
			getVisualisationContext().setCameraToEncompassObjects(m_o3DWidgetIdentifier);
			m_bCameraPositioned = true;
		}
	}
/*
	//create visual objects corresponding to each electrode
	if(m_pTopographicMapDatabase->isFirstBufferReceived() == true &&
		m_oElectrodeIds.size() == 0)
	{
		m_oElectrodeIds.resize((size_t)m_pTopographicMapDatabase->getChannelCount());

		for(uint32 i=0; i<m_oElectrodeIds.size(); i++)
		{
			m_oElectrodeIds[i] = getVisualisationContext().createObject(Standard3DObject_Sphere);

			if(m_oElectrodeIds[i] == OV_UndefinedIdentifier)
			{
				getLogManager() << LogLevel_Warning << "process3D() : couldn't create electrode object!\n";
				continue;
			}

			//scale electrode so that it is homogeneous with skull model
			getVisualisationContext().setObjectScale(m_oElectrodeIds[i], 10.f, 10.f, 10.f);

			//retrieve 3D coordinates of current electrode (in 3D scene coords)
			float64 l_f64ElectrodeWorldX, l_f64ElectrodeWorldY, l_f64ElectrodeWorldZ;
			if(getChannelWorldCoordinates(i, l_f64ElectrodeWorldX, l_f64ElectrodeWorldY, l_f64ElectrodeWorldZ) == false)
			{
				getLogManager() << LogLevel_Warning << "process3D() : couldn't retrieve electrode position!\n";
				continue;
			}

			//position electrode
			//REMOVE ME!!!!!
			getVisualisationContext().setObjectPosition(m_oElectrodeIds[i],
				(float32)l_f64ElectrodeWorldX, (float32)l_f64ElectrodeWorldY, (float32)l_f64ElectrodeWorldZ);
		}
	}
*/
	m_pTopographicMapDatabase->processValues();
}

boolean CTopographicMap3DDisplay::getChannelWorldCoordinates(uint32 ui32ChannelIndex, float64& rElectrodeWorldX, float64& rElectrodeWorldY, float64& rElectrodeWorldZ)
{
	/*
	OMK				Model space
	===				===========

						Z Y
						|/
		+-- X		+-- X
	 /|
	Y Z
*/
	CString l_oElectrodeLabel;
	m_pTopographicMapDatabase->getChannelLabel(ui32ChannelIndex, l_oElectrodeLabel);

	int j;
	for(j=0; j<s_nbChannels; j++)
	{
		if(s_channelLabels[j] == string(l_oElectrodeLabel))
		{
			rElectrodeWorldX = *(s_channelCoords + 3*j);
			rElectrodeWorldY = *(s_channelCoords + 3*j+1);
			rElectrodeWorldZ = *(s_channelCoords + 3*j+2);
			break;
		}
	}

	return j<s_nbChannels;
}

boolean CTopographicMap3DDisplay::getElectrodeObjectCoordinates(uint32 ui32ChannelIndex, float64& rElectrodeObjectX, float64& rElectrodeObjectY, float64& rElectrodeObjectZ)
{
	CString l_oElectrodeLabel;
	m_pTopographicMapDatabase->getElectrodeLabel(ui32ChannelIndex, l_oElectrodeLabel);

	int j;
	for(j=0; j<s_nbChannels; j++)
	{
		if(s_channelLabels[j] == string(l_oElectrodeLabel))
		{
			rElectrodeObjectX = *(s_channelCoords + 3*j);
			rElectrodeObjectY = *(s_channelCoords + 3*j+1);
			rElectrodeObjectZ = *(s_channelCoords + 3*j+2);
			break;
		}
	}

	return j<s_nbChannels;
}

};
};
