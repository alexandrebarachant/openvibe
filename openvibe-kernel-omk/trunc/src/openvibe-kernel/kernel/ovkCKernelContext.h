#ifndef __OpenViBEKernel_CKernelContext_H__
#define __OpenViBEKernel_CKernelContext_H__

#include "ovkTKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class CLogListenerConsole;
		class CLogListenerFile;

		class CKernelContext : public OpenViBE::Kernel::IKernelContext
		{
		public:

			CKernelContext(const OpenViBE::CString& rApplicationName, const OpenViBE::CString& rConfigurationFile);
			virtual ~CKernelContext(void);

			virtual OpenViBE::Kernel::IAlgorithmManager& getAlgorithmManager(void) const;
			virtual OpenViBE::Kernel::IConfigurationManager& getConfigurationManager(void) const;
			virtual OpenViBE::Kernel::IKernelObjectFactory& getKernelObjectFactory(void) const;
			virtual OpenViBE::Kernel::IPlayerManager& getPlayerManager(void) const;
			virtual OpenViBE::Kernel::IPluginManager& getPluginManager(void) const;
			virtual OpenViBE::Kernel::IScenarioManager& getScenarioManager(void) const;
			virtual OpenViBE::Kernel::ITypeManager& getTypeManager(void) const;
			virtual OpenViBE::Kernel::ILogManager& getLogManager(void) const;
			virtual OpenViBE::Kernel::IVisualisationManager& getVisualisationManager(void) const;

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::IKernelContext, OVK_ClassId_Kernel_KernelContext)

		protected:

			virtual OpenViBE::Kernel::ELogLevel earlyGetLogLevel(const OpenViBE::CString& rLogLevelName);

		protected:

			mutable OpenViBE::Kernel::IAlgorithmManager* m_pAlgorithmManager;
			mutable OpenViBE::Kernel::IConfigurationManager* m_pConfigurationManager;
			mutable OpenViBE::Kernel::IKernelObjectFactory* m_pKernelObjectFactory;
			mutable OpenViBE::Kernel::IPlayerManager* m_pPlayerManager;
			mutable OpenViBE::Kernel::IPluginManager* m_pPluginManager;
			mutable OpenViBE::Kernel::IScenarioManager* m_pScenarioManager;
			mutable OpenViBE::Kernel::ITypeManager* m_pTypeManager;
			mutable OpenViBE::Kernel::ILogManager* m_pLogManager;
			mutable OpenViBE::Kernel::IVisualisationManager* m_pVisualisationManager;

			OpenViBE::Kernel::CLogListenerConsole* m_pLogListenerConsole;
			OpenViBE::Kernel::CLogListenerFile* m_pLogListenerFile;

		private:

			CKernelContext(void);
		};
	};
};

#endif // __OpenViBEKernel_CKernelContext_H__
