#ifndef __Automaton_TXMLReader_HPP__
#define __Automaton_TXMLReader_HPP__

#include "defines.h"
#include <xml/IReader.h>

#include <string.h>

namespace Automaton
{

	template<class T> class TXMLReader : virtual public T
	{
		public:
			virtual ~TXMLReader() {}

			const char * getAttributeValue(const char** sAttributeName, const char** sAttributeValue, uint64 ui64Count, const char * sWantedAttribute)
			{
				for(Automaton::uint64 i=0 ; i<ui64Count ; i++)
				{
					if(strcmp(sAttributeName[i], sWantedAttribute) == 0)
					{
						return sAttributeValue[i];
					}
				}

				return NULL;
			}

	};
};

#endif

