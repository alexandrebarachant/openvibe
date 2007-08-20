#ifndef __OpenViBE_Kernel_Scenario_IAttributable_H__
#define __OpenViBE_Kernel_Scenario_IAttributable_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class OV_API IAttributable : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

#if 0
			class OV_API IAttributeEnum
			{
			public:
				virtual ~IAttributeEnum(void) { }
				virtual void preCallback(void) { }
				virtual OpenViBE::boolean callback(
					const OpenViBE::Kernel::IAttributable& rAttributable,
					const OpenViBE::CIdentifier& rAttributeIdentifier,
					const OpenViBE::CString& rAttributeValue)=0;
				virtual void postCallback(void) { }
			};
#endif

			virtual OpenViBE::boolean addAttribute(
				const OpenViBE::CIdentifier& rAttributeIdentifier,
				const OpenViBE::CString& sAttributeValue)=0;
			virtual OpenViBE::boolean removeAttribute(
				const OpenViBE::CIdentifier& rAttributeIdentifier)=0;
			virtual OpenViBE::boolean removeAllAttributes(void)=0;

			virtual OpenViBE::CString getAttributeValue(
				const OpenViBE::CIdentifier& rAttributeIdentifier) const=0;
			virtual OpenViBE::boolean setAttributeValue(
				const OpenViBE::CIdentifier& rAttributeIdentifier,
				const OpenViBE::CString& sAttributeValue)=0;

			virtual OpenViBE::boolean hasAttribute(
				const OpenViBE::CIdentifier& rAttributeIdentifier) const=0;
			virtual OpenViBE::boolean hasAttributes(void) const=0;
#if 0
			virtual OpenViBE::boolean enumerateAttributes(
				OpenViBE::Kernel::IAttributable::IAttributeEnum& rCallback) const=0;
#endif
			virtual OpenViBE::CIdentifier getNextAttributeIdentifier(
				const OpenViBE::CIdentifier& rPreviousIdentifier) const=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Scenario_Attributable);
		};
	};
};

#endif // __OpenViBE_Kernel_Scenario_IAttributable_H__
