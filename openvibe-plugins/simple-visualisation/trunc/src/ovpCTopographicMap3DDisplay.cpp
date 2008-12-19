#include "ovpCTopographicMap3DDisplay.h"
#include "ovpCTopographicMap3DDisplay/ovpCTopographicMap3DView.h"
#include "algorithms/ovpCAlgorithmSphericalSplineInterpolation.h"
#include <stdlib.h>
#include <math.h>
#include <memory.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;
using namespace OpenViBEToolkit;
using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;
using namespace std;

CTopographicMap3DDisplay::CTopographicMap3DDisplay(void) :
	m_pChannelLocalisationStreamDecoder(NULL),
	m_pStreamedMatrixReader(NULL),
	m_pStreamedMatrixReaderCallBack(NULL),
	m_pSphericalSplineInterpolation(NULL),
	m_pTopographicMapDatabase(NULL),
	m_pTopographicMap3DView(NULL),
	m_o3DWidgetIdentifier(OV_UndefinedIdentifier),
	m_oResourceGroupIdentifier(OV_UndefinedIdentifier),
	m_bSkullCreated(false),
	m_bCameraPositioned(false),
	m_bElectrodesCreated(false),
	m_ui32NbColors(0),
	m_pColorScale(NULL),
	m_bNeedToggleElectrodes(false),
	m_bElectrodesToggleState(false),
	m_bNeedToggleSamplingPoints(false),
	m_bSamplingPointsToggleState(false),
	m_oScalpId(OV_UndefinedIdentifier),
	m_ui32NbScalpVertices(0),
	m_pScalpVertices(NULL),
	m_pScalpColors(NULL)
{
	m_f32ProjectionCenter[0] = 0.f;
	m_f32ProjectionCenter[1] = 0.f;
	m_f32ProjectionCenter[2] = 0.f;

	//TODO : read color scale from some database or flow header
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
	//initialize chanloc decoder
	m_pChannelLocalisationStreamDecoder = &getAlgorithmManager().getAlgorithm(
		getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_ChannelLocalisationStreamDecoder));
	m_pChannelLocalisationStreamDecoder->initialize();

	//initializes the ebml input
	m_pStreamedMatrixReaderCallBack = createBoxAlgorithmStreamedMatrixInputReaderCallback(*this);
	m_pStreamedMatrixReader=EBML::createReader(*m_pStreamedMatrixReaderCallBack);

	//initialize spline interpolation algorithm
	m_pSphericalSplineInterpolation = &getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_Algorithm_SphericalSplineInterpolation));
	m_pSphericalSplineInterpolation->initialize();

	//create topographic map database
	m_pTopographicMapDatabase = new CTopographicMapDatabase(*this, *m_pSphericalSplineInterpolation);

	//retrieve settings
	CString l_sInterpolationModeSettingValue;
	getStaticBoxContext().getSettingValue(0, l_sInterpolationModeSettingValue);
	CString l_sDelaySettingValue;
	getStaticBoxContext().getSettingValue(1, l_sDelaySettingValue);
	getStaticBoxContext().getSettingValue(2, m_oFaceMeshFilename);
	getStaticBoxContext().getSettingValue(3, m_oScalpMeshFilename);
	getStaticBoxContext().getSettingValue(4, m_oProjectionSphereMeshFilename);

	//create topographic map view (handling GUI interaction)
	m_pTopographicMap3DView = new CTopographicMap3DView(
		*this,
		*m_pTopographicMapDatabase,
		getTypeManager().getEnumerationEntryValueFromName(OVP_TypeId_SphericalLinearInterpolationType, l_sInterpolationModeSettingValue),
		atof(l_sDelaySettingValue));

	//have database notify us when new data is available
	m_pTopographicMapDatabase->setDrawable(this);
	//ask not to be notified when new data is available (refresh is handled separately)
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

	//resource group
	getVisualisationContext().createResourceGroup(m_oResourceGroupIdentifier, "TopographicMap3DResources");
	getVisualisationContext().addResourceLocation(m_oResourceGroupIdentifier, "../share/openvibe-plugins/simple-visualisation/topographicmap3D", ResourceType_Directory, false);
	getVisualisationContext().initializeResourceGroup(m_oResourceGroupIdentifier);

	return true;
}

boolean CTopographicMap3DDisplay::uninitialize(void)
{
	//delete decoder algorithm
	m_pChannelLocalisationStreamDecoder->uninitialize();
	getAlgorithmManager().releaseAlgorithm(*m_pChannelLocalisationStreamDecoder);

	//release the ebml reader
	releaseBoxAlgorithmStreamedMatrixInputReaderCallback(m_pStreamedMatrixReaderCallBack);
	m_pStreamedMatrixReaderCallBack=NULL;

	m_pStreamedMatrixReader->release();
	m_pStreamedMatrixReader=NULL;

	//release algorithm
	m_pSphericalSplineInterpolation->uninitialize();
	getAlgorithmManager().releaseAlgorithm(*m_pSphericalSplineInterpolation);

	delete m_pTopographicMap3DView;
	m_pTopographicMap3DView = NULL;
	delete m_pTopographicMapDatabase;
	m_pTopographicMapDatabase = NULL;

	delete[] m_pColorScale;
	delete[] m_pScalpColors;
	if(m_pScalpVertices != NULL)
	{
		delete[] m_pScalpVertices;
	}

	//destroy resource group
	getVisualisationContext().destroyResourceGroup(m_oResourceGroupIdentifier);

	return true;
}

boolean CTopographicMap3DDisplay::processInput(uint32 ui32InputIndex)
{
	if(!getBoxAlgorithmContext()->getVisualisationContext()->is3DWidgetRealized(m_o3DWidgetIdentifier))
	{
		return true;
	}
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CTopographicMap3DDisplay::processClock(IMessageClock& rMessageClock)
{
	if(!getBoxAlgorithmContext()->getVisualisationContext()->is3DWidgetRealized(m_o3DWidgetIdentifier))
	{
		return true;
	}
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CTopographicMap3DDisplay::process(void)
{
	IDynamicBoxContext* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();
	uint32 i;

	//decode signal data
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

	//decode channel localisation data
	for(i=0; i<l_pDynamicBoxContext->getInputChunkCount(1); i++)
	{
		const IMemoryBuffer* l_pBuf = l_pDynamicBoxContext->getInputChunk(1, i);
		m_pTopographicMapDatabase->decodeChannelLocalisationMemoryBuffer(
			l_pBuf,
			l_pDynamicBoxContext->getInputChunkStartTime(1, i),
			l_pDynamicBoxContext->getInputChunkEndTime(1, i));
		l_pDynamicBoxContext->markInputAsDeprecated(1, i);
	}

	//decode channel localisation (in model frame) data
	for(i=0; i<l_pDynamicBoxContext->getInputChunkCount(2); i++)
	{
		const IMemoryBuffer* l_pBuf = l_pDynamicBoxContext->getInputChunk(2, i);
		decodeChannelLocalisationMemoryBuffer(l_pBuf);
		l_pDynamicBoxContext->markInputAsDeprecated(2, i);
	}

	process3D();

	getBoxAlgorithmContext()->getVisualisationContext()->update3DWidget(m_o3DWidgetIdentifier);

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
			getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Error << "Couldn't retrieve number of vertices from scalp object\n";
		}

		if(m_ui32NbScalpVertices > 0)
		{
			//allocate colors array
			m_pScalpColors = new float32[4*m_ui32NbScalpVertices];

			//retrieve vertices
			if(m_pScalpVertices == NULL)
			{
				m_pScalpVertices = new float32[3*m_ui32NbScalpVertices];
			}

			if(getVisualisationContext().getObjectVertexPositionArray(m_oScalpId, m_ui32NbScalpVertices, m_pScalpVertices) == false)
			{
				getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Error << "Couldn't retrieve vertex array from scalp object\n";
			}

			//allocate normalized sample coordinates matrix
			m_oSampleCoordinatesMatrix.setDimensionSize(0, m_ui32NbScalpVertices);
			m_oSampleCoordinatesMatrix.setDimensionSize(1, 3);

			//compute scalp vertices coordinates once projected onto a unit sphere
			float32 l_f32UnitVector[3];
			float32* l_pScalpVertexCoord = m_pScalpVertices;
			float64* l_pSampleCoordsBuffer = m_oSampleCoordinatesMatrix.getBuffer();

			for(uint32 i=0; i<m_ui32NbScalpVertices; i++, l_pScalpVertexCoord+=3)
			{
				/* Ogre     Normalized space
					 ====     ================
						Y            Zn
						|            |
						+-- X    Xn--+
					 /            /
					Z            Yn
					 => X = -Xn, Y = Zn, Z = Yn */

				//compute vector from center of unit sphere to scalp vertex
				l_f32UnitVector[0] = -l_pScalpVertexCoord[0] - m_f32ProjectionCenter[0];
				l_f32UnitVector[1] = l_pScalpVertexCoord[2] - m_f32ProjectionCenter[1];
				l_f32UnitVector[2] = l_pScalpVertexCoord[1] - m_f32ProjectionCenter[2];

				//normalize vector
				float32 l_f32InvLength = 1.f / sqrtf(l_f32UnitVector[0]*l_f32UnitVector[0] + l_f32UnitVector[1]*l_f32UnitVector[1] + l_f32UnitVector[2]*l_f32UnitVector[2]);
				l_f32UnitVector[0] *= l_f32InvLength;
				l_f32UnitVector[1] *= l_f32InvLength;
				l_f32UnitVector[2] *= l_f32InvLength;

				//store vertex in matrix to be fed to interpolation algorithm
				*l_pSampleCoordsBuffer++ = l_f32UnitVector[0];
				*l_pSampleCoordsBuffer++ = l_f32UnitVector[1];
				*l_pSampleCoordsBuffer++ = l_f32UnitVector[2];

				//display a sphere at the location of the normalized coordinates

				/*CIdentifier id = getVisualisationContext().createObject(Standard3DObject_Sphere);
				getVisualisationContext().setObjectScale(id, 0.001f, 0.001f, 0.001f);
				getVisualisationContext().setObjectPosition(id, 3 * (-l_f32UnitVector[0]), 3 * l_f32UnitVector[2], 3 * l_f32UnitVector[1]);*/
			}
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
	float64 l_f64InvPotentialStep = 0;
	if(l_f64MinPotential < l_f64MaxPotential)
	{
		l_f64InvPotentialStep = m_ui32NbColors / (l_f64MaxPotential - l_f64MinPotential);
	}

	//determine color index of each vertex
	for(uint32 i=0; i<m_ui32NbScalpVertices; i++)
	{
		//determine color index to use
		float64 l_f64Value = *(pSampleValuesMatrix->getBuffer() + i);

		uint32 l_iColorIndex = (uint32)((l_f64Value - l_f64MinPotential) * l_f64InvPotentialStep);
		if(l_iColorIndex >= m_ui32NbColors)
		{
			l_iColorIndex = m_ui32NbColors-1;
		}

		m_pScalpColors[i*4] = m_pColorScale[l_iColorIndex*3];
		m_pScalpColors[i*4+1] = m_pColorScale[l_iColorIndex*3+1];
		m_pScalpColors[i*4+2] = m_pColorScale[l_iColorIndex*3+2];
		m_pScalpColors[i*4+3] = 1;
	}

	//set colors
	getVisualisationContext().setObjectVertexColorArray(m_oScalpId, m_ui32NbScalpVertices, m_pScalpColors);

	return true;
}

void CTopographicMap3DDisplay::toggleElectrodes(boolean bToggle)
{
	m_bNeedToggleElectrodes = true;
	m_bElectrodesToggleState = bToggle;
}

/*void CTopographicMap3DDisplay::toggleSamplingPoints(boolean bToggle)
{
	m_bNeedToggleSamplingPoints = true;
	m_bSamplingPointsToggleState = bToggle;
}*/

boolean CTopographicMap3DDisplay::decodeChannelLocalisationMemoryBuffer(const IMemoryBuffer* pMemoryBuffer)
{
	//feed memory buffer to decoder
	m_pChannelLocalisationStreamDecoder->getInputParameter(
		OVP_GD_Algorithm_ChannelLocalisationStreamDecoder_InputParameterId_MemoryBufferToDecode)->setReferenceTarget(&pMemoryBuffer);

	//process buffer
	m_pChannelLocalisationStreamDecoder->process();

	//copy header if needed
	if(m_pChannelLocalisationStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_ChannelLocalisationStreamDecoder_OutputTriggerId_ReceivedHeader) == true)
	{
		//retrieve matrix header
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oMatrix;
		l_oMatrix.initialize(m_pChannelLocalisationStreamDecoder->getOutputParameter(OVP_GD_Algorithm_ChannelLocalisationStreamDecoder_OutputParameterId_Matrix));
		if(l_oMatrix->getDimensionSize(1) != 3)
		{
			getLogManager() << LogLevel_Error
				<< "Wrong size found for dimension 1 of channel localisation header! Can't process header!\n";
			return false;
		}

		//copy channel labels
		Tools::Matrix::copyDescription(m_oModelElectrodeCoordinates, *l_oMatrix);
	}

	//has a chanloc buffer been received?
	if(m_pChannelLocalisationStreamDecoder->isOutputTriggerActive(OVP_GD_Algorithm_ChannelLocalisationStreamDecoder_OutputTriggerId_ReceivedBuffer) == true)
	{
		//retrieve coordinates matrix
		OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > l_oMatrix;
		l_oMatrix.initialize(m_pChannelLocalisationStreamDecoder->getOutputParameter(OVP_GD_Algorithm_ChannelLocalisationStreamDecoder_OutputParameterId_Matrix));

		//save labels
		CMatrix l_oTempMatrix;
		Tools::Matrix::copyDescription(l_oTempMatrix, m_oModelElectrodeCoordinates);

		//resize matrix
		Tools::Matrix::copyDescription(m_oModelElectrodeCoordinates, *l_oMatrix);

		//copy labels
		for(uint32 i=0; i<m_oModelElectrodeCoordinates.getDimensionSize(0); i++)
		{
			m_oModelElectrodeCoordinates.setDimensionLabel(0, i, l_oTempMatrix.getDimensionLabel(0, i));
		}
		for(uint32 i=0; i<m_oModelElectrodeCoordinates.getDimensionSize(1); i++)
		{
			m_oModelElectrodeCoordinates.setDimensionLabel(1, i, l_oTempMatrix.getDimensionLabel(1, i));
		}

		//copy coordinates
		Tools::Matrix::copyContent(m_oModelElectrodeCoordinates, *l_oMatrix);

		m_bModelElectrodeCoordinatesInitialized = true;
	}

	return true;
}

void CTopographicMap3DDisplay::process3D()
{
	//first pass : initialize 3D scene
	if(m_bSkullCreated == false)
	{
		createSkull();
		return;
	}
	//second pass : auto position camera
	else if(m_bCameraPositioned == false)
	{
		getVisualisationContext().setCameraToEncompassObjects(m_o3DWidgetIdentifier);
		m_bCameraPositioned = true;
		return;
	}

	//wait for first buffer
	if(m_pTopographicMapDatabase->isFirstBufferReceived() == false)
	{
		return;
	}

	//third pass : create electrode and sampling point objects
	if(m_bElectrodesCreated == false)
	{
		createElectrodes();
		//createSamplingPoints();
		m_bElectrodesCreated = true;
		return;
	}

	//fourth pass and more : handle 3D requests
	if(m_bNeedToggleElectrodes == true)
	{
		for(uint32 i=0; i<m_oElectrodeIds.size(); i++)
		{
			getVisualisationContext().setObjectVisible(m_oElectrodeIds[i], m_bElectrodesToggleState);
		}

		m_bNeedToggleElectrodes = false;
	}

	if(m_bNeedToggleSamplingPoints == true)
	{
		for(uint32 i=0; i<m_oSamplingPointIds.size(); i++)
		{
			getVisualisationContext().setObjectVisible(m_oSamplingPointIds[i], m_bSamplingPointsToggleState);
		}

		m_bNeedToggleSamplingPoints = false;
	}

	m_pTopographicMapDatabase->processValues();
}

boolean CTopographicMap3DDisplay::createSkull()
{
	//set background color
	getVisualisationContext().setBackgroundColor(m_o3DWidgetIdentifier, 0, 0, 0);

	//load face mesh
	if(getVisualisationContext().createObject(m_oFaceMeshFilename) == OV_UndefinedIdentifier)
	{
		getLogManager() << LogLevel_Warning << "Couldn't load face mesh!\n";
	}

	//load scalp mesh
	CNameValuePairList l_oParams;
	l_oParams.setValue("CloneMeshes", true); //clone scalp mesh so that it doesn't interfere with other maps
	m_oScalpId = getVisualisationContext().createObject(m_oScalpMeshFilename, &l_oParams);

	if(m_oScalpId == OV_UndefinedIdentifier)
	{
		getLogManager() << LogLevel_Warning << "Couldn't load scalp mesh!\n";
	}

	//load projection sphere mesh
	CIdentifier l_oDummyObject = getVisualisationContext().createObject(m_oProjectionSphereMeshFilename);
	if(l_oDummyObject == OV_UndefinedIdentifier)
	{
		getLogManager() << LogLevel_Warning << "Couldn't load projection sphere mesh!\n";
	}
	else
	{
		float32 l_oMin[3];
		float32 l_oMax[3];
		getVisualisationContext().getObjectAxisAlignedBoundingBox(l_oDummyObject, l_oMin, l_oMax);
		float32 l_oModelSpaceProjectionCenter[3];

		l_oModelSpaceProjectionCenter[0] = (l_oMin[0] + l_oMax[0]) / 2;
		l_oModelSpaceProjectionCenter[1] = (l_oMin[1] + l_oMax[1]) / 2;
		l_oModelSpaceProjectionCenter[2] = (l_oMin[2] + l_oMax[2]) / 2;

		/* Ogre     Normalized space
		   ====     ================
		   Y            Zn
		   |            |
		   +-- X    Xn--+
		  /            /
		 Z            Yn
		 => X = -Xn, Y = Zn, Z = Yn */

		m_f32ProjectionCenter[0] = -l_oModelSpaceProjectionCenter[0];
		m_f32ProjectionCenter[1] = l_oModelSpaceProjectionCenter[2];
		m_f32ProjectionCenter[2] = l_oModelSpaceProjectionCenter[1];
	}

	//set skull creation flag
	m_bSkullCreated = true;

	return true;
}

boolean CTopographicMap3DDisplay::createElectrodes()
{
	//create visual objects corresponding to each channel
#if 1
	m_oElectrodeIds.resize((size_t)m_pTopographicMapDatabase->getChannelCount());

	for(uint32 i=0; i<m_oElectrodeIds.size(); i++)
	{
		m_oElectrodeIds[i] = getVisualisationContext().createObject(Standard3DObject_Sphere);

		if(m_oElectrodeIds[i] == OV_UndefinedIdentifier)
		{
			CString l_oElectrodeLabel;
			m_pTopographicMapDatabase->getChannelLabel(i, l_oElectrodeLabel);
			getLogManager() << LogLevel_Warning << "Couldn't create electrode object for channel " << l_oElectrodeLabel << " !\n";
			break;
		}

		//should electrodes be shown initially?
		if(m_bNeedToggleElectrodes == false)
		{
			getVisualisationContext().setObjectVisible(m_oElectrodeIds[i], false);
		}

		//scale electrode so that it is homogeneous with skull model
		getVisualisationContext().setObjectScale(m_oElectrodeIds[i], 0.002f, 0.002f, 0.002f);

		//retrieve 3D coordinates of current electrode (in 3D scene coords)
		float64 l_f64ElectrodeWorldX, l_f64ElectrodeWorldY, l_f64ElectrodeWorldZ;
		if(getChannelWorldCoordinates(i, l_f64ElectrodeWorldX, l_f64ElectrodeWorldY, l_f64ElectrodeWorldZ) == false)
		{
			CString l_oElectrodeLabel;
			m_pTopographicMapDatabase->getChannelLabel(i, l_oElectrodeLabel);
			getLogManager() << LogLevel_Warning << "Couldn't retrieve electrode position for channel " << l_oElectrodeLabel << " !\n";
			continue;
		}

		//position electrode
		getVisualisationContext().setObjectPosition(m_oElectrodeIds[i],
			(float32)l_f64ElectrodeWorldX, (float32)l_f64ElectrodeWorldY, (float32)l_f64ElectrodeWorldZ);
	}
#else
	//create visual objects corresponding to each electrode in database
	m_oElectrodeIds.resize((size_t)m_pTopographicMapDatabase->getElectrodeCount());

	for(uint32 i=0; i<m_oElectrodeIds.size(); i++)
	{
		m_oElectrodeIds[i] = getVisualisationContext().createObject(Standard3DObject_Sphere);

		if(m_oElectrodeIds[i] == OV_UndefinedIdentifier)
		{
			CString l_oElectrodeLabel;
			m_pTopographicMapDatabase->getChannelLabel(i, l_oElectrodeLabel);
			getLogManager() << LogLevel_Warning << "Couldn't create electrode object for channel " << l_oElectrodeLabel << " !\n";
			continue;
		}

		//scale electrode so that it is homogeneous with skull model
		getVisualisationContext().setObjectScale(m_oElectrodeIds[i], 0.001f, 0.001f, 0.001f);

		//retrieve 3D coordinates of current electrode (in 3D scene coords)
		float64 l_f64ElectrodeWorldX, l_f64ElectrodeWorldY, l_f64ElectrodeWorldZ;
		if(getElectrodeObjectCoordinates(i, l_f64ElectrodeWorldX, l_f64ElectrodeWorldY, l_f64ElectrodeWorldZ) == false)
		{
			CString l_oElectrodeLabel;
			m_pTopographicMapDatabase->getChannelLabel(i, l_oElectrodeLabel);
			getLogManager() << LogLevel_Warning << "Couldn't retrieve electrode position for channel " << l_oElectrodeLabel << " !\n";
			continue;
		}

		//position electrode
		getVisualisationContext().setObjectPosition(m_oElectrodeIds[i],
			(float32)l_f64ElectrodeWorldX, (float32)l_f64ElectrodeWorldY, (float32)l_f64ElectrodeWorldZ);
	}
#endif

	return true;
}

boolean CTopographicMap3DDisplay::createSamplingPoints()
{
	//display a sphere at the vertex location
	m_oSamplingPointIds.resize(m_ui32NbScalpVertices);

	//retrieve vertices
	if(m_pScalpVertices == NULL)
	{
		m_pScalpVertices = new float32[3*m_ui32NbScalpVertices];
	}

	if(getVisualisationContext().getObjectVertexPositionArray(m_oScalpId, m_ui32NbScalpVertices, m_pScalpVertices) == false)
	{
		getBoxAlgorithmContext()->getPlayerContext()->getLogManager() << LogLevel_Error << "Couldn't retrieve vertex array from scalp object\n";
	}

	for(uint32 i=0; i<m_ui32NbScalpVertices; i++)
	{
		m_oSamplingPointIds[i] = getVisualisationContext().createObject(Standard3DObject_Sphere);

		if(m_oSamplingPointIds[i] == OV_UndefinedIdentifier)
		{
			getLogManager() << LogLevel_Warning << "process3D() : couldn't create electrode object!\n";
			break;
		}

		//hide objects by default
		getVisualisationContext().setObjectVisible(m_oSamplingPointIds[i], false);

		//position electrode at vertex location
		getVisualisationContext().setObjectPosition(m_oSamplingPointIds[i], m_pScalpVertices[3*i], m_pScalpVertices[3*i+1], m_pScalpVertices[3*i+2]);

		float64 l_f64VertexX, l_f64VertexY, l_f64VertexZ;
		l_f64VertexX = *(m_oSampleCoordinatesMatrix.getBuffer() + 3*i);
		l_f64VertexY = *(m_oSampleCoordinatesMatrix.getBuffer() + 3*i+1);
		l_f64VertexZ = *(m_oSampleCoordinatesMatrix.getBuffer() + 3*i+2);

		//map theta and phi to size/color
#define M_PI       3.14159265358979323846

		float64 theta = acos(l_f64VertexZ);
		float64 phi;
		if(l_f64VertexX > 0.001)
		{
			phi = atan(l_f64VertexY / l_f64VertexX);

			if(phi < 0)
			{
				phi += 2 * M_PI;
			}
		}
		else if(l_f64VertexX < -0.001)
		{
			phi = atan(l_f64VertexY / l_f64VertexX) + M_PI;
		}
		else
		{
			phi = l_f64VertexY > 0 ? (M_PI / 2) : (3 * M_PI/2);
		}

		static float64 minScale = 0.0005f;
		static float64 maxScale = 0.002f;
#if 0 //reflect phi angle on size
		float64 scale = minScale + (maxScale - minScale) * phi / (2*M_PI);
		getVisualisationContext().setObjectScale(m_oSamplingPointIds[i], scale, scale, scale);

		getVisualisationContext().setObjectColor(m_oSamplingPointIds[i], theta / M_PI * 1.f, 0, 0);
		getVisualisationContext().setObjectTransparency(m_oSamplingPointIds[i], 0.5f);
#else //reflect theta angle on size
		float32 scale = (float32)(minScale + (maxScale - minScale) * theta / M_PI);
		getVisualisationContext().setObjectScale(m_oSamplingPointIds[i], scale, scale, scale);

		getVisualisationContext().setObjectColor(m_oSamplingPointIds[i], (float32)(phi / (2*M_PI) * 1.f), 0, 0);
		getVisualisationContext().setObjectTransparency(m_oSamplingPointIds[i], 0.5f);
	}
#endif
	return true;
}

boolean CTopographicMap3DDisplay::getChannelWorldCoordinates(uint32 ui32ChannelIndex, float64& rElectrodeWorldX, float64& rElectrodeWorldY, float64& rElectrodeWorldZ)
{
	CString l_oElectrodeLabel;
	m_pTopographicMapDatabase->getChannelLabel(ui32ChannelIndex, l_oElectrodeLabel);

	uint32 j;
	for(j=0; j<m_oModelElectrodeCoordinates.getDimensionSize(0); j++)
	{
		if(string(m_oModelElectrodeCoordinates.getDimensionLabel(0, j)) == string(l_oElectrodeLabel))
		{
			rElectrodeWorldX = *(m_oModelElectrodeCoordinates.getBuffer() + 3*j);
			rElectrodeWorldY = *(m_oModelElectrodeCoordinates.getBuffer() + 3*j+1);
			rElectrodeWorldZ = *(m_oModelElectrodeCoordinates.getBuffer() + 3*j+2);
			break;
		}
	}

	return j<m_oModelElectrodeCoordinates.getDimensionSize(0);
}

boolean CTopographicMap3DDisplay::getElectrodeObjectCoordinates(uint32 ui32ChannelIndex, float64& rElectrodeObjectX, float64& rElectrodeObjectY, float64& rElectrodeObjectZ)
{
	CString l_oElectrodeLabel;
	m_pTopographicMapDatabase->getElectrodeLabel(ui32ChannelIndex, l_oElectrodeLabel);

	uint32 j;
	for(j=0; j<m_oModelElectrodeCoordinates.getDimensionSize(0); j++)
	{
		if(string(m_oModelElectrodeCoordinates.getDimensionLabel(0, j)) == string(l_oElectrodeLabel))
		{
			rElectrodeObjectX = *(m_oModelElectrodeCoordinates.getBuffer() + 3*j);
			rElectrodeObjectY = *(m_oModelElectrodeCoordinates.getBuffer() + 3*j+1);
			rElectrodeObjectZ = *(m_oModelElectrodeCoordinates.getBuffer() + 3*j+2);
			break;
		}
	}

	return j<m_oModelElectrodeCoordinates.getDimensionSize(0);
}
