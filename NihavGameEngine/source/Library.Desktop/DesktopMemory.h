#pragma once
#include "../Library.Shared/Memory.h"

namespace Library
{
	class DesktopMemory : public Memory
	{
	public:
		DesktopMemory() = default;
		virtual ~DesktopMemory() = default;

		virtual void* OSMalloc(std::uint32_t numBytes) override;
		virtual void OSFree(void* ptr) override;
	};
}

