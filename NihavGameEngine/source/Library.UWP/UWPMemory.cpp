#include "pch.h"
#include "UWPMemory.h"

namespace Library
{
	void* UWPMemory::OSMalloc(std::uint32_t numBytes)
	{
		return malloc(numBytes);
	}

	void UWPMemory::OSFree(void* ptr)
	{
		free(ptr);
	}
}
