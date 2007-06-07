#include "ovkCDynamicBoxContext.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Tools;

//___________________________________________________________________//
//                                                                   //

CDynamicBoxContext::CDynamicBoxContext(const IKernelContext& rKernelContext, ::PsSimulatedBox* pSimulatedBox)
	:TKernelObject<IBoxIO>(rKernelContext)
	,TBoxIOProxy<TKernelObject<IBoxIO> >(rKernelContext, *pSimulatedBox)
{
}

//___________________________________________________________________//
//                                                                   //
