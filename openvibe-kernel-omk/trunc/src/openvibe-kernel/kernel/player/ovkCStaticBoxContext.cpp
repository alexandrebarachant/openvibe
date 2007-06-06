#include "ovkCStaticBoxContext.h"

#include <assert.h>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Tools;
#define boolean OpenViBE::boolean

//___________________________________________________________________//
//                                                                   //

CStaticBoxContext::CStaticBoxContext(const IKernelContext& rKernelContext, const IBox* pBox)
	:TKernelObject<IBox>(rKernelContext)
	,TBoxProxy<TKernelObject<IBox> >(rKernelContext, *pBox)
{
}
