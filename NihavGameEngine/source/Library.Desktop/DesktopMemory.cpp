#include "pch.h"
#include "DesktopMemory.h"

namespace Library
{
	void* DesktopMemory::OSMalloc(std::uint32_t numBytes)
	{
		return malloc(numBytes);
	}

	void DesktopMemory::OSFree(void* ptr)
	{
		free(ptr);
	}
}