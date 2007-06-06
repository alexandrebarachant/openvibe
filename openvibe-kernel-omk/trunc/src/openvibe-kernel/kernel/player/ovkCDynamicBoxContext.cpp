#include "ovkCDynamicBoxContext.h"

#include <assert.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Tools;
#define boolean OpenViBE::boolean

//___________________________________________________________________//
//                                                                   //

CDynamicBoxContext::CDynamicBoxContext(const IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox)
	:TKernelObject<IBoxIO>(rKernelContext)
	,TBoxIOProxy<TKernelObject<IBoxIO> >(rKernelContext, *pSimulatedBox)
{
}

//___________________________________________________________________//
//                                                                   //
