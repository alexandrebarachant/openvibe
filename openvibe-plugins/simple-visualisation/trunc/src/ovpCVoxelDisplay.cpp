#include "ovpCVoxelDisplay.h"
#include "algorithms/ovpCAlgorithmSphericalSplineInterpolation.h"
#include <stdlib.h>
#include <math.h>
#include <memory.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::SimpleVisualisation;

namespace OpenViBEPlugins
{
namespace SimpleVisualisation
{
CVoxelDisplay::CVoxelDisplay(void) :
	m_pStreamedMatrixReader(NULL),
	m_pStreamedMatrixReaderCallBack(NULL),
	//m_pProxy(NULL),
	m_pDatabase(NULL),
	m_pVoxelView(NULL),
	m_o3DWidgetIdentifier(OV_UndefinedIdentifier),
	m_ui32NbColors(0),
	m_pColorScale(NULL),
	m_bCameraPositioned(false)
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
	//initializes the ebml input
	m_pStreamedMatrixReaderCallBack = createBoxAlgorithmStreamedMatrixInputReaderCallback(*this);
	m_pStreamedMatrixReader=EBML::createReader(*m_pStreamedMatrixReaderCallBack);

	m_pDatabase = new CBufferDatabase(*this);
	m_pVoxelView = new CVoxelView();
	m_pDatabase->setDrawable(this);
	m_pDatabase->setRedrawOnNewData(false);

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

	return true;
}

boolean CVoxelDisplay::uninitialize(void)
{
	//release the ebml reader
	releaseBoxAlgorithmStreamedMatrixInputReaderCallback(m_pStreamedMatrixReaderCallBack);
	m_pStreamedMatrixReaderCallBack=NULL;

	m_pStreamedMatrixReader->release();
	m_pStreamedMatrixReader=NULL;

	delete m_pVoxelView;
	m_pVoxelView = NULL;
	delete m_pDatabase;
	m_pDatabase = NULL;

	delete m_pColorScale;

	return true;
}

boolean CVoxelDisplay::processInput(OpenViBE::uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CVoxelDisplay::processClock(IMessageClock& rMessageClock)
{
	process3D();

	return true;
}

boolean CVoxelDisplay::process(void)
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

void CVoxelDisplay::setMatrixDimmensionCount(const uint32 ui32DimmensionCount)
{
	m_pDatabase->setMatrixDimmensionCount(ui32DimmensionCount);
}

void CVoxelDisplay::setMatrixDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
{
	m_pDatabase->setMatrixDimmensionSize(ui32DimmensionIndex, ui32DimmensionSize);
}

void CVoxelDisplay::setMatrixDimmensionLabel(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
{
	m_pDatabase->setMatrixDimmensionLabel(ui32DimmensionIndex, ui32DimmensionEntryIndex, sDimmensionLabel);
}

void CVoxelDisplay::setMatrixBuffer(const float64* pBuffer)
{
	m_pDatabase->setMatrixBuffer(pBuffer, m_ui64StartTime, m_ui64EndTime);
}

//CSignalDisplayDrawable implementation
//-------------------------------------
void CVoxelDisplay::init()
{
	m_pVoxelView->init();
}

void CVoxelDisplay::redraw()
{
	//3D widgets refresh is handled by OpenMASK
}

void CVoxelDisplay::process3D()
{
	//this initialization code could be put in init(), but then it would cause
	//sync problems with the following else (related to the camera), which
	//needs to be executed at least one cycle after
	if(m_oElectrodeIds.size() == 0)
	{
		//don't intialize 3D stuff here : it is called after
		//set background color
		getVisualisationContext().setBackgroundColor(m_o3DWidgetIdentifier, 0, 0, 0);

		m_oElectrodeIds.resize((size_t)m_pDatabase->getChannelCount());

		//create visual objects corresponding to each electrode
		for(unsigned int i=0; i<m_pDatabase->getChannelCount(); i++)
		{
			//CString l_oElectrodeName = m_pDatabase->m_pDimmesionLabels[0][i].c_str();

			m_oElectrodeIds[i] = getVisualisationContext().createObject(Standard3DObject_Sphere);

			if(m_oElectrodeIds[i] == OV_UndefinedIdentifier)
			{
				getLogManager() << LogLevel_Warning << "process3D() : couldn't create electrode object!\n";
				continue;
			}

			float64* l_pElectrodePosition;
			if(m_pDatabase->getChannelPosition(i, l_pElectrodePosition) == false)
			{
				getLogManager() << LogLevel_Warning << "process3D() : couldn't retrieve electrode position!\n";
				continue;
			}

			getVisualisationContext().setObjectPosition(m_oElectrodeIds[i],
				(float32)l_pElectrodePosition[0], (float32)l_pElectrodePosition[1], (float32)l_pElectrodePosition[2]);
		}

		//TODO : create transparent skull
	}
	else if(m_bCameraPositioned == false) //objects have been created : auto position camera
	{
		getVisualisationContext().setCameraToEncompassObjects(m_o3DWidgetIdentifier);
		m_bCameraPositioned = true;
	}

	//retrieve min/max potentials
	float64 l_f64LocalMinPotential, l_f64LocalMaxPotential;
	float64 l_f64MinPotential, l_f64MaxPotential;
	m_pDatabase->getLastBufferMinMaxValue(l_f64MinPotential, l_f64MaxPotential);
	float64 l_f64PotentialStep = (l_f64MaxPotential - l_f64MinPotential) / m_ui32NbColors;
	float64 l_f64ElectrodePotential;

	//reflect potential values visually for each electrode
	for(unsigned int i=0; i<m_oElectrodeIds.size(); i++)
	{
		//compute channel potential
		m_pDatabase->getLastBufferChannelLocalMinMaxValue(i, l_f64LocalMinPotential, l_f64LocalMaxPotential);
		l_f64ElectrodePotential =  (l_f64LocalMinPotential + l_f64MaxPotential) / 2;

		//retrieve corresponding color index
		uint32 l_ui32ColorIndex = (uint32)((l_f64ElectrodePotential - l_f64MinPotential) / l_f64PotentialStep);
		if(l_ui32ColorIndex >= m_ui32NbColors)
			l_ui32ColorIndex = m_ui32NbColors-1;

		//set color on electrode object
		getVisualisationContext().setObjectColor(m_oElectrodeIds[i], m_pColorScale[3*i], m_pColorScale[3*i+1], m_pColorScale[3*i+2]);
	}
}

};
};
