#pragma once
#include <vector>
#include "CustomHeap.h"

namespace Library
{
	class Memory;
	class HeapManager
	{
	public:
		struct HeapDef
		{
			std::uint32_t HeapID;
			std::string Name;
			std::uint32_t HeapSize;
		};

		static void CreateHeapManager(Memory& memory);
		static HeapManager* GetHeapManager();
		static void DestroyHeapManager();

		void Init(const std::vector<HeapDef>& heapDefs, std::uint32_t defaultHeapId);
		std::size_t GetHeapSize(std::uint32_t id) const;
		const std::string& GetHeapName(std::uint32_t id) const;
		std::uint32_t GetHeapNumBlocks(std::uint32_t id) const;

		void* AllocateBlock(std::uint32_t heapId, std::size_t bytes, const std::string& filename, std::uint32_t linenumber, const std::string& tag, const std::chrono::high_resolution_clock::time_point& time, std::uint32_t alignment = 4);
		void FreeBlock(void* ptr);

		void SetCurrentHeapID(std::uint32_t id);
		void RestoreHeapID();
		//Memory& GetMemoryRef();

		bool IsHeapConsistent(std::uint32_t heapID) const;

	private:
		HeapManager(Memory& memory);
		~HeapManager();

		static HeapManager* sInstance;

		void InitHeaps(const std::vector<HeapDef>& heapDefs);
		CustomHeap& GetHeap(std::uint32_t id);
		const CustomHeap& GetHeap(std::uint32_t id) const;

		std::uint32_t mOldHeapID;
		std::uint32_t mCurrentHeapID;
		std::uint32_t mDefaultHeapID;

		std::vector<CustomHeap*> mHeaps;

		Memory* mMemory;
		CustomHeap mHeap;
		std::uint32_t mNumHeaps;

		static const std::uint32_t HEAP_SIZE = 10240;
		static const std::string HEAP_HEAP_NAME;
		static const std::uint32_t HEAP_HEAP_ID = 2U;
	};

}
