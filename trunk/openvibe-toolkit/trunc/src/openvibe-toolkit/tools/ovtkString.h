#ifndef __OpenViBEToolkit_Tools_String_H__
#define __OpenViBEToolkit_Tools_String_H__

#include "../ovtk_base.h"

namespace OpenViBEToolkit
{
	namespace Tools
	{
		namespace String
		{
			OVTK_API OpenViBE::uint32 split(const OpenViBE::CString& rString, OpenViBE::CString* pString, OpenViBE::uint32 pStringCount, OpenViBE::uint8 ui8Separator);
			OVTK_API OpenViBE::uint32 split(const OpenViBE::CString& rString, OpenViBE::CString* pString, OpenViBE::uint32 pStringCount, OpenViBE::uint8* pSeparator, OpenViBE::uint32 ui32SeparatorCount);
			OVTK_API OpenViBE::boolean isAlmostEqual(const OpenViBE::CString& rString1, const OpenViBE::CString& rString2, const OpenViBE::boolean bCaseSensitive=true, const OpenViBE::boolean bRemoveStartSpaces=true, const OpenViBE::boolean bRemoveEndSpaces=true);
		};
	};
};

#endif // __OpenViBEToolkit_Tools_String_H__
