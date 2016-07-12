#include "pch.h"
#include "DesktopFileManager.h"

namespace Library
{
	DesktopFileManagerFactory DesktopFileManager::sFactory;
	RTTI_DEFINITIONS(DesktopFileManager, FileManager);
	ENGINE_MODULE_DEFINITIONS(DesktopFileManager, &DesktopFileManager::sRunTimeTypeId, &FileManager::sInstance);

	DesktopFileManager::DesktopFileManager()
	{
	}


	DesktopFileManager::~DesktopFileManager()
	{
	}

	void DesktopFileManager::GetFileAsync(const std::string& fileName, std::function<void(FileHandle*)>& callback)
	{
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(callback);
	}

	void DesktopFileManager::CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName)
	{
		UNREFERENCED_PARAMETER(outFileHandle);
		UNREFERENCED_PARAMETER(fileName);
	}

}
