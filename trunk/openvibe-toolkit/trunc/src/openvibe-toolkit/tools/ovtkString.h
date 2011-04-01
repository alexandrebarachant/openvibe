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
		};
	};
};

#endif // __OpenViBEToolkit_Tools_String_H__
