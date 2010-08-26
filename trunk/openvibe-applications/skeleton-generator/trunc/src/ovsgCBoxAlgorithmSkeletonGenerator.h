#ifndef __OpenViBESkeletonGenerator_CBoxAlgorithmSkeletonGenerator_H__
#define __OpenViBESkeletonGenerator_CBoxAlgorithmSkeletonGenerator_H__

#include "ovsgCSkeletonGenerator.h"

namespace OpenViBESkeletonGenerator
{
	class CBoxAlgorithmSkeletonGenerator : public CSkeletonGenerator
	{
	public:

		CBoxAlgorithmSkeletonGenerator(OpenViBE::Kernel::IKernelContext & rKernelContext,::GtkBuilder * pBuilderInterface);
		virtual ~CBoxAlgorithmSkeletonGenerator(void);

		void initialize(void);
	};
}

#endif //__OpenViBESkeletonGenerator_CBoxAlgorithmSkeletonGenerator_H__
