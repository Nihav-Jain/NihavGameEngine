#include "pch.h"
#include "UWPFileManager.h"
#include "Engine.h"
#include "EngineModule.h"

namespace Library
{

	UWPFileManagerFactory UWPFileManager::sFactory;
	RTTI_DEFINITIONS(UWPFileManager, FileManager);
	ENGINE_MODULE_DEFINITIONS(UWPFileManager, &UWPFileManager::sRunTimeTypeId, &FileManager::sInstance);
	//const Library::Hashmap<const std::uint64_t*, EngineModule**>::Iterator UWPFileManager::Itr = Engine::ModuleList().Insert(&UWPFileManager::sRunTimeTypeId, reinterpret_cast<EngineModule**>(&FileManager::sInstance));

	UWPFileManager::UWPFileManager()
	{
	}


	UWPFileManager::~UWPFileManager()
	{
	}

	void UWPFileManager::OpenFileAsync(FileHandle* outFileHandle, const std::string& fileName, FileHandle::FileMode mode)
	{
		UNREFERENCED_PARAMETER(outFileHandle);
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(mode);
	}

	void UWPFileManager::CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName)
	{
		UNREFERENCED_PARAMETER(outFileHandle);
		UNREFERENCED_PARAMETER(fileName);
	}
}
