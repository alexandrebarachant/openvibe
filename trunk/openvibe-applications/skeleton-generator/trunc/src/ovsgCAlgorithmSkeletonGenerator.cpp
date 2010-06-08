#include "ovsgCAlgorithmSkeletonGenerator.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESkeletonGenerator;

CAlgorithmSkeletonGenerator::CAlgorithmSkeletonGenerator(IKernelContext & rKernelContext, ::GladeXML * pGladeInterface)
: CSkeletonGenerator(rKernelContext,pGladeInterface)
{
}
CAlgorithmSkeletonGenerator::~CAlgorithmSkeletonGenerator(void)
{	
}

void CAlgorithmSkeletonGenerator::initialize( void )
{
}
