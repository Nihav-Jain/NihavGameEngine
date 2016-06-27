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

	std::uint32_t CustomHeap::HeapSize() const
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

	void* CustomHeap::AllocateBlock(std::uint32_t bytes, const std::string& filename, std::uint32_t lineNumber, const std::string& tag, const std::chrono::high_resolution_clock::time_point& time, std::uint32_t alignment)
	{
		void* thisBlock = AllocateBlock(bytes, alignment);

#ifdef _DEBUG
		MemoryBlock& thisMemoryBlock = BeginningOfBlockFromPtr(thisBlock);
		thisMemoryBlock.Tag = tag;
		thisMemoryBlock.Filename = filename;
		thisMemoryBlock.LineNumber = lineNumber;
		thisMemoryBlock.Time = time;
#endif
		return thisBlock;
	}

	void CustomHeap::FreeBlock(void* ptr)
	{
		MemoryBlock& thisBlock = BeginningOfBlockFromPtr(ptr);

		assert(!IsBlockFree(thisBlock));

#ifdef _DEBUG
#endif
		MakeBlockFree(thisBlock);
		if (&thisBlock < mFirstFreeBlock)
		{
			mFirstFreeBlock = &thisBlock;
		}

		MemoryBlock* nextBlock = reinterpret_cast<MemoryBlock*>(thisBlock.NextBlock);
		if (nextBlock != nullptr)
		{
			if (IsBlockFree(*nextBlock))
				CombineBlocks(thisBlock, *nextBlock);
		}

		MemoryBlock* prevBlock = reinterpret_cast<MemoryBlock*>(thisBlock.PrevBlock);
		if (prevBlock != nullptr)
		{
			if (IsBlockFree(*prevBlock))
			{
				CombineBlocks(*prevBlock, thisBlock);
				if (prevBlock < mFirstFreeBlock)
					mFirstFreeBlock = prevBlock;
			}
		}
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

	CustomHeap::MemoryBlock& CustomHeap::MemoryBlockFromPtr(void* ptr)
	{
		MemoryBlock* returnVal = nullptr;
		std::uint32_t* tempPtr = reinterpret_cast<std::uint32_t*>(ptr);
		if (*(tempPtr - 1) == 0x00000000)
		{
			tempPtr--;
			while (*tempPtr == 0x00000000)
				tempPtr--;
			tempPtr++;
			returnVal = reinterpret_cast<MemoryBlock*>(reinterpret_cast<std::uint32_t>(tempPtr) - FrontBlockOverhead());
		}
		else
		{
			returnVal = reinterpret_cast<MemoryBlock*>(reinterpret_cast<std::uint32_t>(ptr) - FrontBlockOverhead());
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
		MemoryBlock& address = *(static_cast<MemoryBlock*>(mHeapStart));
		address.Flags = NULL;
		SetFlag(FREE_FLAG, address);

		address.NextBlock = nullptr;
		address.PrevBlock = nullptr;

#if _DEBUG
		address.Filename = "";
		address.LineNumber = 0;
		address.Time = std::chrono::high_resolution_clock::now();
		address.Tag = "";
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

	std::uint32_t CustomHeap::BlockSize(const MemoryBlock& block) const
	{
		std::uint32_t blockSize = 0;
		std::uint32_t endAddress = 0;

		if (block.NextBlock == nullptr)
		{
			endAddress = reinterpret_cast<std::uint32_t>(mHeapStart) + mHeapSize;
			blockSize = endAddress - reinterpret_cast<std::uint32_t>(&block);
		}
		else
		{
			blockSize = reinterpret_cast<std::uint32_t>(block.NextBlock) - reinterpret_cast<std::uint32_t>(&block);
		}

		return blockSize;
	}

	void CustomHeap::ClearBlock(MemoryBlock& block)
	{
		void* dataStartAddress = reinterpret_cast<void*>(reinterpret_cast<std::uint32_t>(&block) + FrontBlockOverhead());
		void* endAddress = nullptr;

		if (block.NextBlock == nullptr)
			endAddress = reinterpret_cast<void*>(reinterpret_cast<std::uint32_t>(mHeapStart) + mHeapSize);
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
		assert(reinterpret_cast<std::uint32_t>(&firstBlock) < reinterpret_cast<std::uint32_t>(&secondBlock));
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

		MemoryBlock* returnBlock = reinterpret_cast<MemoryBlock*>(reinterpret_cast<std::uint32_t>(tempPtr) - FrontBlockOverhead());
		return *returnBlock;
	}

	void* CustomHeap::AllocateBlock(std::uint32_t bytes, std::uint32_t alignment)
	{
		bytes = (bytes + 3)&(0xFFFFFFFF - 3);   //always ask for a multiple of 4 bytes just to simplify life
		bool blockFound = false;
		void* returnBlock = nullptr;

		std::uint32_t alignmentOffset = (alignment == 4U) ? 0 : PowerOfTwo(alignment);
		std::uint32_t actualBytesNeeded = bytes + BlockOverhead() + alignmentOffset;

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

					oldNextBlock = reinterpret_cast<MemoryBlock*>(addr->NextBlock);
					addr->NextBlock = reinterpret_cast<void*>(reinterpret_cast<std::uint32_t>(addr) + actualBytesNeeded);
					
					ClearFlag(FREE_FLAG, *addr);
					SetHeapID(*addr, mHeapID);
					
					assert((oldNextBlock == nullptr) || (reinterpret_cast<std::uint32_t>(oldNextBlock) >= reinterpret_cast<std::uint32_t>(addr->NextBlock)));

					if (reinterpret_cast<std::uint32_t>(oldNextBlock) != reinterpret_cast<std::uint32_t>(addr->NextBlock))
					{
						addr = reinterpret_cast<MemoryBlock*>(addr->NextBlock);
						addr->PrevBlock = reinterpret_cast<MemoryBlock*>(returnBlock);
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

		std::uint32_t returnPtrVal = reinterpret_cast<std::uint32_t>(returnBlock) + FrontBlockOverhead();
		std::uint32_t* origPtr = reinterpret_cast<std::uint32_t*>(returnPtrVal);

		if (alignmentOffset != 0)
		{
			returnPtrVal += (alignmentOffset - 1);
			returnPtrVal = returnPtrVal & (0xFFFFFFFF - alignmentOffset + 1);

			std::uint32_t* newPtrVal = reinterpret_cast<std::uint32_t*>(returnPtrVal);
			while (origPtr != newPtrVal)
			{
				*origPtr++ = 0x00000000;
			}
		}

#ifdef _DEBUG
		MemoryBlock* tempBlock = reinterpret_cast<MemoryBlock*>(returnBlock);
		tempBlock = reinterpret_cast<MemoryBlock*>(tempBlock->NextBlock);

		std::uint32_t* clearPtr = reinterpret_cast<std::uint32_t*>(tempBlock);
		clearPtr -= NUM_GUARD_LONGS;
		for (std::uint32_t i = 0; i < NUM_GUARD_LONGS; ++i)
		{
			*clearPtr++ = GUARD_VAL;
		}
#endif
		return returnBlock;
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

	std::uint32_t CustomHeap::TotalFreeMemory() const
	{
		std::uint32_t freeBytes = 0;
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

	std::uint32_t CustomHeap::LargestFreeBlockSize() const
	{
		std::uint32_t maxFreeBytes = 0;
		MemoryBlock* addr = reinterpret_cast<MemoryBlock*>(mHeapStart);
		do
		{
			if (IsBlockFree(*addr))
			{
				std::uint32_t blockSize = BlockSize(*addr);
				if (blockSize > maxFreeBytes)
					maxFreeBytes = blockSize;
			}
			addr = reinterpret_cast<MemoryBlock*>(addr->NextBlock);
		} while (addr != nullptr);
		return maxFreeBytes;
	}

	bool CustomHeap::CheckGuardBytes(const MemoryBlock& block) const
	{
		std::uint32_t* checkAddr = nullptr;
		if (block.NextBlock == nullptr)
			checkAddr = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint32_t>(mHeapStart) + mHeapSize);
		else
			checkAddr = reinterpret_cast<std::uint32_t*>(block.NextBlock);

		checkAddr -= NUM_GUARD_LONGS;
		for (std::uint32_t i = 0; i < NUM_GUARD_LONGS; ++i)
		{
			if (*checkAddr++ != GUARD_VAL)
				return false;
		}
		return true;
	}
}
