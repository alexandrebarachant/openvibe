
#ifdef TARGET_HAS_Boost
#include <boost/static_assert.hpp>
#endif

// Appeasing Intellisense by including the following
#include "ov_common_defines.h"

#define OV_APPEND_TO_NAMESPACE OpenViBETypeTest
#include "ov_common_types.h"
#undef OV_APPEND_TO_NAMESPACE

using namespace OpenViBETypeTest;

int main(void) 
{
#ifdef TARGET_HAS_Boost
	// static (on compile time) test of types here. 
	// Note that these are legacy tests, kept until all compilers have moved to C++0x. 
	BOOST_STATIC_ASSERT(sizeof(uint64)!=sizeof(uint32));
	BOOST_STATIC_ASSERT(sizeof(uint64)!=sizeof(uint16));
	BOOST_STATIC_ASSERT(sizeof(uint64)!=sizeof(uint8));
	BOOST_STATIC_ASSERT(sizeof(uint32)!=sizeof(uint16));
	BOOST_STATIC_ASSERT(sizeof(uint32)!=sizeof(uint8));
	BOOST_STATIC_ASSERT(sizeof(uint16)!=sizeof(uint8));
	
	BOOST_STATIC_ASSERT(sizeof(int64)!=sizeof(int32));
	BOOST_STATIC_ASSERT(sizeof(int64)!=sizeof(int16));
	BOOST_STATIC_ASSERT(sizeof(int64)!=sizeof(int8));
	BOOST_STATIC_ASSERT(sizeof(int32)!=sizeof(int16));
	BOOST_STATIC_ASSERT(sizeof(int32)!=sizeof(int8));
	BOOST_STATIC_ASSERT(sizeof(int16)!=sizeof(int8));
	
#ifndef TARGET_OS_Windows
	BOOST_STATIC_ASSERT(sizeof(float80)!=sizeof(float64));
	BOOST_STATIC_ASSERT(sizeof(float80)!=sizeof(float32));
#endif
	BOOST_STATIC_ASSERT(sizeof(float64)!=sizeof(float32));
#endif

#if defined(WIN32) || (defined(LINUX) && defined(__GXX_EXPERIMENTAL_CXX0X__))
	static_assert(sizeof(uint64)>=8, "uint64 is not at least 8 bytes");
	static_assert(sizeof(uint32)>=4, "uint32 is not at least 4 bytes");
	static_assert(sizeof(uint16)>=2, "uint16 is not at least 2 bytes");
	static_assert(sizeof(uint8) >=1, "uint8 is not at least 1 byte");

	static_assert(sizeof(int64)>=8, "int64 is not at least 8 bytes");
	static_assert(sizeof(int32)>=4, "int32 is not at least 4 bytes");
	static_assert(sizeof(int16)>=2, "int16 is not at least 2 bytes");
	static_assert(sizeof(int8) >=1, "int8 is not at least 1 byte");

#if defined(LINUX)
	static_assert(sizeof(float80)>=10, "float80 is not at least 10 bytes");
#endif
	// Float80 seems to be the same size as float64 on Win at the time of writing, but its not widely used in openvibe.	
	static_assert(sizeof(float64)>=8, "float64 is not at least 8 bytes");
	static_assert(sizeof(float32)>=4, "float32 is not at least 4 bytes");
#endif

	return 0;
}

