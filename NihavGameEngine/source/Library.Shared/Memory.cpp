#include "pch.h"
#include "Memory.h"
#include "HeapManager.h"

//void* operator new(std::uint32_t bytes)
//{
//
//	if (Library::HeapManager::GetHeapManager() == nullptr)
//		return malloc(bytes);
//	return Library::HeapManager::GetHeapManager()->AllocateBlock(1, bytes, __FILE__, __LINE__, "Default", std::chrono::high_resolution_clock::now());
//}

void* operator new(std::uint32_t allocBytes, std::uint32_t heapId, const std::string& filename, std::uint32_t lineNumber, const std::string& tag, const std::chrono::high_resolution_clock::time_point& time, std::uint32_t alignment)
{
	if (Library::HeapManager::GetHeapManager() == nullptr)
		return ::operator new(allocBytes);
	return Library::HeapManager::GetHeapManager()->AllocateBlock(heapId, allocBytes, filename, lineNumber, tag, time, alignment);
}

void operator delete(void* ptr, std::uint32_t heapID, const std::string& filename, std::uint32_t linenumber, const std::string& tag, const std::chrono::high_resolution_clock::time_point& time, std::uint32_t alignment)
{
	UNREFERENCED_PARAMETER(heapID);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(linenumber);
	UNREFERENCED_PARAMETER(tag);
	UNREFERENCED_PARAMETER(time);
	UNREFERENCED_PARAMETER(alignment);

	if (Library::HeapManager::GetHeapManager() == nullptr)
		return ::operator delete(ptr);
	return Library::HeapManager::GetHeapManager()->FreeBlock(ptr);
}

//void operator delete(void* ptr)
//{
//	if (Library::HeapManager::GetHeapManager() == nullptr)
//		free(ptr);
//	Library::HeapManager::GetHeapManager()->FreeBlock(ptr);
//}
