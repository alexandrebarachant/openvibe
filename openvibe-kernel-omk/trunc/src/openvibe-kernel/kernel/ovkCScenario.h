#ifndef __OpenViBEKernel_Kernel_CScenario_H__
#define __OpenViBEKernel_Kernel_CScenario_H__

#include "ovkTKernelObject.h"

#include <vector>
#include <map>

namespace OpenViBE
{
	namespace Kernel
	{
		class CScenario : virtual public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IScenario>
		{
		public:

			CScenario(const OpenViBE::Kernel::IKernelContext& rKernelContext);

			virtual OpenViBE::boolean load(
				const OpenViBE::CString& sFileName);
			virtual OpenViBE::boolean load(
				const OpenViBE::CString& sFileName,
				const OpenViBE::CIdentifier& rLoaderIdentifier);
			virtual OpenViBE::boolean save(
				const OpenViBE::CString& sFileName);
			virtual OpenViBE::boolean save(
				const OpenViBE::CString& sFileName,
				const OpenViBE::CIdentifier& rSaverIdentifieri);

			virtual OpenViBE::boolean enumerateBoxes(
				OpenViBE::Kernel::IScenario::IBoxEnum& rCallback) const;
			virtual const OpenViBE::Kernel::IBox* getBoxDetails(
				const OpenViBE::CIdentifier& rBoxIdentifier) const;
			virtual OpenViBE::Kernel::IBox* getBoxDetails(
				const OpenViBE::CIdentifier& rBoxIdentifier);
			virtual OpenViBE::boolean addBox(
				const OpenViBE::CIdentifier& rBoxAlgorithmIdentifier,
				OpenViBE::CIdentifier& rBoxIdentifier);
			virtual OpenViBE::boolean removeBox(
				const OpenViBE::CIdentifier& rBoxIdentifier);

			virtual OpenViBE::boolean enumerateLinks(
				OpenViBE::Kernel::IScenario::ILinkEnum& rCallback) const;
			virtual OpenViBE::boolean enumerateLinksFromBox(
				OpenViBE::Kernel::IScenario::ILinkEnum& rCallback,
				const OpenViBE::CIdentifier& rBoxIdentifier) const;
			virtual OpenViBE::boolean enumerateLinksFromBoxOutput(
				OpenViBE::Kernel::IScenario::ILinkEnum& rCallback,
				const OpenViBE::CIdentifier& rBoxIdentifier,
				const OpenViBE::uint32 ui32OutputIndex) const;
			virtual OpenViBE::boolean enumerateLinksToBox(
				OpenViBE::Kernel::IScenario::ILinkEnum& rCallback,
				const OpenViBE::CIdentifier& rBoxIdentifier) const;
			virtual const OpenViBE::Kernel::ILink* getLinkDetails(
				const OpenViBE::CIdentifier& rLinkIdentifier) const;
			virtual OpenViBE::Kernel::ILink* getLinkDetails(
				const OpenViBE::CIdentifier& rLinkIdentifier);
			virtual OpenViBE::boolean connect(
				const OpenViBE::CIdentifier& rSourceBoxIdentifier,
				const OpenViBE::uint32 ui32SourceBoxOutputIndex,
				const OpenViBE::CIdentifier& rTargetBoxIdentifier,
				const OpenViBE::uint32 ui32TargetBoxInputIndex,
				OpenViBE::CIdentifier& rLinkIdentifier);
			virtual OpenViBE::boolean disconnect(
				const OpenViBE::CIdentifier& rSourceBoxIdentifier,
				const OpenViBE::uint32 ui32SourceBoxOutputIndex,
				const OpenViBE::CIdentifier& rTargetBoxIdentifier,
				const OpenViBE::uint32 ui32TargetBoxInputIndex);
			virtual OpenViBE::boolean disconnect(
				const OpenViBE::CIdentifier& rLinkIdentifier);

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::IScenario, OVK_ClassId_Kernel_Scenario)

		protected:

			virtual OpenViBE::CIdentifier getUnusedIdentifier(void) const;

		protected:

			std::map<OpenViBE::CIdentifier, OpenViBE::Kernel::IBox*> m_vBox;
			std::map<OpenViBE::CIdentifier, OpenViBE::Kernel::ILink*> m_vLink;
			std::map<OpenViBE::CIdentifier, std::vector<OpenViBE::Kernel::ILink*> > m_vBoxLink;
			std::map<OpenViBE::CIdentifier, std::vector<OpenViBE::Kernel::IBox*> > m_vLinkBox;
		};
	};
};

#endif // __OpenViBEKernel_Kernel_CScenario_H__
