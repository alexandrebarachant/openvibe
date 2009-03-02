#include "ovpCVoxelDisplay.h"
#include "ovpCVoxelDisplay/ovpCVoxelView.h"
#include <stdlib.h>
#include <math.h>
#include <memory.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

using namespace OpenViBEToolkit;

using namespace std;

CVoxel::CVoxel():
	m_oCubeIdentifier(OV_UndefinedIdentifier),
	m_oSphereIdentifier(OV_UndefinedIdentifier),
	m_bVisible(false),
	m_f32X(0),
	m_f32Y(0),
	m_f32Z(0)
{
}

boolean CVoxel::setObjectIdentifiers(CIdentifier oCubeIdentifier, CIdentifier oSphereIdentifier)
{
	m_oCubeIdentifier = oCubeIdentifier;
	m_oSphereIdentifier = oSphereIdentifier;
	return true;
}

boolean CVoxel::setPosition(float32 f32X, float32 f32Y, float32 f32Z)
{
	m_f32X = f32X;
	m_f32Y = f32Y;
	m_f32Z = f32Z;
	return true;
}

CVoxelDisplay::CVoxelDisplay(void) :
	m_bVoxelsMatrixLoaded(false),
	m_pStreamedMatrixDatabase(NULL),
	m_pVoxelView(NULL),
	m_o3DWidgetIdentifier(OV_UndefinedIdentifier),
	m_oResourceGroupIdentifier(OV_UndefinedIdentifier),
	m_bCameraPositioned(false),
	m_bPaused(false),
	m_f64Time(0),
	m_ui32NbColors(0),
	m_pColorScale(NULL),
	m_oScalpId(OV_UndefinedIdentifier),
	m_oFaceId(OV_UndefinedIdentifier),
	m_bSetVoxelObject(false),
	m_eVoxelObject(Standard3DObject_Cube),
	m_bToggleColorModification(false),
	m_bColorModificationToggled(false),
	m_bToggleTransparencyModification(false),
	m_bTransparencyModificationToggled(false),
	m_bToggleSizeModification(false),
	m_bSizeModificationToggled(false),
	m_f64MinScaleFactor(1),
	m_f64MaxScaleFactor(2),
	m_f64VoxelDisplayThreshold(0),
	m_bSetSkullOpacity(false),
	m_f64SkullOpacity(0),
	m_bRepositionCamera(false)
{
	//TODO : read color scale from some database of flow header
	m_ui32NbColors = 13;
	m_pColorScale = new float32[m_ui32NbColors*3];
	m_pColorScale[0] = 255/255.f; m_pColorScale[1] = 255/255.f; m_pColorScale[2] = 0/255.f;
	m_pColorScale[3] = 200/255.f; m_pColorScale[4] = 255/255.f; m_pColorScale[5] = 0/255.f;
	m_pColorScale[6] = 150/255.f; m_pColorScale[7] = 255/255.f; m_pColorScale[8] = 0/255.f;
	m_pColorScale[9] = 0/255.f; m_pColorScale[10] = 225/255.f; m_pColorScale[11] = 25/255.f;
	m_pColorScale[12] = 0/255.f; m_pColorScale[13] = 164/255.f; m_pColorScale[14] = 100/255.f;
	m_pColorScale[15] = 0/255.f; m_pColorScale[16] = 97/255.f; m_pColorScale[17] = 121/255.f;
	m_pColorScale[18] = 0/255.f; m_pColorScale[19] = 0/255.f; m_pColorScale[20] = 152/255.f;
	m_pColorScale[21] = 77/255.f; m_pColorScale[22] = 0/255.f; m_pColorScale[23] = 178/255.f;
	m_pColorScale[24] = 115/255.f; m_pColorScale[25] = 1/255.f; m_pColorScale[26] = 177/255.f;
	m_pColorScale[27] = 153/255.f; m_pColorScale[28] = 0/255.f; m_pColorScale[29] = 178/255.f;
	m_pColorScale[30] = 205/255.f; m_pColorScale[31] = 0/255.f; m_pColorScale[32] = 101/255.f;
	m_pColorScale[33] = 234/255.f; m_pColorScale[34] = 1/255.f; m_pColorScale[35] = 0/255.f;
	m_pColorScale[36] = 255/255.f; m_pColorScale[37] = 0/255.f; m_pColorScale[38] = 0/255.f;
}

uint64 CVoxelDisplay::getClockFrequency(void)
{
	return ((uint64)1LL)<<37;
}

boolean CVoxelDisplay::initialize(void)
{
	// Create OVMatrix file reader
	//FIXME
	m_pOVMatrixFileReader =
		&getAlgorithmManager().getAlgorithm(
		getAlgorithmManager().createAlgorithm(
		OpenViBE::CIdentifier(0x10661A33, 0x0B0F44A7)
		/*OVP_ClassId_Algorithm_OVMatrixFileReader*/));
	m_pOVMatrixFileReader->initialize();

	// OVMatrix file reader parameters
	ip_sFilename.initialize(
		m_pOVMatrixFileReader->getInputParameter(
		OpenViBE::CIdentifier(0x28F87B29, 0x0B09737E)
		/*OVP_Algorithm_OVMatrixFileReader_InputParameterId_Filename*/));

	op_pVoxelsMatrix.initialize(
		m_pOVMatrixFileReader->getOutputParameter(
		OpenViBE::CIdentifier(0x2F9521E0, 0x027D789F)
		/*OVP_Algorithm_OVMatrixFileReader_OutputParameterId_Matrix*/));

	m_bVoxelsMatrixLoaded = false;

	//create spectrum database
	m_pStreamedMatrixDatabase = new CStreamedMatrixDatabase(*this);
	m_pStreamedMatrixDatabase->initialize();

	//retrieve box settings
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();
	//CString l_sFilename;
	l_rStaticBoxContext.getSettingValue(0, *ip_sFilename/*l_sFilename*/);
	//*ip_sFilename = l_sFilename;

	//create voxel view (handling GUI interaction)
	m_pVoxelView = new CVoxelView(*this);

	//have database notify view when data is received
	m_pStreamedMatrixDatabase->setDrawable(m_pVoxelView);

	//send widget pointers to visualisation context for parenting
	::GtkWidget* l_pWidget=NULL;
	m_o3DWidgetIdentifier = getBoxAlgorithmContext()->getVisualisationContext()->create3DWidget(l_pWidget);
	if(l_pWidget != NULL)
	{
		getBoxAlgorithmContext()->getVisualisationContext()->setWidget(l_pWidget);
	}

	::GtkWidget* l_pToolbarWidget=NULL;
	m_pVoxelView->getToolbar(l_pToolbarWidget);
	if(l_pToolbarWidget != NULL)
	{
		getBoxAlgorithmContext()->getVisualisationContext()->setToolbar(l_pToolbarWidget);
	}

	//resource group
	getVisualisationContext().createResourceGroup(m_oResourceGroupIdentifier, "VoxelDisplayResources");
	getVisualisationContext().addResourceLocation(m_oResourceGroupIdentifier, "../share/openvibe-plugins/simple-visualisation/voxeldisplay", ResourceType_Directory, false);
	getVisualisationContext().initializeResourceGroup(m_oResourceGroupIdentifier);

	return true;
}

boolean CVoxelDisplay::uninitialize(void)
{
	delete m_pVoxelView;
	m_pVoxelView = NULL;
	delete m_pStreamedMatrixDatabase;
	m_pStreamedMatrixDatabase = NULL;

	m_pOVMatrixFileReader->uninitialize();
	getAlgorithmManager().releaseAlgorithm(*m_pOVMatrixFileReader);

	delete[] m_pColorScale;

	//destroy resource group
	getVisualisationContext().destroyResourceGroup(m_oResourceGroupIdentifier);

	return true;
}

boolean CVoxelDisplay::processInput(uint32 ui32InputIndex)
{
	if(!getBoxAlgorithmContext()->getVisualisationContext()->is3DWidgetRealized(m_o3DWidgetIdentifier))
	{
		return true;
	}
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CVoxelDisplay::processClock(IMessageClock& rMessageClock)
{
	if(!getBoxAlgorithmContext()->getVisualisationContext()->is3DWidgetRealized(m_o3DWidgetIdentifier))
	{
		return true;
	}
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CVoxelDisplay::process(void)
{
	IDynamicBoxContext* l_pDynamicBoxContext=getBoxAlgorithmContext()->getDynamicBoxContext();
	uint32 i;

	for(i=0; i<l_pDynamicBoxContext->getInputChunkCount(0); i++)
	{
		m_pStreamedMatrixDatabase->decodeMemoryBuffer(
			l_pDynamicBoxContext->getInputChunk(0, i),
			l_pDynamicBoxContext->getInputChunkStartTime(0, i),
			l_pDynamicBoxContext->getInputChunkEndTime(0, i));

		l_pDynamicBoxContext->markInputAsDeprecated(0, i);
	}

	process3D();
	getBoxAlgorithmContext()->getVisualisationContext()->update3DWidget(m_o3DWidgetIdentifier);

	return true;
}

boolean CVoxelDisplay::setPaused(boolean bPaused)
{
	m_bPaused = bPaused;
	return true;
}

boolean CVoxelDisplay::setVoxelObject(EStandard3DObject eVoxelObject)
{
	m_bSetVoxelObject = true;
	m_eVoxelObject = eVoxelObject;

	//set mod flags to true so that voxel state is up to date
	m_bToggleColorModification = true;
	m_bToggleTransparencyModification = true;
	m_bToggleSizeModification = true;

	return true;
}

boolean CVoxelDisplay::toggleColorModification(boolean bModifyColor)
{
	m_bToggleColorModification = true;
	m_bColorModificationToggled = bModifyColor;
	return true;
}

boolean CVoxelDisplay::toggleTransparencyModification(boolean bModifyTransparency)
{
	m_bToggleTransparencyModification = true;
	m_bTransparencyModificationToggled = bModifyTransparency;
	return true;
}

boolean CVoxelDisplay::toggleSizeModification(boolean bModifySize)
{
	m_bToggleSizeModification = true;
	m_bSizeModificationToggled = bModifySize;
	return true;
}

boolean CVoxelDisplay::setMinScaleFactor(float64 f64MinScaleFactor)
{
	m_f64MinScaleFactor = f64MinScaleFactor;
	return true;
}

boolean CVoxelDisplay::setMaxScaleFactor(float64 f64MaxScaleFactor)
{
	m_f64MaxScaleFactor = f64MaxScaleFactor;
	return true;
}

boolean CVoxelDisplay::setVoxelDisplayThreshold(float64 f64Threshold)
{
	m_f64VoxelDisplayThreshold = f64Threshold;
	return true;
}

boolean CVoxelDisplay::setSkullOpacity(float64 f64Opacity)
{
	m_bSetSkullOpacity = true;
	m_f64SkullOpacity = f64Opacity;
	return true;
}

boolean CVoxelDisplay::repositionCamera()
{
	m_bRepositionCamera = true;
	return true;
}

static CVoxel l_oSource;
static CVoxel l_oSource2;
static float32 l_f32VoxelOffsetX = 0.f;
static float32 l_f32VoxelOffsetY = 1.2f;
static float32 l_f32VoxelOffsetZ = -0.05f;
static float32 l_f32ScaleFromOffset = 0.015f;
static float32 l_f32VoxelScale = 0.05f;
static uint32 s_ui32VoxelStep = 4;

boolean CVoxelDisplay::process3D()
{
	//load voxel coords
	if(m_bVoxelsMatrixLoaded == false)
	{
		loadVoxels();
	}
	//create voxels and load head mesh
	else if(m_oVoxels.size() == 0)
	{
		createVoxels();
	}
	else if(m_bCameraPositioned == false) //objects have been created : auto position camera
	{
		getVisualisationContext().setCameraToEncompassObjects(m_o3DWidgetIdentifier);
		m_bCameraPositioned = true;
	}
	else //proceed with color/shape modification
	{
		//shape change
		if(m_bSetVoxelObject == true)
		{
			for(uint32 i=0; i<m_oVoxels.size(); i++)
			{
				getVisualisationContext().setObjectVisible(m_oVoxels[i].m_oCubeIdentifier, m_eVoxelObject == Standard3DObject_Cube);
				getVisualisationContext().setObjectVisible(m_oVoxels[i].m_oSphereIdentifier, m_eVoxelObject == Standard3DObject_Sphere);
				//object is visible now
				m_oVoxels[i].m_bVisible = true;
			}

			m_bSetVoxelObject = false;
		}

		if(m_bToggleColorModification == true)
		{
			if(m_bColorModificationToggled == false)
			{
				//reset color
				for(uint32 i=0; i<m_oVoxels.size(); i+=s_ui32VoxelStep)
				{
					getVisualisationContext().setObjectColor(getActiveShapeIdentifier(m_oVoxels[i]), 1, 1, 1);
				}
			}

			m_bToggleColorModification = false;
		}

		if(m_bToggleTransparencyModification == true)
		{
			//unset transparency
			if(m_bTransparencyModificationToggled == false)
			{
				for(uint32 i=0; i<m_oVoxels.size(); i+=s_ui32VoxelStep)
				{
					getVisualisationContext().setObjectTransparency(getActiveShapeIdentifier(m_oVoxels[i]), 0);
				}
			}

			m_bToggleTransparencyModification = false;
		}

		if(m_bToggleSizeModification == true)
		{
			if(m_bSizeModificationToggled == false)
			{
				//reset size
				for(uint32 i=0; i<m_oVoxels.size(); i+=s_ui32VoxelStep)
				{
					getVisualisationContext().setObjectScale(getActiveShapeIdentifier(m_oVoxels[i]), l_f32VoxelScale);
				}
			}

			m_bToggleSizeModification = false;
		}

		if(m_bSetSkullOpacity == true)
		{
			getVisualisationContext().setObjectVisible(m_oFaceId, m_f64SkullOpacity > 0);
			getVisualisationContext().setObjectVisible(m_oScalpId, m_f64SkullOpacity > 0);
			getVisualisationContext().setObjectTransparency(m_oFaceId, 1.f-(float32)m_f64SkullOpacity);
			getVisualisationContext().setObjectTransparency(m_oScalpId, 1.f-(float32)m_f64SkullOpacity);
			m_bSetSkullOpacity = false;
		}

		if(m_bRepositionCamera == true)
		{
			getVisualisationContext().setCameraToEncompassObjects(m_o3DWidgetIdentifier);
			m_bRepositionCamera = false;
		}

		//get current time
		if(!m_bPaused)
		{
			uint64 l_ui64Time = getBoxAlgorithmContext()->getPlayerContext()->getCurrentTime();
			//uint64 l_ui64Time = getBoxAlgorithmContext()->getPlayerContext()->getCurrentRealTime();
			m_f64Time = (float64)l_ui64Time / (float64)(1LL<<32);
			if(m_f64Time > 1000)
			{
				m_f64Time = 0;
			}
		}

		//REMOVE ME
		computePotentials();

		//update voxels
		updateVoxels();
	}

	return true;
}

boolean CVoxelDisplay::loadVoxels()
{
	m_bVoxelsMatrixLoaded = true;

	m_pOVMatrixFileReader->process();

	//ensure matrix is 2 dimensional and that dimension sizes are correct
	if(op_pVoxelsMatrix->getDimensionCount() != 2 || op_pVoxelsMatrix->getDimensionSize(1) != 3)
	{
		getLogManager() << LogLevel_Warning << "Wrong format for voxels coordinates matrix loaded from file " << *ip_sFilename << " !\n";
		return false;
	}

	return true;
}

boolean CVoxelDisplay::createVoxels()
{
	//set background color
	getVisualisationContext().setBackgroundColor(m_o3DWidgetIdentifier, 0, 0, 0);

	l_oSource.m_oSphereIdentifier = getVisualisationContext().createObject("ov_unitsphere_80faces");
	getVisualisationContext().setObjectColor(l_oSource.m_oSphereIdentifier, 0, 1, 0);
	getVisualisationContext().setObjectScale(l_oSource.m_oSphereIdentifier, 0.001f, 0.001f, 0.001f);
	getVisualisationContext().setObjectVisible(l_oSource.m_oSphereIdentifier, false);

	//load skull meshes
	m_oFaceId = getVisualisationContext().createObject("ov_voxeldisplay_face");
	m_oScalpId = getVisualisationContext().createObject("ov_voxeldisplay_scalp");
	//initialize skull opacity
	getVisualisationContext().setObjectVisible(m_oFaceId, m_f64SkullOpacity > 0);
	getVisualisationContext().setObjectVisible(m_oScalpId, m_f64SkullOpacity > 0);
	getVisualisationContext().setObjectTransparency(m_oFaceId, 1.f-(float32)m_f64SkullOpacity);
	getVisualisationContext().setObjectTransparency(m_oScalpId, 1.f-(float32)m_f64SkullOpacity);

	//create voxels
	if(op_pVoxelsMatrix->getDimensionCount() != 2)
	{
		return false;
	}
	m_oVoxels.resize(op_pVoxelsMatrix->getDimensionSize(0));
	CNameValuePairList l_oParamsList;
	l_oParamsList.setValue("CloneMaterials", true);

	for(uint32 i=0; i<op_pVoxelsMatrix->getDimensionSize(0); i+=s_ui32VoxelStep)
	{
		//object is visible by default
		m_oVoxels[i].m_bVisible = true;
		//store voxel position
		m_oVoxels[i].setPosition(
			(*(op_pVoxelsMatrix->getBuffer() + 3*i) - l_f32VoxelOffsetX) * l_f32ScaleFromOffset + l_f32VoxelOffsetX,
			(*(op_pVoxelsMatrix->getBuffer() + 3*i+1) - l_f32VoxelOffsetY) * l_f32ScaleFromOffset + l_f32VoxelOffsetY,
			(*(op_pVoxelsMatrix->getBuffer() + 3*i+2) - l_f32VoxelOffsetZ) * l_f32ScaleFromOffset + l_f32VoxelOffsetZ);

		//load shapes
		m_oVoxels[i].setObjectIdentifiers(
			getVisualisationContext().createObject("ov_unitcube", &l_oParamsList),
			getVisualisationContext().createObject("ov_unitsphere_80faces", &l_oParamsList));

		//show active shape and hide the other one
		getVisualisationContext().setObjectVisible(m_oVoxels[i].m_oCubeIdentifier, m_eVoxelObject == Standard3DObject_Cube);
		getVisualisationContext().setObjectVisible(m_oVoxels[i].m_oSphereIdentifier, m_eVoxelObject == Standard3DObject_Sphere);
		//position 3D objects
		getVisualisationContext().setObjectPosition(m_oVoxels[i].m_oCubeIdentifier, m_oVoxels[i].m_f32X, m_oVoxels[i].m_f32Y, m_oVoxels[i].m_f32Z);
		getVisualisationContext().setObjectPosition(m_oVoxels[i].m_oSphereIdentifier, m_oVoxels[i].m_f32X, m_oVoxels[i].m_f32Y, m_oVoxels[i].m_f32Z);
		//scale 3D objects
		getVisualisationContext().setObjectScale(m_oVoxels[i].m_oCubeIdentifier, l_f32VoxelScale, l_f32VoxelScale, l_f32VoxelScale);
		getVisualisationContext().setObjectScale(m_oVoxels[i].m_oSphereIdentifier, l_f32VoxelScale, l_f32VoxelScale, l_f32VoxelScale);
	}

	//REMOVE ME
	m_oPotentialMatrix.setDimensionCount(1);
	m_oPotentialMatrix.setDimensionSize(0, m_oVoxels.size());
	//

	return true;
}

boolean CVoxelDisplay::computePotentials()
{
#if 1
	//update source position
	static float64 l_f64OrbitCenterX = 0;
	static float64 l_f64OrbitCenterY = 1.5;
	static float64 l_f64OrbitCenterZ = -0.3;
	static float64 l_f64OrbitRadius = 1;
	l_oSource.m_f32X = (float32)(l_f64OrbitCenterX + l_f64OrbitRadius * cos(m_f64Time * 3.1415926535f));
	l_oSource.m_f32Y = (float32)l_f64OrbitCenterY;
	l_oSource.m_f32Z = (float32)(l_f64OrbitCenterZ + l_f64OrbitRadius * sin(m_f64Time * 3.1415926535f));

	//update source attenuation factor
	float64 l_f64SourceAttenuationFactor = cos(m_f64Time * 0.7);
	if(l_f64SourceAttenuationFactor < 0)
	{
		l_f64SourceAttenuationFactor = -l_f64SourceAttenuationFactor;
	}

	//update voxel activation levels
	// float64 l_f64MinDistance = 0;
	float64 l_f64MaxDistance = 1.3;
	float64 l_f64SquareMaxDistance = 1.3*1.3;

	//getVisualisationContext().setObjectPosition(l_oSource.m_oSphereIdentifier, l_oSource.m_f32X, l_oSource.m_f32Y, l_oSource.m_f32Z);

	//fill matrix
	for(uint32 i=0; i<m_oVoxels.size(); i+=s_ui32VoxelStep)
	{
		//voxel potential, assumed to stay in [0, 1]
		float64 l_f64Potential = 0;

		//compute voxel activation level based on distance from source
		float64 squareDistance = (m_oVoxels[i].m_f32X - l_oSource.m_f32X) *  (m_oVoxels[i].m_f32X - l_oSource.m_f32X) +
		(m_oVoxels[i].m_f32Y - l_oSource.m_f32Y) *  (m_oVoxels[i].m_f32Y - l_oSource.m_f32Y) +
		(m_oVoxels[i].m_f32Z - l_oSource.m_f32Z) *  (m_oVoxels[i].m_f32Z - l_oSource.m_f32Z);

		if(squareDistance < l_f64SquareMaxDistance)
		{
			l_f64Potential = (1 - sqrt(squareDistance) / l_f64MaxDistance) * l_f64SourceAttenuationFactor;
		}

		//ensure potential doesn't exceed 1
		if(l_f64Potential > 1)
		{
			l_f64Potential = 1;
		}

		*(m_oPotentialMatrix.getBuffer() + i) = l_f64Potential;
	}
#else
	//retrieve matrix
	const float64* l_pBuffer = m_pStreamedMatrixDatabase->getBuffer(m_pStreamedMatrixDatabase->getBufferCount()-1);

	//compute vector norm
	for(uint32 i=0; i<m_oVoxels.size(); i++)
	{
		//FIXME take into account number of samples per buffer and use last buffer!
		const float64* l_pVoxelBuffer = l_pBuffer + 3 * (i /** nbEchantillons*/);

		float64 f64Potential = sqrt(*l_pVoxelBuffer * *l_pVoxelBuffer +
			*(l_pVoxelBuffer+1) * *(l_pVoxelBuffer+1) + *(l_pVoxelBuffer+2) * *(l_pVoxelBuffer+2));

		*(m_oPotentialMatrix.getBuffer() + i) = f64Potential;
/*
		if(m_f64MinPotential > f64Potential)
		{
			m_f64MinPotential = f64Potential;
		}
		else if(m_f64MaxPotential < f64Potential)
		{
			m_f64MaxPotential = f64Potential;
		}
*/
	}
#endif

	return true;
}

boolean CVoxelDisplay::getMinMaxPotentials(float64& rMinPotential, float64& rMaxPotential)
{
#if 1
	rMinPotential = -1;
	rMaxPotential = 1;
#else
	m_rStreamedMatrixDatabase->getLastBufferMinMaxValues(rMinPotential, rMaxPotential);
#endif
	return true;
}

boolean CVoxelDisplay::updateVoxels()
{
	float64 l_f64MinPotential = 0;
	float64 l_f64MaxPotential = 0;
	getMinMaxPotentials(l_f64MinPotential, l_f64MaxPotential);

	float64 l_f64PotentialStep = (l_f64MaxPotential - l_f64MinPotential) / m_ui32NbColors;

	//update each voxel
	for(uint32 i=0; i<m_oVoxels.size(); i+=s_ui32VoxelStep)
	{
#if 1
		//voxel potential, assumed to stay in [0, 1]
		float64 l_f64Potential = *(m_oPotentialMatrix.getBuffer() + i);
#else
		const float64* l_pBuffer = m_pStreamedMatrixDatabase->getBuffer(m_pStreamedMatrixDatabase->getBufferCount()-1);
		if(l_pBuffer == NULL)
		{
			return false;
		}
		float64 l_f64Potential = *(l_pBuffer + i);
#endif

		//determine whether this voxel should be displayed
		float64 l_f64ActivationFactor = (l_f64Potential - l_f64MinPotential) / (l_f64MaxPotential - l_f64MinPotential); //0<x<1
		/*
		l_f64ActivationFactor -= 0.5; //-0.5<x<0.5
		l_f64ActivationFactor *= 2; //-1<x<1
		if(l_f64ActivationFactor < 0)
		{
			l_f64ActivationFactor = -l_f64ActivationFactor;
		}*/

		boolean l_bDisplayVoxel = (l_f64ActivationFactor >= m_f64VoxelDisplayThreshold);
		if(l_bDisplayVoxel != m_oVoxels[i].m_bVisible)
		{
			m_oVoxels[i].m_bVisible = l_bDisplayVoxel;
			getVisualisationContext().setObjectVisible(getActiveShapeIdentifier(m_oVoxels[i]), l_bDisplayVoxel);
		}

		if(l_bDisplayVoxel == false)
		{
			continue;
		}

		//retrieve corresponding color index
		uint32 l_ui32ColorIndex = (uint32)((l_f64Potential - l_f64MinPotential) / l_f64PotentialStep);
		if(l_ui32ColorIndex >= m_ui32NbColors)
		{
			l_ui32ColorIndex = m_ui32NbColors-1;
		}

		if(m_bColorModificationToggled == true)
		{
			getVisualisationContext().setObjectColor(
				getActiveShapeIdentifier(m_oVoxels[i]),
				m_pColorScale[3*l_ui32ColorIndex],
				m_pColorScale[3*l_ui32ColorIndex+1],
				m_pColorScale[3*l_ui32ColorIndex+2]);
		}

		if(m_bTransparencyModificationToggled == true)
		{
			float32 l_f32Opacity = (float32)((l_f64Potential - m_f64VoxelDisplayThreshold) / (1 - m_f64VoxelDisplayThreshold));

			getVisualisationContext().setObjectTransparency(getActiveShapeIdentifier(m_oVoxels[i]),	1 - l_f32Opacity);
		}

		if(m_bSizeModificationToggled == true)
		{
			getVisualisationContext().setObjectScale(
				getActiveShapeIdentifier(m_oVoxels[i]), l_f32VoxelScale * (
				l_f64Potential < 0 ?
				(float32)(m_f64MinScaleFactor - l_f64Potential * (m_f64MaxScaleFactor - m_f64MinScaleFactor)) :
				(float32)(m_f64MinScaleFactor + l_f64Potential * (m_f64MaxScaleFactor - m_f64MinScaleFactor)))
				);
		}
	}

	return true;
}

CIdentifier CVoxelDisplay::getActiveShapeIdentifier(CVoxel& rVoxel)
{
	return m_eVoxelObject==Standard3DObject_Cube?rVoxel.m_oCubeIdentifier:rVoxel.m_oSphereIdentifier;
}
