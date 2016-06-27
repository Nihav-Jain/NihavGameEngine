#pragma once
#include <string>
#include <cstdint>
#include <chrono>

namespace Library
{
	class CustomHeap
	{
	public:

		struct MemoryBlock
		{
		public:
			void* PrevBlock;
			void* NextBlock;
			std::uint32_t Flags;
#ifdef _DEBUG
			std::string Filename;
			std::string Tag;
			std::uint32_t LineNumber;
			std::chrono::high_resolution_clock::time_point Time;
#endif
		};

		CustomHeap();
		~CustomHeap();

		CustomHeap(const CustomHeap& rhs) = delete;
		CustomHeap& operator=(const CustomHeap& rhs) = delete;

		void Init(const std::string& heapName, void* startAddress, std::uint32_t heapSize, std::uint32_t heapId);

		const std::string& HeapName() const;
		std::uint32_t HeapSize() const;
		std::uint32_t HeapId() const;
		std::uint32_t NumBlocks() const;

		void* AllocateBlock(std::uint32_t bytes, const std::string& filename, std::uint32_t lineNumber, const std::string& tag, const std::chrono::high_resolution_clock::time_point& time, std::uint32_t alignment = 4);
		void FreeBlock(void* ptr);
		bool IsHeapConsistent() const;

		static MemoryBlock& MemoryBlockFromPtr(void* ptr);
		static std::uint32_t HeapID(const MemoryBlock& block);

	private:
		void InitFirstBlock();

		void SetFlag(std::uint32_t flagNumber, MemoryBlock& block);
		std::uint32_t GetFlag(std::uint32_t flagNumber, const MemoryBlock& block) const;
		void ClearFlag(std::uint32_t flagNumber, MemoryBlock& block);
		bool IsBlockFree(const MemoryBlock& block) const;
		void MakeBlockFree(MemoryBlock& block);

		std::uint32_t BlockSize(const MemoryBlock& block) const;
		void ClearBlock(MemoryBlock& block);
		void CombineBlocks(MemoryBlock& firstBlock, MemoryBlock& secondBlock);

		void SetHeapID(MemoryBlock& block, std::uint32_t id);

		static std::uint32_t PowerOfTwo(std::uint32_t value);
		static std::uint32_t FrontBlockOverhead();
		static std::uint32_t RearBlockOverhead();
		static std::uint32_t BlockOverhead();

		MemoryBlock& BeginningOfBlockFromPtr(void* ptr) const;
		void* AllocateBlock(std::uint32_t bytes, std::uint32_t alignment = 4U);

		std::uint32_t NumFreeBlocks() const;
		std::uint32_t TotalFreeMemory() const;
		std::uint32_t LargestFreeBlockSize() const;

#ifdef _DEBUG
		bool CheckGuardBytes(const MemoryBlock& block) const;
#endif

		std::string mHeapName;
		void* mHeapStart;
		std::uint32_t mHeapSize;
		std::uint32_t mHeapID;
		std::uint32_t mNumBlocks;
		MemoryBlock* mFirstFreeBlock;
		MemoryBlock* mLastFreeBlock;

		static const std::uint8_t FREE_FLAG = 0;
		static const std::uint8_t CLEAR_BYTE = 0xEA;
		static const std::uint32_t NUM_GUARD_LONGS = 16;
		static const std::uint32_t GUARD_VAL = 0xFACEFACE;
	};
}

