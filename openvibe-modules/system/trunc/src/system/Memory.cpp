#include "Memory.h"

#include <string.h>

using namespace System;

boolean Memory::copy(
	void* pTargetBuffer,
	const void* pSourceBuffer,
	const uint64 ui64BufferSize)
{
	memcpy(pTargetBuffer, pSourceBuffer, ui64BufferSize);
	return true;
}
