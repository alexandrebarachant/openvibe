#include "ovsgCAlgorithmSkeletonGenerator.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESkeletonGenerator;

CAlgorithmSkeletonGenerator::CAlgorithmSkeletonGenerator(IKernelContext & rKernelContext, ::GtkBuilder * pBuilderInterface)
	:CSkeletonGenerator(rKernelContext, pBuilderInterface)
{
}
CAlgorithmSkeletonGenerator::~CAlgorithmSkeletonGenerator(void)
{
}

void CAlgorithmSkeletonGenerator::initialize(void)
{
}
