#ifndef __Stream_Defines_H__
#define __Stream_Defines_H__

#include <ov_common_defines.h>

#define OV_APPEND_TO_NAMESPACE Stream
#include <ov_common_types.h>
#undef OV_APPEND_TO_NAMESPACE

namespace Stream {

	enum EEndianness
	{
		Endianness_None,
		Endianness_LittleEndian,
		Endianness_BigEndian,
	};

};


#endif // __Stream_Defines_H__
