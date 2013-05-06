#ifndef __Automaton_IXMLNodeReaderFactory_H__
#define __Automaton_IXMLNodeReaderFactory_H__

#include "CIdentifier.h"
#include "IXMLNodeReader.h"

namespace Automaton
{
	class OV_API IXMLNodeReaderFactory
	{
		public:
			virtual IXMLNodeReader * createNodeReader(const CIdentifier& oIdentifier) = 0;
			virtual void releaseNodeReader(IXMLNodeReader * pReader) = 0;
			virtual void release() = 0;


		protected:
			virtual ~IXMLNodeReaderFactory(){}
	};

	extern OV_API Automaton::IXMLNodeReaderFactory* createXMLNodeReaderFactory();
	extern OV_API void releaseXMLNodeReaderFactory(Automaton::IXMLNodeReaderFactory* pFactory);

};

#endif

