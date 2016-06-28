#include "pch.h"
#include "Memory.h"
#include "HeapManager.h"

namespace Library
{
	HeapManager* HeapManager::sInstance = nullptr;
	const std::string HeapManager::HEAP_HEAP_NAME = "TheHeapHeap";

	HeapManager::HeapManager(Memory& memory) :
		mMemory(&memory), mHeap(), mHeaps()
	{}

	void HeapManager::InitHeaps(const std::vector<HeapDef>& heapDefs)
	{
		void* heapHeapStart = mMemory->OSMalloc(HEAP_SIZE);
		mHeap.Init(HEAP_HEAP_NAME, heapHeapStart, HEAP_SIZE, HEAP_HEAP_ID);
		mHeaps.push_back(&mHeap);

		for (const HeapDef& heapDef : heapDefs)
		{
			CustomHeap* thisHeap = ENGINE_NEW(HEAP_HEAP_ID, "DEF") CustomHeap();
			mHeaps.push_back(thisHeap);
			void* startHeapPtr = mMemory->OSMalloc(heapDef.HeapSize);
			thisHeap->Init(heapDef.Name, startHeapPtr, heapDef.HeapSize, heapDef.HeapID);
			assert(mHeap.IsHeapConsistent());
		}
	}

	CustomHeap& HeapManager::GetHeap(std::uint32_t id)
	{
		for (CustomHeap* heap : mHeaps)
		{
			if (heap->HeapId() == id)
				return *heap;
		}
		throw std::exception("Heap not found");
		//return CustomHeap();
	}

	const CustomHeap& HeapManager::GetHeap(std::uint32_t id) const
	{
		for (CustomHeap* heap : mHeaps)
		{
			if (heap->HeapId() == id)
				return *heap;
		}
		throw std::exception("Heap not found");
		//return CustomHeap();
	}

	void HeapManager::CreateHeapManager(Memory& memory)
	{
		if (sInstance == nullptr)
			sInstance = new HeapManager(memory);
	}

	HeapManager* HeapManager::GetHeapManager()
	{
		return sInstance;
	}

	void HeapManager::DestroyHeapManager()
	{
		if(sInstance != nullptr)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

	HeapManager::~HeapManager()
	{
		if (!mHeaps[0]->IsHeapConsistent())
		{
			assert(false);
		}

		std::uint32_t i;
		for (i = 1; i < mHeaps.size(); ++i)
		{
			mHeaps[i]->FreeAllBlocks();
			mMemory->OSFree(mHeaps[i]->HeapStart());
			ENGINE_DELETE(mHeaps[i]);
		}
		mHeaps[0]->FreeAllBlocks();
		mMemory->OSFree(mHeaps[0]->HeapStart());
	}

	void HeapManager::Init(const std::vector<HeapDef>& heapDefs, std::uint32_t defaultHeapId)
	{
		mDefaultHeapID = defaultHeapId;
		mCurrentHeapID = mDefaultHeapID;
		mOldHeapID = mDefaultHeapID;

		mNumHeaps = heapDefs.size();
		InitHeaps(heapDefs);
	}

	std::uint32_t HeapManager::GetHeapSize(std::uint32_t id) const
	{
		return GetHeap(id).HeapSize();
	}

	const std::string& HeapManager::GetHeapName(std::uint32_t id) const
	{
		return GetHeap(id).HeapName();
	}

	std::uint32_t HeapManager::GetHeapNumBlocks(std::uint32_t id) const
	{
		return GetHeap(id).NumBlocks();
	}

	void* HeapManager::AllocateBlock(std::uint32_t heapId, std::uint32_t bytes, const std::string& filename, std::uint32_t linenumber, const std::string& tag, const std::chrono::high_resolution_clock::time_point& time, std::uint32_t alignment)
	{
		return GetHeap(heapId).AllocateBlock(bytes, filename, linenumber, tag, time, alignment);
	}

	void HeapManager::FreeBlock(void* ptr)
	{
		CustomHeap::MemoryBlock& block = CustomHeap::MemoryBlockFromPtr(ptr);
		CustomHeap& heap = GetHeap(CustomHeap::HeapID(block));
		heap.FreeBlock(ptr);
	}

	void HeapManager::SetCurrentHeapID(std::uint32_t id)
	{
		mOldHeapID = mCurrentHeapID;
		mCurrentHeapID = id;
	}

	void HeapManager::RestoreHeapID()
	{
		mCurrentHeapID = mOldHeapID;
	}

	bool HeapManager::IsHeapConsistent(std::uint32_t heapID) const
	{
		return GetHeap(heapID).IsHeapConsistent();
	}

	//Memory& HeapManager::GetMemoryRef()
	//{
	//	return *mMemory;
	//}
}
