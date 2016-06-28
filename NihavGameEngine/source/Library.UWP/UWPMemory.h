#pragma once
#include "Memory.h"

namespace Library
{
	class UWPMemory : public Memory
	{
	public:
		UWPMemory() = default;
		virtual ~UWPMemory() = default;

		virtual void* OSMalloc(std::uint32_t numBytes) override;
		virtual void OSFree(void* ptr) override;
	};
}

