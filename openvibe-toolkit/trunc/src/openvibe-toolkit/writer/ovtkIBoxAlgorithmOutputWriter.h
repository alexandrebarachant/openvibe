#ifndef __OpenViBEToolkit_Writer_IBoxAlgorithmOutputWriter_H__
#define __OpenViBEToolkit_Writer_IBoxAlgorithmOutputWriter_H__

#include "../ovtkIObject.h"

namespace OpenViBEToolkit
{
	class OVTK_API IBoxAlgorithmOutputWriter : virtual public OpenViBEToolkit::IObject
	{
	public:

		_IsDerivedFromClass_(OpenViBEToolkit::IObject, OVTK_ClassId_);
	};
};

#endif // __OpenViBEToolkit_Writer_IBoxAlgorithmOutputWriter_H__
