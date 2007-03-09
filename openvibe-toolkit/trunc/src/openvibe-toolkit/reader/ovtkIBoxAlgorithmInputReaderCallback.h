#ifndef __OpenViBEToolkit_Reader_IBoxAlgorithmInputReaderCallback_H__
#define __OpenViBEToolkit_Reader_IBoxAlgorithmInputReaderCallback_H__

#include "../ovtkIObject.h"

#include <ebml/IReader.h>

namespace OpenViBEToolkit
{
	class OVTK_API IBoxAlgorithmInputReaderCallback : virtual public OpenViBEToolkit::IObject, virtual public EBML::IReaderCallback
	{
	public:

		_IsDerivedFromClass_(OpenViBEToolkit::IObject, OVTK_ClassId_);
	};
};

#endif // __OpenViBEToolkit_Reader_IBoxAlgorithmInputReaderCallback_H__
