#include "Memory.h"

#include <string.h>

using namespace System;

boolean Memory::copy(
	void* pTargetBuffer,
	const void* pSourceBuffer,
	const uint64 ui64BufferSize)
{
	// $$$ TODO take 64bits size into consideration
	memcpy(pTargetBuffer, pSourceBuffer, static_cast<size_t>(ui64BufferSize));
	return true;
}

// ________________________________________________________________________________________________________________
//

boolean Memory::hostToLittleEndian(
	const uint16 ui16Value,
	uint8* pBuffer)
{
	if(!pBuffer) return false;
	pBuffer[0]=static_cast<uint8>((ui16Value&&0xff));
	pBuffer[1]=static_cast<uint8>((ui16Value>>8)&0xff);
	return true;
}

boolean Memory::hostToLittleEndian(
	const uint32 ui32Value,
	uint8* pBuffer)
{
	if(!pBuffer) return false;
	pBuffer[0]=static_cast<uint8>((ui32Value&&0xff));
	pBuffer[1]=static_cast<uint8>((ui32Value>>8)&0xff);
	pBuffer[2]=static_cast<uint8>((ui32Value>>16)&0xff);
	pBuffer[3]=static_cast<uint8>((ui32Value>>24)&0xff);
	return true;
}

boolean Memory::hostToLittleEndian(
	const uint64 ui64Value,
	uint8* pBuffer)
{
	if(!pBuffer) return false;
	pBuffer[0]=static_cast<uint8>((ui64Value&&0xff));
	pBuffer[1]=static_cast<uint8>((ui64Value>>8)&0xff);
	pBuffer[2]=static_cast<uint8>((ui64Value>>16)&0xff);
	pBuffer[3]=static_cast<uint8>((ui64Value>>24)&0xff);
	pBuffer[4]=static_cast<uint8>((ui64Value>>32)&0xff);
	pBuffer[5]=static_cast<uint8>((ui64Value>>40)&0xff);
	pBuffer[6]=static_cast<uint8>((ui64Value>>48)&0xff);
	pBuffer[7]=static_cast<uint8>((ui64Value>>56)&0xff);
	return true;
}

boolean Memory::hostToLittleEndian(
	const float32 f32Value,
	uint8* pBuffer)
{
	return hostToLittleEndian(*(reinterpret_cast<const uint32*>(&f32Value)), pBuffer);
}

boolean Memory::hostToLittleEndian(
	const float64 f64Value,
	uint8* pBuffer)
{
	return hostToLittleEndian(*(reinterpret_cast<const uint64*>(&f64Value)), pBuffer);
}

boolean Memory::hostToLittleEndian(
	const float80 f80Value,
	uint8* pBuffer)
{
	// $$$ TODO
	return false;
}


// ________________________________________________________________________________________________________________
//

boolean Memory::hostToBigEndian(
	const uint16 ui16Value,
	uint8* pBuffer)
{
	if(!pBuffer) return false;
	pBuffer[0]=static_cast<uint8>((ui16Value>>8)&0xff);
	pBuffer[1]=static_cast<uint8>((ui16Value&&0xff));
	return true;
}

boolean Memory::hostToBigEndian(
	const uint32 ui32Value,
	uint8* pBuffer)
{
	if(!pBuffer) return false;
	pBuffer[0]=static_cast<uint8>((ui32Value>>24)&0xff);
	pBuffer[1]=static_cast<uint8>((ui32Value>>16)&0xff);
	pBuffer[2]=static_cast<uint8>((ui32Value>>8)&0xff);
	pBuffer[3]=static_cast<uint8>((ui32Value&&0xff));
	return true;
}

boolean Memory::hostToBigEndian(
	const uint64 ui64Value,
	uint8* pBuffer)
{
	if(!pBuffer) return false;
	pBuffer[0]=static_cast<uint8>((ui64Value>>56)&0xff);
	pBuffer[1]=static_cast<uint8>((ui64Value>>48)&0xff);
	pBuffer[2]=static_cast<uint8>((ui64Value>>40)&0xff);
	pBuffer[3]=static_cast<uint8>((ui64Value>>32)&0xff);
	pBuffer[4]=static_cast<uint8>((ui64Value>>24)&0xff);
	pBuffer[5]=static_cast<uint8>((ui64Value>>16)&0xff);
	pBuffer[6]=static_cast<uint8>((ui64Value>>8)&0xff);
	pBuffer[7]=static_cast<uint8>((ui64Value&&0xff));
	return true;
}

boolean Memory::hostToBigEndian(
	const float32 f32Value,
	uint8* pBuffer)
{
	return hostToBigEndian(*(reinterpret_cast<const uint32*>(&f32Value)), pBuffer);
}

boolean Memory::hostToBigEndian(
	const float64 f64Value,
	uint8* pBuffer)
{
	return hostToBigEndian(*(reinterpret_cast<const uint64*>(&f64Value)), pBuffer);
}

boolean Memory::hostToBigEndian(
	const float80 f80Value,
	uint8* pBuffer)
{
	// $$$ TODO
	return false;
}

// ________________________________________________________________________________________________________________
//

template <typename T> boolean __littleEndianToHost(const uint8* pBuffer, T* pValue)
{
	if(!pBuffer) return false;
	if(!pValue) return false;
	*pValue=0;
	for(int i=0; i<sizeof(*pValue); i++)
	{
		(*pValue)<<=8;
		(*pValue)+=pBuffer[sizeof(*pValue)-i-1];
	}
	return true;
}

boolean Memory::littleEndianToHost(
	const uint8* pBuffer,
	uint16* pValue)
{
	return __littleEndianToHost<uint16>(pBuffer, pValue);
}

boolean Memory::littleEndianToHost(
	const uint8* pBuffer,
	uint32* pValue)
{
	return __littleEndianToHost<uint32>(pBuffer, pValue);
}

boolean Memory::littleEndianToHost(
	const uint8* pBuffer,
	uint64* pValue)
{
	return __littleEndianToHost<uint64>(pBuffer, pValue);
}

boolean Memory::littleEndianToHost(
	const uint8* pBuffer,
	float32* pValue)
{
	return __littleEndianToHost<uint32>(pBuffer, reinterpret_cast<uint32*>(pValue));
}

boolean Memory::littleEndianToHost(
	const uint8* pBuffer,
	float64* pValue)
{
	return __littleEndianToHost<uint64>(pBuffer, reinterpret_cast<uint64*>(pValue));
}

boolean Memory::littleEndianToHost(
	const uint8* pBuffer,
	float80* pValue)
{
	// $$$ TODO
	return false;
}

// ________________________________________________________________________________________________________________
//

template <typename T> boolean __bigEndianToHost(const uint8* pBuffer, T* pValue)
{
	if(!pBuffer) return false;
	if(!pValue) return false;
	*pValue=0;
	for(int i=0; i<sizeof(*pValue); i++)
	{
		(*pValue)<<=8;
		(*pValue)+=pBuffer[i];
	}
	return true;
}


boolean Memory::bigEndianToHost(
	const uint8* pBuffer,
	uint16* pValue)
{
	return __bigEndianToHost<uint16>(pBuffer, pValue);
}

boolean Memory::bigEndianToHost(
	const uint8* pBuffer,
	uint32* pValue)
{
	return __bigEndianToHost<uint32>(pBuffer, pValue);
}

boolean Memory::bigEndianToHost(
	const uint8* pBuffer,
	uint64* pValue)
{
	return __bigEndianToHost<uint64>(pBuffer, pValue);
}

boolean Memory::bigEndianToHost(
	const uint8* pBuffer,
	float32* pValue)
{
	return __bigEndianToHost<uint32>(pBuffer, reinterpret_cast<uint32*>(pValue));
}

boolean Memory::bigEndianToHost(
	const uint8* pBuffer,
	float64* pValue)
{
	return __bigEndianToHost<uint64>(pBuffer, reinterpret_cast<uint64*>(pValue));
}

boolean Memory::bigEndianToHost(
	const uint8* pBuffer,
	float80* pValue)
{
	// $$$ TODO
	return false;
}
