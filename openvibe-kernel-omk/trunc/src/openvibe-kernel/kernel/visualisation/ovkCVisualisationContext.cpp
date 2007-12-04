#include "ovkCVisualisationContext.h"
#include "../player/ovkPsSimulatedBox.h"

#include "../../tools/ovk_bridge_bind_function.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CVisualisationManagerBridge : virtual public TKernelObject<IVisualisationManager>
		{
		public:

			CVisualisationManagerBridge(const IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox) : TKernelObject<IVisualisationManager>(rKernelContext), m_pSimulatedBox(pSimulatedBox) { }

			virtual __BridgeBindFunc1__(getKernelContext().getVisualisationManager(), boolean, createVisualisationTree, , CIdentifier&, rVisualisationTreeIdentifier)
			virtual __BridgeBindFunc1__(getKernelContext().getVisualisationManager(), boolean, releaseVisualisationTree, , const CIdentifier&, rVisualisationTreeIdentifier)
			virtual __BridgeBindFunc1__(getKernelContext().getVisualisationManager(), IVisualisationTree&, getVisualisationTree, , const CIdentifier&, rVisualisationTreeIdentifier)
			virtual __BridgeBindFunc1__(getKernelContext().getVisualisationManager(), boolean, enumerateVisualisationTrees, const, IVisualisationManager::IVisualisationTreeEnum&, rCallBack)
			virtual __BridgeBindFunc4__(getKernelContext().getVisualisationManager(), boolean, setWidgets, , const CIdentifier&, rVisualisationTreeIdentifier, const CString&, rVisualisationBoxName,  GtkWidget*, pWidget, GtkWidget*, pToolbarWidget)
			/*
			virtual __BridgeBindFunc0__(getKernelContext().getVisualisationManager(), void, init3DContext, )
			virtual __BridgeBindFunc2__(getKernelContext().getVisualisationManager(), boolean, create3DVisualisationWidget, , void*, pParentWidget, OpenViBE::uint32&, uint32Index)
			virtual __BridgeBindFunc0__(getKernelContext().getVisualisationManager(), boolean, realize3DVisualisationWidgets, )
			virtual __BridgeBindFunc0__(getKernelContext().getVisualisationManager(), void, release3DContext, )
			*/
			_IsDerivedFromClass_Final_(TKernelObject<IVisualisationManager>, OV_UndefinedIdentifier);

		protected:

			::PsSimulatedBox* m_pSimulatedBox;
		};
	};
};

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
#define boolean OpenViBE::boolean

CVisualisationContext::CVisualisationContext(const IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox)
	:TKernelObject<IVisualisationContext>(rKernelContext)
	,m_pSimulatedBox(pSimulatedBox)
	,m_pVisualisationManagerBridge(NULL)
{
	m_pVisualisationManagerBridge=new CVisualisationManagerBridge(rKernelContext, pSimulatedBox);
}

CVisualisationContext::~CVisualisationContext(void)
{
	delete m_pVisualisationManagerBridge;
}

boolean CVisualisationContext::setWidgets(GtkWidget* pWidget, GtkWidget* pToolbarWidget)
{
	return m_pVisualisationManagerBridge->setWidgets(
		m_pSimulatedBox->getScenario().getVisualisationTreeIdentifier(),
		m_pSimulatedBox->getOVName(),
		pWidget,
		pToolbarWidget);
}
