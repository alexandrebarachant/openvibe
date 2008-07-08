#ifndef __EBML_CReader_H__
#define __EBML_CReader_H__

#include "IReader.h"

namespace EBML
{
	class EBML_API CReader : public EBML::IReader
	{
	public:

		CReader(EBML::IReaderCallback& rReaderCallback);
		virtual ~CReader(void);

		virtual EBML::boolean processData(const void* pBuffer, const EBML::uint64 ui64BufferSize);
		virtual void release(void);

	protected:

		EBML::IReader* m_pReaderImplementation;

	private:

		CReader(void);
	};
};

#endif // __EBML_IReader_H__
