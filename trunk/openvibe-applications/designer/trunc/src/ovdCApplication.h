#ifndef __OpenViBEDesigner_CApplication_H__
#define __OpenViBEDesigner_CApplication_H__

#include "ovd_base.h"

#include <vector>

namespace OpenViBEDesigner
{
	class CInterfacedScenario;

	class CApplication
	{
	public:
		CApplication(const OpenViBE::Kernel::IKernelContext& rKernelContext);

		void initialize(void);

		/** \name Drag and drop management */
		//@{

		void dragDataGetCB(
			::GtkWidget* pWidget,
			::GdkDragContext* pDragContex,
			::GtkSelectionData* pSelectionData,
			guint uiInfo,
			guint uiT);

		//@}

		/** \name Selection management */
		//@{

		void copySelectionCB(void);
		void cutSelectionCB(void);
		void pasteSelectionCB(void);
		void deleteSelectionCB(void);
		void preferencesCB(void);

		//@}

		/** \name Scenario management */
		//@{

		OpenViBE::CString getWorkingDirectory(void);

		OpenViBE::boolean hasScenarioRunning(void);

		OpenViBEDesigner::CInterfacedScenario* getCurrentInterfacedScenario(void);

		void testCB(void);

		void newScenarioCB(void);

		void openScenarioCB(void);

		void saveScenarioCB(void);

		void saveScenarioAsCB(void);

		void closeScenarioCB(
			OpenViBEDesigner::CInterfacedScenario* pInterfacedScenario);

		void stopScenarioCB(void);

		void pauseScenarioCB(void);

		void nextScenarioCB(void);

		void playScenarioCB(void);

		void forwardScenarioCB(void);

		void changeCurrentScenario(
			OpenViBE::int32 i32PageIndex);

		//@}

		/** \name Designer visualisation management */
		//@{

		void deleteDesignerVisualisationCB();

		void toggleDesignerVisualisationCB();

		//@}

		/** \name Player management */
		//@{

		OpenViBE::Kernel::IPlayer* getPlayer(void);

		void createPlayer(void);

		void releasePlayer(void);

		//@}

		/** \name Application management */
		//@{

		OpenViBE::boolean quitApplicationCB(void);

		//@}

		/** \name Log management */
		//@{

		void logLevelCB(void);

		//@}

		/** \name CPU usage */
		//@{

		void CPUUsageCB(void);

		//@}

	public:

		const OpenViBE::Kernel::IKernelContext& m_rKernelContext;
		OpenViBE::Kernel::IPluginManager* m_pPluginManager;
		OpenViBE::Kernel::IScenarioManager* m_pScenarioManager;
		OpenViBE::Kernel::IVisualisationManager* m_pVisualisationManager;
		OpenViBE::Kernel::IScenario* m_pClipboardScenario;

		::GladeXML* m_pGladeInterface;
		::GtkWidget* m_pMainWindow;
		::GtkNotebook* m_pScenarioNotebook;
		::GtkNotebook* m_pResourceNotebook;
		::GtkTreeStore* m_pBoxAlgorithmTreeModel;
		::GtkTreeView* m_pBoxAlgorithmTreeView;
		::GtkTreeStore* m_pAlgorithmTreeModel;
		::GtkTreeView* m_pAlgorithmTreeView;

		std::vector < OpenViBEDesigner::CInterfacedScenario* > m_vInterfacedScenario;
	};
};

#endif // __OpenViBEDesigner_CApplication_H__