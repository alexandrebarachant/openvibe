#ifndef __OpenViBE_Kernel_Visualisation_IVisualisationWidget_H__
#define __OpenViBE_Kernel_Visualisation_IVisualisationWidget_H__

#include "../scenario/ovIAttributable.h"

namespace OpenViBE
{
	namespace Kernel
	{
		enum EVisualisationWidgetType
		{
			EVisualisationWidget_Undefined,
			EVisualisationWidget_VisualisationWindow,
			EVisualisationWidget_VisualisationPanel,
			EVisualisationWidget_VisualisationBox,
			EVisualisationWidget_VerticalSplit,
			EVisualisationWidget_HorizontalSplit
		};

		class OV_API IVisualisationWidget : public OpenViBE::Kernel::IAttributable
		{
		public:

			virtual OpenViBE::boolean	initialize(
				const OpenViBE::CIdentifier& rIdentifier,
				const OpenViBE::CString& rName,
				OpenViBE::Kernel::EVisualisationWidgetType oType,
				const OpenViBE::CIdentifier& rParentIdentifier,
				const OpenViBE::CIdentifier& rBoxIdentifier,
				OpenViBE::uint32 ui32NbChildren)=0;

			virtual OpenViBE::CIdentifier	getIdentifier(void) const=0;

			virtual const OpenViBE::CString& getName(void) const=0;

			virtual void setName(const CString& rName)=0;

			virtual EVisualisationWidgetType getType(void) const=0;

			virtual OpenViBE::CIdentifier	getParentIdentifier(void) const=0;
			virtual void setParentIdentifier(
				const OpenViBE::CIdentifier& rParentIdentifier)=0;

			virtual OpenViBE::CIdentifier getBoxIdentifier(void) const=0;

			virtual OpenViBE::uint32 getNbChildren(void) const=0;

			virtual OpenViBE::boolean getChildIndex(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::uint32& ui32Index) const=0;

			// for windows, the number of children is unknown a priori
			virtual OpenViBE::boolean addChild(
				const OpenViBE::CIdentifier& rChildIdentifier)=0;

			virtual OpenViBE::boolean removeChild(
				const OpenViBE::CIdentifier& rIdentifier)=0;

			virtual OpenViBE::boolean getChildIdentifier(
				OpenViBE::uint32 ui32ChildIndex,
				OpenViBE::CIdentifier& rIdentifier) const=0;

			virtual OpenViBE::boolean setChildIdentifier(
				OpenViBE::uint32 ui32ChildIndex,
				const OpenViBE::CIdentifier& rIdentifier)=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IAttributable, OV_ClassId_Kernel_Visualisation_VisualisationWidget)
		};
	};
};

#endif //__OpenViBE_Kernel_Visualisation_IVisualisationWidget_H__
