#include "ovpCSimple3DDisplay.h"
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

CSimple3DDisplay::CSimple3DDisplay(void) :
	m_pStreamedMatrixReader(NULL),
	m_pStreamedMatrixReaderCallBack(NULL),
	m_pProxy(NULL),
	m_pSimple3DDatabase(NULL),
	m_o3DWidgetIdentifier(OV_UndefinedIdentifier)
{
}

uint64 CSimple3DDisplay::getClockFrequency(void)
{
	return ((uint64)1LL)<<36;
}

boolean CSimple3DDisplay::initialize(void)
{
	//initializes the ebml input
	m_pStreamedMatrixReaderCallBack = createBoxAlgorithmStreamedMatrixInputReaderCallback(*this);
	m_pStreamedMatrixReader=EBML::createReader(*m_pStreamedMatrixReaderCallBack);

	m_pProxy=&getAlgorithmManager().getAlgorithm(getAlgorithmManager().createAlgorithm(OVP_ClassId_AlgorithmSphericalSplineInterpolation));
	m_pProxy->initialize();

	m_pSimple3DDatabase = new CSimple3DDatabase(*this);

	m_pSimple3DView = new CSimple3DView(*m_pSimple3DDatabase);
	m_pSimple3DDatabase->setDrawable(m_pSimple3DView);
	m_pSimple3DDatabase->setRedrawOnNewData(false);

	//send widget pointers to visualisation context for parenting
	::GtkWidget* l_pWidget=NULL;
	m_o3DWidgetIdentifier = getBoxAlgorithmContext()->getVisualisationContext()->create3DWidget(l_pWidget);
	if(l_pWidget != NULL)
	{
		getBoxAlgorithmContext()->getVisualisationContext()->setWidget(l_pWidget);
	}

	::GtkWidget* l_pToolbarWidget=NULL;
	dynamic_cast<CSimple3DView*>(m_pSimple3DView)->getToolbar(l_pToolbarWidget);
	if(l_pToolbarWidget != NULL)
	{
		getBoxAlgorithmContext()->getVisualisationContext()->setToolbar(l_pToolbarWidget);
	}

	m_pSimple3DDatabase->set3DWidgetIdentifier(m_o3DWidgetIdentifier);

	/*
	//initialise plugin-specific resources
	m_oResourceGroupIdentifier = getBoxAlgorithmContext()->getVisualisationContext()->createResourceGroup(m_sName);

	//add resources
	//char buf[MAX_PATH];
	//sprintf(buf, "./media/%s", m_sName.toCString());
	//getBoxAlgorithmContext()->getVisualisationContext()->addResourceLocation(m_oResourceGroupIdentifier, buf, EResourceLocationType::RESOURCE_LOCATION_FILE, true);
	getBoxAlgorithmContext()->getVisualisationContext()->addResourceLocation(m_oResourceGroupIdentifier, "./media/Plugin0", OpenViBE::Kernel::RESOURCE_LOCATION_FILE, true);
	//getBoxAlgorithmContext()->getVisualisationContext()->addResourceLocation(m_oResourceGroupIdentifier, "./media/Plugin0/tempo", EResourceLocationType::RESOURCE_LOCATION_FILE, true);

	//initialize them
	getBoxAlgorithmContext()->getVisualisationContext()->initializeResourceGroup(m_oResourceGroupIdentifier);
	*/

	return true;
}

boolean CSimple3DDisplay::uninitialize(void)
{
	//release the ebml reader
	releaseBoxAlgorithmStreamedMatrixInputReaderCallback(m_pStreamedMatrixReaderCallBack);
	m_pStreamedMatrixReaderCallBack=NULL;

	m_pStreamedMatrixReader->release();
	m_pStreamedMatrixReader=NULL;

	delete m_pSimple3DView;
	m_pSimple3DView = NULL;
	delete m_pSimple3DDatabase;
	m_pSimple3DDatabase = NULL;

	m_pProxy->uninitialize();

	getAlgorithmManager().releaseAlgorithm(*m_pProxy);

	return true;
}

OpenViBE::boolean CSimple3DDisplay::processInput(OpenViBE::uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();
	return true;
}

boolean CSimple3DDisplay::processClock(IMessageClock& rMessageClock)
{
	return true;
}

boolean CSimple3DDisplay::process(void)
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

	m_pSimple3DDatabase->process3D();

	return true;
}

void CSimple3DDisplay::setMatrixDimmensionCount(const uint32 ui32DimmensionCount)
{
	m_pSimple3DDatabase->setMatrixDimmensionCount(ui32DimmensionCount);
}

void CSimple3DDisplay::setMatrixDimmensionSize(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionSize)
{
	m_pSimple3DDatabase->setMatrixDimmensionSize(ui32DimmensionIndex, ui32DimmensionSize);
}

void CSimple3DDisplay::setMatrixDimmensionLabel(const uint32 ui32DimmensionIndex, const uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel)
{
	m_pSimple3DDatabase->setMatrixDimmensionLabel(ui32DimmensionIndex, ui32DimmensionEntryIndex, sDimmensionLabel);
}

void CSimple3DDisplay::setMatrixBuffer(const float64* pBuffer)
{
	m_pSimple3DDatabase->setMatrixBuffer(pBuffer, m_ui64StartTime, m_ui64EndTime);
}

};
};
