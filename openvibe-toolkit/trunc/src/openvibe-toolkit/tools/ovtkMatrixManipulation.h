#ifndef __OpenViBEToolkit_Tools_MatrixManipulation_H__
#define __OpenViBEToolkit_Tools_MatrixManipulation_H__

#include "../ovtk_base.h"

namespace OpenViBEToolkit
{
	namespace Tools
	{
		namespace MatrixManipulation
		{
			OVTK_API OpenViBE::boolean copy(OpenViBE::IMatrix& rDestinationMatrix, const OpenViBE::IMatrix& rSourceMatrix);
			OVTK_API OpenViBE::boolean copyDescription(OpenViBE::IMatrix& rDestinationMatrix, const OpenViBE::IMatrix& rSourceMatrix);
			OVTK_API OpenViBE::boolean copyContent(OpenViBE::IMatrix& rDestinationMatrix, const OpenViBE::IMatrix& rSourceMatrix);
			OVTK_API OpenViBE::boolean clearContent(OpenViBE::IMatrix& rMatrix);
		};
	};
};

#endif // __OpenViBEToolkit_Tools_MatrixManipulation_H__
