#include "pch.h"
#include "CustomHeap.h"

namespace Library
{
	CustomHeap::CustomHeap() :
		mHeapName(), mHeapStart(nullptr), mHeapSize(0U), mHeapID(0U),
		mNumBlocks(0U), mFirstFreeBlock(nullptr), mLastFreeBlock(nullptr)
	{}

	CustomHeap::~CustomHeap()
	{}

	void CustomHeap::Init(const std::string& heapName, void* startAddress, std::uint32_t heapSize, std::uint32_t heapId)
	{
		mHeapName = heapName;
		mHeapStart = startAddress;
		mHeapSize = heapSize;
		mHeapID = heapId;

		InitFirstBlock();

		mNumBlocks = 1;
	}

	const std::string& CustomHeap::HeapName() const
	{
		return mHeapName;
	}

	std::size_t CustomHeap::HeapSize() const
	{
		return mHeapSize;
	}

	std::uint32_t CustomHeap::HeapId() const
	{
		return mHeapID;
	}

	std::uint32_t CustomHeap::NumBlocks() const
	{
		return mNumBlocks;
	}

	void* CustomHeap::HeapStart()
	{
		return mHeapStart;
	}

	void* CustomHeap::AllocateBlock(std::size_t bytes, const std::string& filename, std::uint32_t lineNumber, const std::string& tag, const std::chrono::high_resolution_clock::time_point& time, std::uint32_t alignment)
	{
		void* thisBlock = AllocateBlock(bytes, alignment);

		UNREFERENCED_PARAMETER(filename);
		UNREFERENCED_PARAMETER(tag);
		UNREFERENCED_PARAMETER(lineNumber);
		UNREFERENCED_PARAMETER(time);
#ifdef _DEBUG
		MemoryBlock& thisMemoryBlock = BeginningOfBlockFromPtr(thisBlock);
		//thisMemoryBlock.Tag = tag;
		//thisMemoryBlock.Filename = filename;
		thisMemoryBlock.LineNumber = lineNumber;
		thisMemoryBlock.Time = time;
#endif
		return thisBlock;
	}

	void CustomHeap::FreeBlock(void* ptr)
	{
		MemoryBlock& thisBlock = BeginningOfBlockFromPtr(ptr);
		FreeBlock(thisBlock);
	}

	bool CustomHeap::IsHeapConsistent() const
	{
		MemoryBlock* addr = reinterpret_cast<MemoryBlock*>(mHeapStart);
		std::uint32_t count = 1;
		void* prev = nullptr;

		while (addr->NextBlock != nullptr)
		{
			++count;
			assert(prev == addr->PrevBlock);
			prev = addr;
			addr = reinterpret_cast<MemoryBlock*>(addr->NextBlock);
		}
		return (count == mNumBlocks);
	}

	void CustomHeap::FreeAllBlocks()
	{
		MemoryBlock* addr = reinterpret_cast<MemoryBlock*>(mHeapStart);
		if (!IsBlockFree(*addr))
			FreeBlock(*addr);

		addr = reinterpret_cast<MemoryBlock*>(addr->NextBlock);
		while(!(mNumBlocks == 1 && NumFreeBlocks() == 1 && addr == nullptr))
		{
			FreeBlock(*addr);
			addr = reinterpret_cast<MemoryBlock*>(reinterpret_cast<MemoryBlock*>(mHeapStart)->NextBlock);
		}
	}

	CustomHeap::MemoryBlock& CustomHeap::MemoryBlockFromPtr(void* ptr)
	{
		MemoryBlock* returnVal = nullptr;
		std::size_t* tempPtr = reinterpret_cast<std::size_t*>(ptr);
		if (*(tempPtr - 1) == ALIGNMENT_BYTES)
		{
			tempPtr--;
			while (*tempPtr == ALIGNMENT_BYTES)
				tempPtr--;
			tempPtr++;
			returnVal = reinterpret_cast<MemoryBlock*>(reinterpret_cast<std::size_t>(tempPtr) - FrontBlockOverhead());
		}
		else
		{
			returnVal = reinterpret_cast<MemoryBlock*>(reinterpret_cast<std::size_t>(ptr) - FrontBlockOverhead());
		}
		return *returnVal;
	}

	std::uint32_t CustomHeap::HeapID(const MemoryBlock& block)
	{
		std::uint32_t id = block.Flags;
		id >>= 1;
		id &= 0xFF;

		return id;
	}

	void CustomHeap::InitFirstBlock()
	{
		MemoryBlock& address = *(reinterpret_cast<MemoryBlock*>(mHeapStart));
		address.Flags = NULL;
		SetFlag(FREE_FLAG, address);

		address.NextBlock = nullptr;
		address.PrevBlock = nullptr;

#if _DEBUG
		//address.Filename = "";
		address.LineNumber = 0;
		address.Time = std::chrono::high_resolution_clock::now();
		//address.Tag = "";
#endif

		mFirstFreeBlock = &address;
		mLastFreeBlock = &address;
	}

	void CustomHeap::SetFlag(std::uint32_t flagNumber, MemoryBlock& block)
	{
		block.Flags = block.Flags | (1 << flagNumber);
	}

	std::uint32_t CustomHeap::GetFlag(std::uint32_t flagNumber, const MemoryBlock& block) const
	{
		return (block.Flags & (1 << flagNumber));
	}

	void CustomHeap::ClearFlag(std::uint32_t flagNumber, MemoryBlock& block)
	{
		block.Flags = block.Flags & (0xFFFFFFFF - (1 << flagNumber));
	}

	bool CustomHeap::IsBlockFree(const MemoryBlock& block) const
	{
		return (GetFlag(FREE_FLAG, block) == 1) ? true : false;
	}

	void CustomHeap::MakeBlockFree(MemoryBlock& block)
	{
		SetFlag(FREE_FLAG, block);
	}

	std::size_t CustomHeap::BlockSize(const MemoryBlock& block) const
	{
		std::size_t blockSize = 0;
		std::size_t endAddress = 0;

		if (block.NextBlock == nullptr)
		{
			endAddress = reinterpret_cast<std::size_t>(mHeapStart) + mHeapSize;
			blockSize = endAddress - reinterpret_cast<std::size_t>(&block);
		}
		else
		{
			blockSize = reinterpret_cast<std::size_t>(block.NextBlock) - reinterpret_cast<std::size_t>(&block);
		}

		return blockSize;
	}

	void CustomHeap::ClearBlock(MemoryBlock& block)
	{
		void* dataStartAddress = reinterpret_cast<void*>(reinterpret_cast<std::size_t>(&block) + FrontBlockOverhead());
		void* endAddress = nullptr;

		if (block.NextBlock == nullptr)
			endAddress = reinterpret_cast<void*>(reinterpret_cast<std::size_t>(mHeapStart) + mHeapSize);
		else
			endAddress = block.NextBlock;
		
		std::uint8_t* temp = reinterpret_cast<std::uint8_t*>(dataStartAddress);
		std::uint8_t* lastByteAddress = reinterpret_cast<std::uint8_t*>(endAddress);

		while (temp != lastByteAddress)
		{
			*temp = CLEAR_BYTE;
			temp++;
		}
	}

	void CustomHeap::CombineBlocks(MemoryBlock& firstBlock, MemoryBlock& secondBlock)
	{
		assert(reinterpret_cast<std::size_t>(&firstBlock) < reinterpret_cast<std::size_t>(&secondBlock));
		assert(firstBlock.NextBlock == &secondBlock);
		assert(IsBlockFree(firstBlock) && IsBlockFree(secondBlock));

		firstBlock.NextBlock = secondBlock.NextBlock;
		if (secondBlock.NextBlock != nullptr)
		{
			reinterpret_cast<MemoryBlock*>(secondBlock.NextBlock)->PrevBlock = &firstBlock;
		}

		ClearBlock(firstBlock);
		--mNumBlocks;
	}

	void CustomHeap::SetHeapID(MemoryBlock& block, std::uint32_t id)
	{
		id <<= 1;
		block.Flags &= 0xFFFFF801;	//clear heap bits
		block.Flags |= id;
	}

	std::uint32_t CustomHeap::PowerOfTwo(std::uint32_t value)
	{
		assert(value & (value - 1));
		return value;
	}

	std::uint32_t CustomHeap::FrontBlockOverhead()
	{
		return sizeof(MemoryBlock);
	}

	std::uint32_t CustomHeap::RearBlockOverhead()
	{
		return NUM_GUARD_LONGS * 4U;
	}

	std::uint32_t CustomHeap::BlockOverhead()
	{
		return FrontBlockOverhead() + RearBlockOverhead();
	}

	CustomHeap::MemoryBlock& CustomHeap::BeginningOfBlockFromPtr(void* ptr) const
	{
		std::uint32_t* tempPtr = reinterpret_cast<std::uint32_t*>(ptr);
		--tempPtr;

		while (*tempPtr == 0)
			--tempPtr;

		++tempPtr;

		MemoryBlock* returnBlock = reinterpret_cast<MemoryBlock*>(reinterpret_cast<std::size_t>(tempPtr) - FrontBlockOverhead());
		return *returnBlock;
	}

	void* CustomHeap::AllocateBlock(std::size_t bytes, std::uint32_t alignment)
	{
		bytes = (bytes + 3)&(0xFFFFFFFF - 3);   //always ask for a multiple of 4 bytes just to simplify life
		assert(bytes % 4 == 0);
		bool blockFound = false;
		void* returnBlock = nullptr;

		std::size_t alignmentOffset = (alignment == 4U) ? 0 : PowerOfTwo(alignment);
		std::size_t actualBytesNeeded = bytes + BlockOverhead() + alignmentOffset;

		MemoryBlock* addr = mFirstFreeBlock;
		MemoryBlock* oldNextBlock = nullptr;

		while (!blockFound)
		{
			if (IsBlockFree(*addr))
			{
				if (BlockSize(*addr) > (actualBytesNeeded + BlockOverhead()))
				{
					blockFound = true;
					returnBlock = addr;

					oldNextBlock = (addr->NextBlock != nullptr) ? reinterpret_cast<MemoryBlock*>(addr->NextBlock) : nullptr;
					addr->NextBlock = reinterpret_cast<void*>(reinterpret_cast<std::size_t>(addr) + actualBytesNeeded);
					
					ClearFlag(FREE_FLAG, *addr);
					SetHeapID(*addr, mHeapID);
					
					assert((oldNextBlock == nullptr) || (reinterpret_cast<std::size_t>(oldNextBlock) >= reinterpret_cast<std::size_t>(addr->NextBlock)));

					if (oldNextBlock == nullptr || reinterpret_cast<std::size_t>(oldNextBlock) != reinterpret_cast<std::size_t>(addr->NextBlock))
					{

						addr = reinterpret_cast<MemoryBlock*>(addr->NextBlock);
						addr->PrevBlock = reinterpret_cast<MemoryBlock*>(returnBlock);
						addr->NextBlock = oldNextBlock;
						SetFlag(FREE_FLAG, *addr);
						if (mFirstFreeBlock == returnBlock)
						{
							mFirstFreeBlock = addr;
						}
						if (oldNextBlock != nullptr)
						{
							oldNextBlock->PrevBlock = addr;
						}

						++mNumBlocks;
					}
				}
			}
			addr = reinterpret_cast<MemoryBlock*>(addr->NextBlock);
			if (addr == nullptr && !blockFound)
			{
				assert(false);	// no block is large enough
				// Defragment;
			}
		}

		std::size_t returnPtrVal = reinterpret_cast<std::size_t>(returnBlock) + FrontBlockOverhead();
		std::size_t* origPtr = reinterpret_cast<std::size_t*>(returnPtrVal);

		if (alignmentOffset != 0)
		{
			returnPtrVal += (alignmentOffset - 1);
			returnPtrVal = returnPtrVal & (ALL_F - alignmentOffset + 1);

			std::size_t* newPtrVal = reinterpret_cast<std::size_t*>(returnPtrVal);
			while (origPtr != newPtrVal)
			{
				*origPtr++ = ALIGNMENT_BYTES;
			}
		}

#ifdef _DEBUG
		MemoryBlock* tempBlock = reinterpret_cast<MemoryBlock*>(returnBlock);
		tempBlock = reinterpret_cast<MemoryBlock*>(tempBlock->NextBlock);

		std::size_t* clearPtr = reinterpret_cast<std::size_t*>(tempBlock);
		clearPtr -= NUM_GUARD_LONGS;
		for (std::uint32_t i = 0; i < NUM_GUARD_LONGS; ++i)
		{
			*clearPtr++ = GUARD_VAL;
		}
#endif
		return reinterpret_cast<void*>(returnPtrVal);
	}

	void CustomHeap::FreeBlock(MemoryBlock& block)
	{
		if (IsBlockFree(block))
			return;

		MakeBlockFree(block);
		if (&block < mFirstFreeBlock)
		{
			mFirstFreeBlock = &block;
		}

		MemoryBlock* nextBlock = reinterpret_cast<MemoryBlock*>(block.NextBlock);
		if (nextBlock != nullptr)
		{
			if (IsBlockFree(*nextBlock))
				CombineBlocks(block, *nextBlock);
		}

		MemoryBlock* prevBlock = reinterpret_cast<MemoryBlock*>(block.PrevBlock);
		if (prevBlock != nullptr)
		{
			if (IsBlockFree(*prevBlock))
			{
				CombineBlocks(*prevBlock, block);
				if (prevBlock < mFirstFreeBlock)
					mFirstFreeBlock = prevBlock;
			}
		}
	}

	std::uint32_t CustomHeap::NumFreeBlocks() const
	{
		std::uint32_t count = 0;
		MemoryBlock* addr = reinterpret_cast<MemoryBlock*>(mHeapStart);
		do
		{
			if (IsBlockFree(*addr))
			{
				++count;
			}
			addr = reinterpret_cast<MemoryBlock*>(addr->NextBlock);
		} while (addr != nullptr);
		return count;
	}

	std::size_t CustomHeap::TotalFreeMemory() const
	{
		std::size_t freeBytes = 0;
		MemoryBlock* addr = reinterpret_cast<MemoryBlock*>(mHeapStart);
		do
		{
			if (IsBlockFree(*addr))
			{
				freeBytes += BlockSize(*addr);
			}
			addr = reinterpret_cast<MemoryBlock*>(addr->NextBlock);
		} while (addr != nullptr);
		return freeBytes;
	}

	std::size_t CustomHeap::LargestFreeBlockSize() const
	{
		std::size_t maxFreeBytes = 0;
		MemoryBlock* addr = reinterpret_cast<MemoryBlock*>(mHeapStart);
		do
		{
			if (IsBlockFree(*addr))
			{
				std::size_t blockSize = BlockSize(*addr);
				if (blockSize > maxFreeBytes)
					maxFreeBytes = blockSize;
			}
			addr = reinterpret_cast<MemoryBlock*>(addr->NextBlock);
		} while (addr != nullptr);
		return maxFreeBytes;
	}

#ifdef _DEBUG
	bool CustomHeap::CheckGuardBytes(const MemoryBlock& block) const
	{
		std::size_t* checkAddr = nullptr;
		if (block.NextBlock == nullptr)
			checkAddr = reinterpret_cast<std::size_t*>(reinterpret_cast<std::size_t>(mHeapStart) + mHeapSize);
		else
			checkAddr = reinterpret_cast<std::size_t*>(block.NextBlock);

		checkAddr -= NUM_GUARD_LONGS;
		for (std::uint32_t i = 0; i < NUM_GUARD_LONGS; ++i)
		{
			if (*checkAddr++ != GUARD_VAL)
				return false;
		}
		return true;
	}
#endif
}
