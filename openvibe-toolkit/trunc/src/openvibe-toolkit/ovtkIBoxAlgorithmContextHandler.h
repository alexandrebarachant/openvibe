#ifndef __OpenViBEToolkit_IBoxAlgorithmContextHandler_H__
#define __OpenViBEToolkit_IBoxAlgorithmContextHandler_H__

#include "ovtkIObject.h"

namespace OpenViBEToolkit
{
	class OVTK_API IBoxAlgorithmContextHandler : virtual public OpenViBEToolkit::IObject
	{
	public:

		virtual OpenViBE::boolean push(OpenViBE::Plugins::IBoxAlgorithmContext* pBoxAlgorithmContext)=0;
		virtual OpenViBE::boolean pop(void)=0;
		virtual OpenViBE::Plugins::IBoxAlgorithmContext* top(void)=0;

		_IsDerivedFromClass_(OpenViBEToolkit::IObject, OVTK_ClassId_);
	};

	extern OpenViBEToolkit::IBoxAlgorithmContextHandler* createBoxAlgorithmContextHandler(void);
	extern void releaseBoxAlgorithmContextHandler(OpenViBEToolkit::IBoxAlgorithmContextHandler* pBoxAlgorithmContextHandler);
};

#endif // __OpenViBEToolkit_IBoxAlgorithmContextHandler_H__
