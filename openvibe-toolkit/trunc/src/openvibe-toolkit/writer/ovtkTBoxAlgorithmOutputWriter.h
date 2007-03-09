#ifndef __OpenViBEToolkit_Writer_TBoxAlgorithmOutputWriter_H__
#define __OpenViBEToolkit_Writer_TBoxAlgorithmOutputWriter_H__

#include "ovtkIBoxAlgorithmOutputWriter.h"

#include <ebml/IWriterHelper.h>

namespace OpenViBEToolkit
{
	template <class T>
	class OVTK_API TBoxAlgorithmOutputWriter : virtual public T
	{
	public:

		TBoxAlgorithmOutputWriter(void)
			:m_pWriterHelper(NULL)
		{
			m_pWriterHelper=EBML::createWriterHelper();
		}

		virtual ~TBoxAlgorithmOutputWriter(void)
		{
			m_pWriterHelper->release();
			m_pWriterHelper=NULL;
		}

		_IsDerivedFromClass_(OpenViBEToolkit::IBoxAlgorithmOutputWriter, OVTK_ClassId_);

	protected:

		EBML::IWriterHelper* m_pWriterHelper;
	};
};

#endif // __OpenViBEToolkit_Writer_TBoxAlgorithmOutputWriter_H__
