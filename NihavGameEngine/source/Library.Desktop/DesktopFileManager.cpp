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

	void DesktopFileManager::OpenFileAsync(FileHandle* outFileHandle, const std::string& fileName, FileHandle::FileMode mode)
	{
		UNREFERENCED_PARAMETER(outFileHandle);
		UNREFERENCED_PARAMETER(fileName);
		UNREFERENCED_PARAMETER(mode);
	}

	void DesktopFileManager::CreateFileAsync(FileHandle* outFileHandle, const std::string& fileName)
	{
		UNREFERENCED_PARAMETER(outFileHandle);
		UNREFERENCED_PARAMETER(fileName);
	}

}
