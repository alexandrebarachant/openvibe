#ifndef __OpenViBESkeletonGenerator_CAlgorithmSkeletonGenerator_H__
#define __OpenViBESkeletonGenerator_CAlgorithmSkeletonGenerator_H__


#include "ovsgCSkeletonGenerator.h"

namespace OpenViBESkeletonGenerator
{
	class CAlgorithmSkeletonGenerator : public CSkeletonGenerator
	{

	public:
		CAlgorithmSkeletonGenerator(OpenViBE::Kernel::IKernelContext & rKernelContext, ::GladeXML * pGladeInterface);
		virtual ~CAlgorithmSkeletonGenerator(void);

		void initialize(void);
	};
}
#endif //__OpenViBESkeletonGenerator_CAlgorithmSkeletonGenerator_H__
