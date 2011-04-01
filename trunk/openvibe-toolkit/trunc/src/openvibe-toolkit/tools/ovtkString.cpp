#include "ovtkString.h"

#include <string>
#include <vector>

using namespace OpenViBE;

namespace OpenViBEToolkit
{
	namespace Tools
	{
		namespace String
		{
			namespace
			{
				inline boolean isSeparator(uint8 ui8Value, uint8* pSeparator, uint32 ui32SeparatorCount)
				{
					for(uint32 i=0; i<ui32SeparatorCount; i++)
					{
						if(ui8Value==pSeparator[i])
						{
							return true;
						}
					}
					return false;
				}
			}
		};
	};
};

uint32 OpenViBEToolkit::Tools::String::split(const CString& rString, CString* pString, uint32 ui32StringCount, uint8 ui8Separator)
{
	return OpenViBEToolkit::Tools::String::split(rString, pString, ui32StringCount, &ui8Separator, 1);
}

uint32 OpenViBEToolkit::Tools::String::split(const CString& rString, CString* pString, uint32 ui32StringCount, uint8* pSeparator, uint32 ui32SeparatorCount)
{
	if(pString==NULL) return 0;
	if(ui32StringCount==0) return 0;
	if(ui32SeparatorCount==0 || pSeparator==NULL) return 0;

	std::string l_sString(rString.toASCIIString());
	std::vector < std::string > l_vResult;
	std::string::size_type i=0;
	std::string::size_type j=0;
	while(i<l_sString.length())
	{
		j=i;
		while(j<l_sString.length() && !OpenViBEToolkit::Tools::String::isSeparator(l_sString[j], pSeparator, ui32SeparatorCount))
		{
			j++;
		}
//		if(i!=j)
		{
			l_vResult.push_back(std::string(l_sString, i, j-i));
		}
		i=j+1;
	}
	if(l_sString.length()!=0 && OpenViBEToolkit::Tools::String::isSeparator(l_sString[l_sString.length()-1], pSeparator, ui32SeparatorCount))
	{
		l_vResult.push_back("");
	}

	if(l_vResult.size()>ui32StringCount) return 0;

	for(uint32 i=0; i<l_vResult.size(); i++)
	{
		pString[i]=l_vResult[i].c_str();
	}
	return l_vResult.size();
}
