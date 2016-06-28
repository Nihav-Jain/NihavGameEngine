#pragma once
#include "HeapManager.h"

namespace Library
{
	class Memory
	{
	public:
		Memory() = default;
		Memory(const Memory& rhs) = delete;
		Memory& operator=(const Memory& rhs) = delete;
		virtual ~Memory() = default;

		virtual void* OSMalloc(std::uint32_t numBytes) = 0;
		virtual void OSFree(void* ptr) = 0;
	};
}

//template <typename T, typename ...ArgTypes>
//inline static T* NewObject(ArgTypes&& ...Args)
//{
//	void* ptr = Library::HeapManager::GetHeapManager().AllocateBlock(1, sizeof(T), __FILE__, __LINE__, "DEF", std::chrono::high_resolution_clock::now(), 4);
//	T* tPtr = reinterpret_cast<T*>(ptr);
//	
//	return tPtr;
//};
//
//template <typename T>
//inline static void DeleteObject(T* ptr)
//{
//	ptr->~T();
//	Library::HeapManager::GetHeapManager().FreeBlock(ptr);
//};

#define ENGINE_NEW(heapID, tag) new(heapID, __FILE__, __LINE__, tag, std::chrono::high_resolution_clock::now(), 4)
#define ENGINE_NEW_ALIGNED(heapID, tag, alignemnt) new(heapID, __FILE__, __LINE__, tag, std::chrono::high_resolution_clock::now(), alignment)
#define ENGINE_NEW_NOTIMER(heapID, tag) new(heapID, __FILE__, __LINE__, tag, std::chrono::milliseconds(0), 4)

#define ENGINE_ALLOC(x, id, tag) Library::HeapManager::GetHeapManager()->AllocateBlock(id, x, __FILE__, __LINE__, tag, std::chrono::high_resolution_clock::now())
#define ENGINE_ALLOC_ALIGNED(x, id, tag, alignment) Library::HeapManager::GetHeapManager()->AllocateBlock(id, x, __FILE__, __LINE__, tag, std::chrono::high_resolution_clock::now(), alignment)

#define ENGINE_FREE(ptr) Library::HeapManager::GetHeapManager()->FreeBlock(ptr)
#define ENGINE_DELETE(ptr) ::operator delete(ptr, 1, __FILE__, __LINE__, "DEF", std::chrono::high_resolution_clock::now(), 4)

void* operator new(std::uint32_t allocBytes, std::uint32_t heapId, const std::string& filename, std::uint32_t lineNumber, const std::string& tag, const std::chrono::high_resolution_clock::time_point& time, std::uint32_t alignment);
void operator delete(void* ptr, std::uint32_t heapID, const std::string& filename, std::uint32_t linenumber, const std::string& tag, const std::chrono::high_resolution_clock::time_point& time, std::uint32_t alignment);
