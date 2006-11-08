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
